#pragma once


#include <stdbool.h>

#include "tokenizer.h"
#include <sys/wait.h>

#include <unistd.h>

#include <fcntl.h>



#define isEnd(ptrptr) ((ptrptr)=='\0' || (ptrptr)=='\n' || (ptrptr)==' ')


int Exec(struct Tokenizer *t) {

    if (t->token_count==0) {
        return 0;
    }

    char buffer[1000];
    int ptrr = 0;


    if (t->token_count==0) {
        return 0;
    }


    int pid = -1;

    while (true) {
        pid = fork();
        if (pid < 0) {
            exit(-1003);
            return -1;
            continue;
        }
        if (pid>0) {
            break;
        }
        //pid == 0

        char* arr[t->token_count+1];

        struct Token* nxt = t->head;


        int ixxxxxxx = 0;
        for (struct Token* nx = nxt; nx != NULL; nx = nx->next) {
            arr[ixxxxxxx++] = buffer+ptrr;
            for (char *c = nx->start;; c++) {
                if (isEnd(*c)) {
                    buffer[ptrr++] = '\0';
                    break;
                }
                buffer[ptrr++] = *c;
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



        //char* processName = NULL;
        char* outIn[2] = {NULL, NULL};
        char* charOI = "<>";


        int cntIO[] = {0,0};
        for (int i = 0; i < t->token_count; i++) {
            for (int j = 0; j < 2; j++) {
                if (arr[i][0]==charOI[j]) {
                    cntIO[j]++;
                }
            }
        }



        //fprintf(stderr, "k allj f %d \n", (int)t->token_count);
        bool broken = false;
        int arrIx = 0;
        for (int i = 0; i < t->token_count; i++) {

            int skip = 0;
            for (int j = 0; j < 2; j++) {
                char c = charOI[j];


                //fprintf(stderr, " jrewrj wekr ||| %s (%c) (%c) %d \n", arr[i], charOI[j], arr[i][0], i);
                if (arr[i][0]==c) {
                    if (isEnd(arr[i][1])) {
                        outIn[j] = arr[i+1];
                    } else {
                        outIn[j] = arr[i]+1;
                    }
                    skip = 1;
                    i+=1;
                    break;
                }
            }
            if (skip==0) {
                arr[arrIx++] = arr[i];
                //fprintf(stderr, "%s %d \n", arr[arrIx-1], skip);
            }
        }





        for (int i = arrIx; i <= t->token_count; i++) {
            arr[i] = NULL;
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
            exit(-1002);
            return -1;
        }




        int flags[] = {O_RDONLY, O_WRONLY | O_CREAT | O_TRUNC};
        int stream[] = {STDIN_FILENO, STDOUT_FILENO};
        int mode[] = {0, 0644};
        for (int j = 0; j < 2; j++) {
            if (outIn[j]!=NULL) {
                int fin = open(outIn[j], flags[j], mode[j]);
                if (fin<0) {
                    printf("I/O error");
                    exit(-1001);
                    return -1;
                }
                if (dup2(fin, stream[j]) < 0) {
                    printf("CRITICAL ERRORORORORORORORO");
                    return -1;
                }
                close(fin);
            }
        }

            //perror(i);
        //printf("DEBUG NAME = %s \n",arr[0]);


        execvp(arr[0], arr);


        printf("Command not found");
        exit(-1004);
        return -1;
        exit(-1000);
    }


    int status = 0;
    waitpid(pid, &status, 0);



    return 0;







}










#ifndef BASE_BASH_H
#define BASE_BASH_H

#endif //BASE_BASH_H



























































































































































































































































































































































































































































































































































































































































































































































































