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

    time_t start = time(NULL);

    await(14);

    //primeiro filho 14s

    printf("PAI (PID=%d)\nProcesso Filho #1 criado tempo: %ld\n", getpid(),time(NULL)-start);

    filho1 = fork();

    if(filho1 < 0){
        fprintf(stderr,"Erro ao criar processo filho #1");
        exit(-1);
    }else{
        if(filho1 > 0){
            //segundo filho 16s
            await(2);
            printf("Processo Filho #2 criado tempo: %ld\n", time(NULL)-start);
            filho2 = fork();
            if(filho2 < 0){
                fprintf(stderr,"Erro ao criar processo neto #2\n");
                exit(-1);
            }else{
                if(filho2 == 0){
                    //segundo neto 30s
                    await(14);
                    printf("Filho #2(PID=%d)\tNeto #2 tempo: %ld\n",getpid(),time(NULL)-start);
                    neto2 = fork();
                    if(neto2 < 0){
                        fprintf(stderr,"Erro ao criar processo neto #2");
                        exit(-1);
                    }else{
                        if(neto2 == 0){
                            // morte do segundo neto 48s.
                            await(18);
                            printf("Neto #2(PID=%d)\ttempo de morte cronológico a partir do pai: %ld\n", getpid(),time(NULL)-start);
                            exit(0);
                        }else{
                            await(16);
                            // morte do segundo filho 30s, await(16+14)
                            printf("Filho #2\t\ttempo de morte cronológico a partir do pai: %ld\n", time(NULL)-start);
                            exit(0);
                        }
                    }
                }
            }
        }else{
            //primeiro neto 26s
            await(12);
            printf("Filho #1(PID=%d)\tNeto #1 tempo: %ld \n",getpid(),time(NULL)-start);
            neto1 = fork();
            if(neto1 < 0){
                fprintf(stderr,"Erro ao criar processo neto #1\n");
                exit(-1);
            }else{
                if(neto1 == 0){
                    // morte do primeito neto 38s
                    await(12);
                    printf("Neto #1(PID=%d)\ttempo de morte cronológico a partir do pai: %ld\n", getpid(),time(NULL)-start);
                    exit(0);
                }else{
                    //morte do primeiro filho 44s
                    await(18);
                    // morre com 30s, await(18+12)
                    wait(NULL);
                    printf("Filho #1\t\ttempo de morte cronológico a partir do pai: %ld\n", time(NULL)-start);
                    exit(0);
                }
            }
        }
    }

    wait(NULL);

    await(16);

    if(filho1>0){
        printf("Tempo de morte do pai: %ld\n", time(NULL) - start);
    }
    
    return 0;

}