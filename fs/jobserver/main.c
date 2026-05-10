#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <err.h>

int job_pipe[2];

void acquire() {
    char token;
    if (read(job_pipe[0], &token, 1) != 1) {
        exit(1);
    }
}

void release() {
    char token = 'c';
    if (write(job_pipe[1], &token, 1) != 1) {
        exit(1);
    }
}

void tryRun(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) return;
    if (!S_ISREG(st.st_mode)) return;
    if (access(path, X_OK) != 0) return;

    acquire();

    pid_t watcher_pid = fork();
    if (watcher_pid == -1)
        err(1, "fork watcher");

    if (watcher_pid == 0) {
        pid_t task_pid = fork();
        if (task_pid == -1)
            err(1, "fork task");

        if (task_pid == 0) {
            execl(path, path, (char *)NULL);
            err(1, "unreachab;e");
        }

        int status;
        waitpid(task_pid, &status, 0);

        release();
        _exit(0);
    }
}

void walk(const char *dir_path) {
    DIR *d = opendir(dir_path);
    if (d == NULL) return;

    struct dirent *entry;
    while ((entry = readdir(d)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        struct stat st;
        if (lstat(full_path, &st) != 0) continue;

        if (S_ISDIR(st.st_mode)) {
            pid_t child_walk = fork();
            if (child_walk == -1)
                err(1, "fork erreorear");
            if (child_walk == 0) {
                walk(full_path);
                closedir(d);
                _exit(0);
            }
        } else {
            tryRun(full_path);
        }
    }
    closedir(d);

    while (wait(NULL) > 0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    int k = atoi(argv[1]);
    if (k < 1) k = 1;

    if (pipe(job_pipe) == -1) {
        err(1, "pipe");
    }

    for (int i = 0; i < k; i++) {
        release();
    }

    walk(".");

    return 0;
}