
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "leituras.h"
#include <conio.h>
void mostraDados(DadosEleicEstado *dados, int quantidadeEstados){
    for(int i = 0; i<quantidadeEstados; i++){
        printf("Estado: %s\n", dados[i].estado);
        for(int j = 0; j<dados[i].quantidadeCidades; j++){
            printf("cidade: %s\n", dados[i].cidadesEst[j].nomeCidade);
            for(int k = 0; k<dados[i].cidadesEst[j].quantidadeCanditadosCidades; k++){

                printf("candidato: %s\n", dados[i].cidadesEst[j].cand[k].nomeCompleto);
                printf("numero: %d\n", dados[i].cidadesEst[j].cand[k].numeroCandidato);
                printf("apelido: %s\n", dados[i].cidadesEst[j].cand[k].apelido);
                printf("cargo: %s\n", dados[i].cidadesEst[j].cand[k].cargo);
                printf("deferido: %s\n", dados[i].cidadesEst[j].cand[k].deferido);
                printf("partido: %s\n", dados[i].cidadesEst[j].cand[k].partido);
                printf("quantidade Votos: %d\n", dados[i].cidadesEst[j].cand[k].quantidadeVotos);
                printf("\n\n");
            }
            printf("\n\n\n\n\n");
            getchar();
        }

    }
}
typedef struct c{

    int a;
    char b;
}c;
void *retorna(int a, float b, c s, int escolha){

    int *memoria = malloc(sizeof(int));
    *memoria = a;
    switch (escolha) {

        case 1:
            return memoria;

        case 2:
            return &b;
        case 3:
            return &s;

    }
}

int main(){
/*
    DadosEleicEstado *estados;
    int quantidadeEstados;

    estados = leDadosEleicoes(&quantidadeEstados);

    mostraDados(estados, quantidadeEstados);
    getch();

*/
    void *endress = retorna(9, 5.2, (c){1,'b'}, 1);
    printf("%d", *((int *)endress));

}
