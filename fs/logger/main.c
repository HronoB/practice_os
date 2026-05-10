// Your code here


#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/signal.h>


volatile sig_atomic_t pend = 0;
void sighupHandle(int signo) {
    pend = 1;
}




int currIx = 0;
void rotate(char* name) {
    currIx++;


    for (int i = currIx; i >=0; i--) {
        int maxsize = 40;
        char old[maxsize];
        char nww[maxsize];
        if (i>0) {
            snprintf(old, maxsize, "%s.%d",name,i);
        } else {
            snprintf(old, maxsize,"%s",name);
        }
        snprintf(nww, maxsize, "%s.%d",name,i+1);

        rename(old, nww);
    }

    FILE *rr = fopen(name, "w");
    if (rr)
        fclose(rr);


}






int main(int args, char* argv[]) {

    struct sigaction sa = {0};
    sa.sa_handler = sighupHandle;

    sigaction(SIGHUP, &sa, NULL);

    char* fname = argv[1];

    int bfsz = 4096;
    char buff[bfsz];

    FILE *ff = NULL;
    while (true) {


        if (pend) {
            fflush(ff);
            fclose(ff);
            ff = NULL;
            rotate(fname);
            pend = 0;
        }

        if (ff==NULL) {
            ff = fopen(fname, "a");
            if (ff==NULL) {
                return 1;
            }
        }
        errno = 0;
        if (fgets(buff, bfsz, stdin)==0) {
            if (errno==EINTR) {
                clearerr(stdin);
                continue;
            }
            break;
        }
        fputs(buff, ff);









    }

    if (ff!=NULL) {
        fclose(ff);
    }

}







