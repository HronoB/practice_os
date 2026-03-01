#include "bash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int GetCmd(char** line, size_t* maxlen) {
    if (true) {
        printf("$ ");
        return getline(line, maxlen, stdin);
    } else {
        static int pp = 1;
        if (*line) {
            free(*line);
        }
        *line = strdup("echo hello");
        //*line = strdup("echo hello world > a.txt");
        return pp--;
    }
}

int main() {
    char* line = NULL;
    size_t maxlen = 0;
    setbuf(stdin, NULL);

    ssize_t len = 0;
    struct Tokenizer tokenizer;

    while ((len = GetCmd(&line, &maxlen)) > 0) {

        //printf("debug %s\n", line);

        TokenizerInit(&tokenizer, line);

        Exec(&tokenizer);

        TokenizerFree(&tokenizer);
    }

    if (line) {
        free(line);
    }

    printf("\n");

    return 0;
}
