#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

// pai morre em 60s, pai tem um filho aos 14s, segundo filho aos 16s, neto aos 26s e 30s, filho1 e filho2 morre aos 44s e 46s, neto morre depois de 38s e depois de 48s

int main(){

    pid_t pid[4] = {-1,-1,-1,-1};

    sleep(14);

    //primeiro filho 14s

    pid[0] = fork();

    if(pid[0] < 0){
        fprintf(stderr,"Erro ao criar processo filho #1");
        exit(-1);
    }else{
        if(pid[0] == 0){
            //primeiro neto 26s
            printf("Filho #1: (PID=%d)\n", getpid());
            printf("Filho #1: Criando neto #1\n");
            sleep(10);
            pid[2] = fork();
            if(pid[2] < 0){
                fprintf(stderr,"Erro ao criar processo neto #2");
                exit(-1);
            }else{
                if(pid[2] == 0){
                    // morte do primeito neto 38s
                    sleep(8);
                    pid_t filho1 = getppid();
                    printf("Matando primeiro neto 38s\n");
                    kill(getpid(),0);
                    sleep(6);
                    //morte do primeiro filho 44s
                    printf("Matando primeiro filho 44s\n");
                    kill(filho1,0);
                    wait(NULL);
                }
            }
            wait(NULL);
        }else{
            printf("Processo Filho #2 criado\n");
            sleep(2);
            //segundo filho 16s
            pid[1] = fork();
            if(pid[1] < 0){
                fprintf(stderr,"Erro ao criar processo filho #2");
            }else{
                if(pid[1] == 0){
                    printf("Filho #2: (PID=%d)\n", getpid());
                    //segundo neto 30s
                    printf("Filho #2: Criando neto #2\n");
                    sleep(4);
                    pid[3] = fork();
                    if(pid[3] < 0){
                        fprintf(stderr,"Erro ao criar processo neto #2");
                        exit(-1);
                    }else{
                        if(pid[3] == 0){
                            // morte do segundo neto 44s
                            pid_t filho2 = getppid();
                            printf("Matando segundo neto 44s\n");
                            kill(getpid(),0);
                            sleep(2);
                            // morte do segundo filho 46s
                            printf("Matando segundo filho 46s\n");
                            kill(filho2,0);
                            wait(NULL);
                        }
                    }
                    wait(NULL);
                }
            } 
        }
    } 

    wait(NULL);

    sleep(14);
    
    return 0;

}