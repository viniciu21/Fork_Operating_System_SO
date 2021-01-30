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
    time_t mortes[5];
    time_t nascimento[5];
    FILE *file;

    file = fopen("mortes.txt","w");

    if(!file){
        printf("Erro ao abrir arquivo\n");
        exit(-1);
    }

    nascimento[0] = time(NULL);

    await(14);

    //primeiro filho 14s

    nascimento[1] = time(NULL);

    printf("PAI (PID=%d)\n", getpid());
    printf("Nascimento Filho #1: %ld\n", nascimento[1]);

    filho1 = fork();

    if(filho1 < 0){
        fprintf(stderr,"Erro ao criar processo filho #1\n");
        exit(-1);
    }else{
        if(filho1 > 0){
            //segundo filho 16s
            await(2);
            nascimento[2] = time(NULL);
            printf("Nascimento Filho #2: %ld\n", nascimento[2]);
            filho2 = fork();
            if(filho2 < 0){
                fprintf(stderr,"Erro ao criar processo neto #2\n");
                exit(-1);
            }else{
                if(filho2 == 0){
                    //segundo neto 30s
                    await(14);
                    nascimento[4] = time(NULL);
                    printf("Filho #2(PID=%d)\tNascimento Neto #2 Hora: %ld\n",getpid(),nascimento[4]);
                    neto2 = fork();
                    if(neto2 < 0){
                        fprintf(stderr,"Erro ao criar processo neto #2");
                        exit(-1);
                    }else{
                        if(neto2 == 0){
                            // morte do segundo neto 18s.
                            await(18);
                            mortes[4] = time(NULL);
                            printf("Neto #2(PID=%d)\tHora de morte: %ld\tTempo vivido: %.1lf\tNascimento: %ld\n", getpid(),mortes[4], difftime(mortes[4],nascimento[4]), nascimento[4]);
                            fprintf(file, "Neto 2, tempo vivido: %.1lf\n", difftime(mortes[4],nascimento[4]));
                            exit(0);
                        }else{
                            await(16);
                            // morte do segundo filho 30s, await(16+14)
                            mortes[2] = time(NULL);
                            printf("Filho #2\t\tHora de morte: %ld\tTempo vivido: %.1lf\tNascimento: %ld\n", mortes[2], difftime(mortes[2],nascimento[2]), nascimento[2]);
                            fprintf(file, "Filho 2, tempo vivido: %.1lf\n", difftime(mortes[2],nascimento[2]));
                            exit(0);
                        }
                    }
                }
            }
        }else{
            //primeiro neto 26s
            await(12);
            nascimento[3] = time(NULL);
            printf("Filho #1(PID=%d)\tNascimento Neto #1 Hora: %ld\n",getpid(),nascimento[3]);
            neto1 = fork();
            if(neto1 < 0){
                fprintf(stderr,"Erro ao criar processo neto #1\n");
                exit(-1);
            }else{
                if(neto1 == 0){
                    // morte do primeito neto 38s
                    await(12);
                    mortes[3] = time(NULL);
                    printf("Neto #1(PID=%d)\tHora de morte: %ld\tTempo vivido: %.1lf\tNascimento: %ld\n", getpid(),mortes[3], difftime(mortes[3],nascimento[3]), nascimento[3]);
                    fprintf(file, "Neto 1, tempo vivido: %.1lf\n", difftime(mortes[3],nascimento[3]));
                    exit(0);
                }else{
                    //morte do primeiro filho 44s
                    await(18);
                    // morre com 30s, await(18+12)
                    wait(NULL);
                    mortes[1] = time(NULL);
                    printf("Filho #1\t\tHora de morte: %ld\tTempo vivido: %.1lf\tNascimento: %ld\n", mortes[1], difftime(mortes[1],nascimento[1]), nascimento[1]);
                    fprintf(file, "Filho 1, tempo vivido: %.1lf\n", difftime(mortes[1],nascimento[1]));
                    exit(0);
                }
            }
        }
    }

    wait(NULL);

    await(16);

    mortes[0] = time(NULL);

    printf("Hora de morte do pai:\t%ld\t\tTempo vivido: %.1lf\tNascimento: %ld\n", mortes[0],difftime(mortes[0],nascimento[0]), nascimento[0]);
    fprintf(file, "Pai, tempo vivido: %.1lf\n", difftime(mortes[0],nascimento[0]));
    
    return 0;

}