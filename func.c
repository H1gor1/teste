#include "leituras.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define PEGACAMINHO(CAMINHO) printf("Digite o caminho para o arquivo %s\n", CAMINHO);

void *alocaMemoria(size_t tamanho){
    void *ptr;

    ptr = malloc(tamanho);

    if(!ptr){
        printf("ERRO: Memoria indisponivel!\n");
        printf("ERRO: %s\n", strerror(errno));
        exit(1);
    }
    return ptr;
}
void *realocaMemoria(void *ptr, size_t tam){

    void *ptrRealocar;

    ptrRealocar = realloc(ptr, tam);

    if(!ptrRealocar){
        printf("ERRO: Memoria indisponivel!\n");
        printf("ERRO: %s\n", strerror(errno));
        exit(1);
    }
    return ptrRealocar;

}
void *limpaMemoria(void *ptr){
    free(ptr);
    return NULL;
}
void fechaArquivo(FILE **f){
    fclose(f[0]);
    f[0] = NULL;
}
int digText(FILE *fluxo, char **string, char caracterParar){

    int indice = 0;
    string[0] = alocaMemoria(sizeof(char));

    do {

        string[0][indice] = fgetc(fluxo);
        indice++;
        if(string[0][indice-1]!=caracterParar){
            string[0] = realocaMemoria(string[0], sizeof(char)*(indice+1));

        }
    } while (string[0][indice-1] != caracterParar);
    string[0][indice-1] = '\0';
    return indice;
}
void avancAtepalavra(char *palavra, int tamanho, FILE *f){

    char letra;
    int indice = 0;
    while(indice<tamanho){
        letra = fgetc(f);
        if(feof(f)){
            return;
        }


        indice = (letra == palavra[indice])
                ?indice+1
                :0;
    }
    return;


}
int pegaDadosCandidato(dadosCandidatos **candidato, int quantidadeCanditadoAnt, FILE *f){

    int numeroTemp;

    avancAtepalavra("<td>", 4, f);
    fscanf(f, "%d", &numeroTemp);

    if(!quantidadeCanditadoAnt || numeroTemp != candidato[0][quantidadeCanditadoAnt].numeroCandidato ) {

        candidato[0] = (!quantidadeCanditadoAnt)
                       ? alocaMemoria(sizeof(dadosCandidatos))
                       : realocaMemoria(candidato[0], sizeof(dadosCandidatos) * (quantidadeCanditadoAnt + 1));

        quantidadeCanditadoAnt++;
        candidato[0][quantidadeCanditadoAnt-1] = (dadosCandidatos) {0, NULL, NULL, NULL, NULL, 0};

    }
    avancAtepalavra("<td>", 4, f);
    candidato[0][quantidadeCanditadoAnt-1].numeroCandidato = numeroTemp;

    digText(f, &candidato[0][quantidadeCanditadoAnt-1].nomeCompleto, '<');

    avancAtepalavra("<td>", 4, f);
    digText(f, &candidato[0][quantidadeCanditadoAnt-1].apelido, '<');

    avancAtepalavra("<td>", 4, f);
    digText(f, &candidato[0][quantidadeCanditadoAnt-1].cargo, '<');

    avancAtepalavra("<td>", 4, f);
    digText(f, &candidato[0][quantidadeCanditadoAnt-1].deferido, '<');

    avancAtepalavra("<td>", 4, f);
    digText(f, &candidato[0][quantidadeCanditadoAnt-1].partido, '<');

    avancAtepalavra("<td>", 4, f);
    fscanf(f, "%d", &numeroTemp);
    candidato[0][quantidadeCanditadoAnt-1].quantidadeVotos+=numeroTemp;

    return quantidadeCanditadoAnt;


}
int pegaCidade(dadosEleicCid **cidade, int quantidadeCidadesAnt, FILE *f){
    char *nomeCidadeTemp;

    avancAtepalavra("<td>", 4, f);
    digText(f, &nomeCidadeTemp, '<');
    if(!quantidadeCidadesAnt || strcmp(nomeCidadeTemp, cidade[0][quantidadeCidadesAnt-1].nomeCidade)){
        cidade[0] = (!quantidadeCidadesAnt)? alocaMemoria(sizeof(dadosEleicCid)): realocaMemoria(cidade[0], sizeof(dadosEleicCid)*(quantidadeCidadesAnt+1));
        quantidadeCidadesAnt++;
        cidade[0][quantidadeCidadesAnt-1] = (dadosEleicCid){NULL,NULL, 0 };


    }
    cidade[0][quantidadeCidadesAnt-1].nomeCidade = nomeCidadeTemp;
    cidade[0][quantidadeCidadesAnt-1].quantidadeCanditadosCidades = pegaDadosCandidato(&cidade[0][quantidadeCidadesAnt-1].cand,
                                                                                     cidade[0][quantidadeCidadesAnt-1].quantidadeCanditadosCidades,
                                                                                     f);
    return quantidadeCidadesAnt;

}
DadosEleicEstado *leDadosEleicoes(int *retornar){
    DadosEleicEstado *dados;
    int quantidadeEstados = 1;
    char *diretorioArquivo;

    PEGACAMINHO("de eleicoes html");
   // digText(stdin, &diretorioArquivo, '\n');

    FILE *f;

    f = fopen("Eleicoes.html", "r");

    if(!f){
        printf("nao existe este arquivo no caminho indicado!\n");
        diretorioArquivo = limpaMemoria(diretorioArquivo);
        return NULL;
    }

    avancAtepalavra("table", 5, f);
    avancAtepalavra("tr>", 3, f);
    avancAtepalavra("td>", 3, f);
    dados = alocaMemoria(sizeof(DadosEleicEstado));
    dados[0] = (DadosEleicEstado){NULL, NULL, 0};

    digText(f, &dados[0].estado, '<');
    pegaCidade(&dados[0].cidadesEst, dados[0].quantidadeCidades, f);

    while(1){
        avancAtepalavra("<tr>", 4, f);
        avancAtepalavra("<td>", 4, f);
        if(feof(f)){
            break;
        }
        digText(f, &diretorioArquivo, '<');

        if(strcmp(diretorioArquivo, dados[quantidadeEstados-1].estado)){

            quantidadeEstados++;

            dados = realocaMemoria(dados, sizeof(DadosEleicEstado)*quantidadeEstados);

            dados[quantidadeEstados-1] = (DadosEleicEstado){NULL, NULL, 0};



        }

        dados[quantidadeEstados-1].quantidadeCidades =  pegaCidade(&dados[quantidadeEstados-1].cidadesEst, dados[quantidadeEstados-1].quantidadeCidades, f);
        diretorioArquivo = limpaMemoria(diretorioArquivo);
    }




    *retornar = quantidadeEstados;
    return dados;




}