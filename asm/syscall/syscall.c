



#include "syscall.h"



ssize_t syscall(size_t sysnum, ssize_t arg1, ssize_t arg2, ssize_t arg3) {
    size_t rdx, rax, rdi, rsi;

    rdi = arg1;
    rsi = arg2;
    rdx = arg3;

    asm volatile(""
        ""
        "movq %0, %4;"
        "syscall;"
        "movq %4, %0;"
        ""
        ""
        //      0            1          2          3        4          5         6            7
        : "+r"(sysnum), "+D"(rdi), "+S"(rsi), "+d"(rdx), "+a"(rax)
        :
        : "rcx", "r11", "memory" );

    return sysnum;
}



// Opens the file specified by pathname.
int open(const char* pathname, int flags){
    return syscall(2, (size_t)pathname, flags, 0);
}

// Closes a file descriptor, so that it no longer
// refers to any file and may be reused.
int close(int fd) {
    return syscall(3, fd, 0, 0);
}

// Attempts to read up to count bytes from file
// descriptor fd into the buffer starting at buf.
ssize_t read(int fd, void* buf, size_t count) {
    return syscall(0, fd, (size_t)buf, count);
}

// Writes up to count bytes from the buffer starting at
// buf to the file referred to by the file descriptor fd.
ssize_t write(int fd, const void* buf, ssize_t count) {
    return syscall(1, fd, (size_t)buf, count);
}

// Creates a pipe, a unidirectional data channel
// that can be used for interprocess communication.
int pipe(int pipefd[2]) {
    return syscall(22, (size_t)pipefd, 0, 0);
}

// Creates a copy of the file descriptor oldfd, using the
// lowest-numbered unused file descriptor for the new descriptor.
int dup(int oldfd) {
    return syscall(32, oldfd, 0, 0);
}

// Creates a new process by duplicating the calling process.
pid_t fork() {
    return syscall(57, 0, 0, 0);
}

// Wait for state changes in a child of the calling process.
pid_t waitpid(pid_t pid, int* wstatus, int options) {
    return syscall(61, pid, (size_t) wstatus, options);
}

// Executes the program pointed to by filename.
int execve(const char* filename, char* const argv[], char* const envp[]) {
    return syscall(59, (size_t)filename, (size_t)argv, (size_t)envp);
}

// Terminates the calling process.
void exit(int status) {
    syscall(60, status, 0, 0);
}































