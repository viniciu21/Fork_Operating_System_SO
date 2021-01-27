#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(){

    pid_t filho1, filho2, neto1, neto2;

    sleep(14);

    filho1 = fork();

    if(filho1 < 0){
        fprintf(stderr,"Erro ao criar processo filho #1");
        exit(-1);
    }else if(filho1 > 0){
        sleep(2);
        printf("aqui é o pai %d \n", getpid());
        // printf("Aqui tem o processo Numero %d \n \n \n", getpid());

        filho2 = fork();
            if(filho2 < 0){
                fprintf(stderr,"Erro ao criar processo filho #2");
                exit(-1);
            }
        // pid_t pidFillho2 = wait(NULL);
        // if(pidFillho2 != -1){
        //     printf("oh no my kid is death %d", pidFillho2);
        // }

    }else if(filho1 == 0){

        sleep(2);
        printf("Teste");
        sleep(10);
        neto1 = fork();
            if(neto1 < 0){
                fprintf(stderr,"Erro ao criar processo neto #1");
                exit(-1);
            }else if(neto1 == 0){
                sleep(8);
                printf("fui kilado %d", getpid());
                kill(getpid(), 0);
            }
        pid_t pidNeto1 = wait(NULL);
        if(pidNeto1 != -1){
            printf("oh no my kid is death %d", pidNeto1);
            sleep(6);
            printf("fui kilado %d", getpid());
            kill(getpid(), 0);
        }

    }else if(filho2 == 0){
        sleep(14);
        neto2 = fork();
            if(neto2 < 0){
                fprintf(stderr,"Erro ao criar processo neto #1");
                exit(-1);
            }else if(neto2 == 0){
                sleep(18);
                printf("fui kilado %d", getpid());
                kill(getpid(), 0);
            }
        if(filho2 == 0) {
            sleep(16);
            printf("fui kilado %d", getpid());
            kill(getpid(), 0);
        }
        
    }else{
        printf("Aqui tem o processo Numero %d \n \n \n", getpid());
        wait(NULL);
        printf("fui kilado %d", getpid());
    }
    // printf("entrou aqui, %d \n", getpid());
    // fflush(stdout);
    wait(NULL);
    kill(getpid(), 0);
}