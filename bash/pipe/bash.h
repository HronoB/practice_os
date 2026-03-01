#pragma once


#include <stdbool.h>

#include "tokenizer.h"
#include <sys/wait.h>

#include <unistd.h>

#include <fcntl.h>



#define isEnd(ptrptr) ((ptrptr)=='\0' || (ptrptr)=='\n' || (ptrptr)==' ')

#define isSpecial(ptrptr) ((ptrptr)=='\0' || (ptrptr)=='<' || (ptrptr)=='>' || (ptrptr)=='|' )

int Exec(struct Tokenizer *t) {
    if (t->token_count==0) {
        return 0;
    }

    char buffer[1000];
    char* ptrBuffer[1000];
    char** ptrBufferPtr = ptrBuffer;
    char* bufferPtr = buffer;


    char* arr[1000];

    {

        struct Token* nxt = t->head;


        int ixxxxxxx = 0;
        for (struct Token* nx = nxt; nx != NULL; nx = nx->next) {
            arr[ixxxxxxx++] = bufferPtr;
            for (char *c = nx->start;; c++) {
                if (isEnd(*c)) {
                    *(bufferPtr++) = '\0';
                    break;
                }
                *(bufferPtr++) = *c;
            }
        }
        arr[ixxxxxxx] = NULL;



        if (false) {
            fprintf(stderr, "START START START %d \n", (int)t->token_count);
            for (char** i = arr; *i != NULL; i++) {

                fprintf(stderr, "%s \n", *i);
                if (false) {
                    for (char *c = *i;; c++) {
                        fprintf(stderr, " %c:%d", *c, *c);
                        if (isEnd(*c) || (c-*i)>10) {
                            break;
                        }
                    }
                }
                fprintf(stderr, "\n");
            }
        }




    }




    int numOfProc = 1;
    for (int i = 0; i < t->token_count; i++) {
        if (arr[i][0]=='|') {
            numOfProc++;
        }
    }





    char* symbols = "<>";

    //char* nameOfProc[100];
    char* nameOfRer[100][2];
    char** args[100];


    int flags[] = {O_RDONLY, O_WRONLY | O_CREAT | O_TRUNC};
    int stream[] = {STDIN_FILENO, STDOUT_FILENO};
    int mode[] = {0, 0644};

    int descs[100][2];

    int ixProc = 0;

    bool broken = false;


    // init everything to -1
    for (int i = 0; i < numOfProc; i++) {
        //nameOfProc[i] = NULL;
        nameOfRer[i][0] = NULL;
        nameOfRer[i][1] = NULL;
        args[i] = NULL;
        descs[i][0] = -1;
        descs[i][1] = -1;
    }

    args[0] = ptrBufferPtr;
    for (int i = 0; i < t->token_count; i++) {
        if (arr[i][0]=='|') {
            ixProc++;

            *(ptrBufferPtr++) = NULL;
            args[ixProc] = ptrBufferPtr;

            continue;
        }

        int skip = 0;
        for (int j = 0; j < 2; j++) {
            if (arr[i][0]==symbols[j]) {
                if (i+1==t->token_count || isSpecial(arr[i+1][0])) {
                    broken = true;
                } else {
                    if (nameOfRer[ixProc][j] != NULL) {
                        broken = true;
                    }
                    nameOfRer[ixProc][j] = arr[i+1];
                    skip = 1;
                }
            }
        }
        if (skip==1) {
            i++;
            continue;
        }
        *(ptrBufferPtr++) = arr[i];
    }
    *(ptrBufferPtr++) = NULL;



    {
        for (int i = 1; i < numOfProc-1; i++) {
            if (nameOfRer[i][0]!=NULL || nameOfRer[i][1]!=NULL) {
                broken = true;
            }
        }
        if (numOfProc>1 && (nameOfRer[0][1]!=NULL || nameOfRer[numOfProc-1][0]!=NULL)) {
            broken = true;
        }

        for (int i = 0; i < numOfProc; i++) {
            if (args[i][0]==NULL) {
                broken = true;
            }
        }

    }



    if (broken) {
        printf("Syntax error");
        return -1;
    }

    if (false) {
        printf("prog = %s\n    \rargs = ", args[0][0]);
        for (char** i = args[0]; *i != NULL; i++) {
            printf("%s ", *i);
        }
        printf("\n");
    }


    //printf("debug test");
    int pid = fork();
    if (pid>0) {

        int status = 0;
        waitpid(pid, &status, 0);

        return status;
    }
    //printf("debug test");







    for (int i =0 ; i < numOfProc; i++) {
        for (int j = 0; j < 2; j++) {

            if (nameOfRer[i][j]!=NULL) {
                int fin = open(nameOfRer[i][j], flags[j], mode[j]);
                if (fin<0) {
                    printf("I/O error");
                    exit(-1001);
                }
                descs[i][j] = fin;
            }
        }
    }





    //int prevPipe = -1;
    int pidArray[100];
    for (int i = 0; i < 100; i++) {
        pidArray[i] = 0;
    }
    int previousOutput = -1;
    int nextInput = -1;

    int pipefd[2] = {-1,-1};

#define closeIfPoss(a) (()a)==-1 ? 0 : close((a)), (a) = -1;)
    for (int i = 0; i < numOfProc; i++) {



        if (i+1!=numOfProc) {
            pipe(pipefd);
        } else {
            pipefd[0] = -1, pipefd[1] = -1;
        }



        int pid = fork();
        if (pid>0) {


            close(previousOutput);
            previousOutput = pipefd[0];
            close(pipefd[1]);

            pidArray[i] = pid;
            continue;
        } else {

            nextInput = pipefd[1];
            close(pipefd[0]);

        }



        for (int j = 0; j < 2; j++) {
            if (nameOfRer[i][j]!=NULL) {
                dup2(descs[i][j],stream[j]);
            }
        }

        if (previousOutput!=-1) {
            dup2(previousOutput, STDIN_FILENO);
            close(previousOutput);
        }
        if (nextInput!=-1) {
            dup2(nextInput, STDOUT_FILENO);
            close(nextInput);
        }



        for (int i = 0; i < numOfProc; i++) {
            for (int j = 0; j < 2; j++) {
                if (descs[i][j]!=-1) {
                    close(descs[i][j]);
                }
            }
        }
        close(pipefd[0]);
        close(pipefd[1]);

        execvp(args[i][0], args[i]);

        printf("Command not found");
        exit(-10000);
    }

    for (int i = 0; i < numOfProc; i++) {
        if (pidArray[i]==0) {
            break;
        }
        int status = 0;
        waitpid(pidArray[i], &status, 0);
    }


    return 0;



}










#ifndef BASE_BASH_H
#define BASE_BASH_H

#endif //BASE_BASH_H



























































































































































































































































































































































































































































































































































































































































































































































































