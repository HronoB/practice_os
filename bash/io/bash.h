#pragma once


#include <stdbool.h>

#include "tokenizer.h"
#include <sys/wait.h>

#include <unistd.h>

#include <fcntl.h>



#define isEnd(ptrptr) (ptrptr=='\0' || ptrptr=='\n' || ptrptr==' ')

int Exec(struct Tokenizer *t) {


    if (t->token_count==0) {
        return 0;
    }


    int pid = -1;

    while (true) {
        pid = fork();
        if (pid < 0) {
            return -1;
            continue;
        }
        if (pid>0) {
            break;
        }
        //pid == 0

        char* arr[t->token_count+1];
        struct Token* nxt = t->head;


        //char* processName = NULL;
        char* outIn[2] = {NULL, NULL};
        char* charOI = "<>";


        bool broken = false;
        int arrIx = 0;
        for (int i = 0; i < t->token_count;) {
            int skip = 0;
            for (int j = 0; j < 2; j++) {
                char c = charOI[j];
                if (nxt->start[0]==c) {
                    if (isEnd(nxt->start[1])) {
                        outIn[j] = nxt->start;
                        skip = 2;
                    } else {
                        outIn[j] = nxt->start+1;
                    }
                    skip = 1;
                    break;
                }
            }
            if (skip==0) {
                arr[arrIx++] = nxt->start;
                skip = 1;
            }
            while (skip--) {
                nxt = nxt->next;
                i++;
            }
        }

        int cntIO[] = {0,0};
        nxt = t->head;
        for (int i = 0; i < t->token_count; i++) {
            for (char* c = nxt->start; !isEnd(*c); c++) {
                for (int j = 0; j < 2; j++) {
                    if (*c==charOI[j]) {
                        cntIO[j]++;
                    }
                }
            }
            nxt = nxt->next;
        }

        for (int j = 0; j < 2; j++) {
            if (cntIO[j]>1) {
                broken = true;
            }
            if (cntIO[j]==1 && outIn[j]==NULL) {
                broken = true;
            }
        }

        if (broken) {
            printf("Syntax error");
            return -1;
        }




        int flags[] = {O_RDONLY, O_WRONLY | O_CREAT | O_TRUNC};
        int stream[] = {STDIN_FILENO, STDOUT_FILENO};
        for (int j = 0; j < 2; j++) {
            if (outIn[j]!=NULL) {
                int fin = open(outIn[j], flags[j]);
                if (fin<0) {
                    printf("I/O error");
                    return -1;
                }
                if (dup2(fin, stream[j]) < 0) {
                    printf("CRITICAL ERRORORORORORORORO");
                    return -1;
                }
                close(fin);
            }
        }


        arr[arrIx] = NULL;


        execvp(arr[0], arr);


        printf("Command not found");
        return -1;
        //exit(-1000);
    }


    int status = 0;
    waitpid(pid, &status, 0);



    return 0;







}










#ifndef BASE_BASH_H
#define BASE_BASH_H

#endif //BASE_BASH_H



























































































































































































































































































































































































































































































































































































































































































































































































