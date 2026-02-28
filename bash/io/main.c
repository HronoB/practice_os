#include "bash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int GetCmd(char** line, size_t* maxlen) {

    static int nx = 1;
    if (true) {
        printf("$ ");
        return getline(line, maxlen, stdin);
    } else {
        if (nx--) {
            *line = strdup("cat <a.txt");
            return 1;
        }
        return 0;
    }
}

int main() {
    char* line = NULL;
    size_t maxlen = 0;
    setbuf(stdin, NULL);

    ssize_t len = 0;
    struct Tokenizer tokenizer;

    while ((len = GetCmd(&line, &maxlen)) > 0) {
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
