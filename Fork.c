#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

// pai morre em 60s, pai tem um filho aos 14s, segundo filho aos 16s, neto aos 26s e 30s, filho1 e filho2 morre aos 44s e 46s, neto morre depois de 38s e depois de 48s

void await(int t){

    time_t start = time(NULL);

    for(; time(NULL)-start < t;);

}

int main(){

    pid_t filho1,filho2,neto1,neto2;

    await(14);

    //primeiro filho 14s

    filho1 = fork();

    printf("PAI (PID=%d):Processo Filho #1 criado\n", getppid());

    if(filho1 < 0){
        fprintf(stderr,"Erro ao criar processo filho #1");
        exit(-1);
    }else{
        if(filho1 > 0){
            //segundo filho 16s
            await(2);
            printf("Processo Filho #2 criado\n");
            filho2 = fork();
            if(filho2 < 0){
                fprintf(stderr,"Erro ao criar processo neto #2\n");
                exit(-1);
            }else{
                if(filho2 == 0){
                    //segundo neto 30s
                    printf("Filho #2: Criando neto #2\n");
                    await(4);
                    neto2 = fork();
                    if(neto2 < 0){
                        fprintf(stderr,"Erro ao criar processo neto #2");
                        exit(-1);
                    }else{
                        if(neto2 == 0){
                            await(18);
                            // morte do segundo neto 44s
                            printf("Matando segundo neto 44s\n");
                            await(5);
                            // morte do segundo filho 48s
                            printf("Matando segundo filho 48s\n");
                        }
                    }
                    wait(NULL);
                }
            }
            wait(NULL);
        }else{
            //primeiro neto 26s
            printf("Filho #1: Criando neto #1\n");
            await(10);
            neto1 = fork();
            if(neto1 < 0){
                fprintf(stderr,"Erro ao criar processo neto #1\n");
                exit(-1);
            }else{
                if(neto1 == 0){
                    // morte do primeito neto 38s
                    await(12);
                    printf("Matando primeiro neto 38s\n");
                    await(6);
                    //morte do primeiro filho 44s
                    printf("Matando primeiro filho 44s\n");
                }
            }
            wait(NULL);
        }
    }

    wait(NULL);

    await(14);
    
    return 0;

}