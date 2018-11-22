/*
 * Projeto de Computação I: Sokoban em C. 11/2018
 *
 * Marcos  Paulo Paolino e Marco  Cabral Campelo
 *
 * Referência:
 *
 * 0 = Vazio
 * 1 = Parede
 * 2 = Caixa
 * 3 = DestinoCaixa
 * 4 = Jogador
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <memory.h>

typedef enum { false, true } bool;

int moves;

char nomeFase[13];

struct Jogador{
    int moves;
    char nome[21];
};

int contaLinhas(char *nome){
    int ch;
    int linhas = 0;
    FILE *arq = fopen(nome,"r");
    while(!feof(arq))
    {
        ch = fgetc(arq);
        if(ch == '\n')
            linhas++;
    }
    return linhas;
}

void pause(){
    printf("Digite enter para continuar");
    char a = 0;
    while(a != '\n')
    {
        getchar();
        fflush(stdin);
        a = (char)getchar();
    }
    system("clear");
}

void ordenar(struct Jogador *jogador, int nLinhas) {

    /*
     * A função realiza a ordenação "Bubble Sort" dos jogadores lidos no arquivo de recorde de determinada fase.
     * Por meio de ponteiros aos struct Jogador *jogador, é possível ordenar o array, levando em conta quem fez menos movimentos.
     * Não possui retorno, apenas ordena o vetor.
     */

    struct Jogador auxiliar;
    for(int i = 0; i < nLinhas-1; i++)
        for(int j = 0; j < nLinhas-i-1; j++)
            if(jogador[j].moves > jogador[j+1].moves)
            {
                auxiliar = jogador[j];
                jogador[j] = jogador[j+1];
                jogador[j+1] = auxiliar;
            }
}

bool Mallocar2d(int *** matriz, int lin, int col) {
    *matriz = malloc( sizeof(int *) * lin);

    if (*matriz == NULL){
        printf("ERROR: Sem memória!\n");
        return false;
    }

    for (int i=0;i<lin;i++){
        (*matriz)[i] = malloc( sizeof(int) * col);
        if ((*matriz)[i] == NULL){
            printf("ERROR: Sem memória!\n");
            return false;
        }
    }
    printf("Alocado!\n");
    return true;
}

bool geraFases(){
    printf("Geração de fases iniciadas:\n");
    FILE *arq;
    int lin, col;

    //Geração do binário da fase 1

    lin = 11; col = 19;
    int fase1[11][19] = {{0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 1, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 1, 1, 1, 0, 0, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 1, 0, 0, 2, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                         {1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
                         {1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 3, 3, 1},
                         {1, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 1},
                         {1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 4, 1, 1, 0, 0, 3, 3, 1},
                         {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                         {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}};

    if((arq = fopen("./Mapas/Fase1/level","w"))!=NULL) {
        fwrite(&lin, sizeof(int), 1, arq);
        fwrite(&col, sizeof(int), 1, arq);
        fwrite(fase1, sizeof(int), lin * col, arq);
        fclose(arq);
        printf("Fase 1 gerada! \n");
    }
    else {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }
    if((arq = fopen("./Mapas/Fase1/Recordes","w")) == NULL)
    {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }
    fclose(arq);
    //Geração do binário  da fase 2

    lin = 10; col = 14;
    int fase2[10][14] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
                         {1, 3, 3, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1},
                         {1, 3, 3, 0, 0, 1, 0, 2, 0, 0, 2, 0, 0, 1},
                         {1, 3, 3, 0, 0, 1, 2, 1, 1, 1, 1, 0, 0, 1},
                         {1, 3, 3, 0, 0, 0, 0, 4, 0, 1, 1, 0, 0, 1},
                         {1, 3, 3, 0, 0, 1, 0, 1, 0, 0, 2, 0, 1, 1},
                         {1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 0, 2, 0, 1},
                         {0, 0, 1, 0, 2, 0, 0, 2, 0, 2, 0, 2, 0, 1},
                         {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
                         {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

    if((arq = fopen("./Mapas/Fase2/level","w"))!=NULL) {
        fwrite(&lin, sizeof(int), 1, arq);
        fwrite(&col, sizeof(int), 1, arq);
        fwrite(fase2, sizeof(int), lin * col, arq);
        fclose(arq);
        printf("Fase 2 gerada! \n");
    }
    else {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }
    if((arq = fopen("./Mapas/Fase2/Recordes","w")) == NULL)
    {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }
    fclose(arq);
    //Geração do binário da fase 3

    lin = 10; col = 17;
    int fase3[10][17] = {{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 4, 1, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 1, 2, 0, 1, 1, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 0, 2, 1, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 0, 2, 0, 1, 0, 0},
                         {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2, 0, 1, 0, 1, 1, 1},
                         {1, 3, 3, 3, 3, 0, 0, 1, 1, 0, 2, 0, 0, 2, 0, 0, 1},
                         {1, 1, 3, 3, 3, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 1},
                         {1, 3, 3, 3, 3, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                         {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    if((arq = fopen("./Mapas/Fase3/level","w"))!=NULL) {
        fwrite(&lin, sizeof(int), 1, arq);
        fwrite(&col, sizeof(int), 1, arq);
        fwrite(fase3, sizeof(int), lin * col, arq);
        fclose(arq);
        printf("Fase 3 gerada! \n");
    }
    else {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }
    if((arq = fopen("./Mapas/Fase3/Recordes","w")) == NULL)
    {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }
    fclose(arq);

    //Geração do binário da fase 4

    lin = 14; col = 19;

    int fase4[14][19] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
                         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 3, 3, 3, 3, 1},
                         {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 3, 3, 3, 3, 1},
                         {1, 0, 0, 0, 0, 1, 0, 0, 2, 0, 2, 0, 0, 0, 3, 3, 3, 3, 1},
                         {1, 0, 2, 2, 2, 1, 2, 0, 0, 2, 0, 1, 0, 0, 3, 3, 3, 3, 1},
                         {1, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 3, 3, 3, 3, 1},
                         {1, 0, 2, 2, 0, 1, 2, 0, 2, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1},
                         {1, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                         {1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                         {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                         {1, 0, 0, 2, 2, 1, 2, 2, 0, 0, 4, 1, 0, 0, 0, 0, 0, 0, 0},
                         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                         {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}};

    if((arq = fopen("./Mapas/Fase4/level","w"))!=NULL) {
        fwrite(&lin, sizeof(int), 1, arq);
        fwrite(&col, sizeof(int), 1, arq);
        fwrite(fase4, sizeof(int), lin * col, arq);
        fclose(arq);
        printf("Fase 4 gerada! \n");
    }
    else {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }
    if((arq = fopen("./Mapas/Fase4/Recordes","w")) == NULL)
    {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }
    fclose(arq);

    //Geração do binário da fase 5

    lin = 13; col = 17;

    int fase5[13][17] = {   {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1},
                            {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 2, 1, 1, 0, 0, 1},
                            {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 1},
                            {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1},
                            {1, 3, 3, 3, 3, 0, 0, 1, 1, 0, 2, 0, 0, 2, 1, 1, 1},
                            {1, 3, 3, 3, 3, 0, 0, 0, 0, 2, 0, 2, 2, 0, 1, 1, 0},
                            {1, 3, 3, 3, 3, 0, 0, 1, 1, 2, 0, 0, 2, 0, 4, 1, 0},
                            {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2, 0, 0, 1, 1, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 2, 0, 0, 1, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0}};

    if((arq = fopen("./Mapas/Fase5/level","w"))!=NULL) {
        fwrite(&lin, sizeof(int), 1, arq);
        fwrite(&col, sizeof(int), 1, arq);
        fwrite(fase5, sizeof(int), lin * col, arq);
        fclose(arq);
        printf("Fase 5 gerada! \n");
    }
    else {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }
    if((arq = fopen("./Mapas/Fase5/Recordes","w")) == NULL)
    {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }
    fclose(arq);

    //Geração do binário da fase 6

    lin = 11; col = 12;

    int fase6[11][12] = {{1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0},
                         {1, 3, 3, 0, 0, 1, 0, 1, 1, 4, 1, 1},
                         {1, 3, 3, 0, 0, 1, 1, 1, 0, 0, 0, 1},
                         {1, 3, 3, 0, 0, 0, 0, 0, 2, 2, 0, 1},
                         {1, 3, 3, 0, 0, 1, 0, 1, 0, 2, 0, 1},
                         {1, 3, 3, 1, 1, 1, 0, 1, 0, 2, 0, 1},
                         {1, 1, 1, 1, 0, 2, 0, 1, 2, 0, 0, 1},
                         {0, 0, 0, 1, 0, 0, 2, 1, 0, 2, 0, 1},
                         {0, 0, 0, 1, 0, 2, 0, 0, 2, 0, 0, 1},
                         {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1},
                         {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

    if((arq = fopen("./Mapas/Fase6/level","w"))!=NULL) {
        fwrite(&lin, sizeof(int), 1, arq);
        fwrite(&col, sizeof(int), 1, arq);
        fwrite(fase6, sizeof(int), lin * col, arq);
        fclose(arq);
        printf("Fase 6 gerada! \n");
    }
    else {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }

    if((arq = fopen("./Mapas/Fase6/Recordes","w")) == NULL)
    {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }
    fclose(arq);

    //Geração do binário da fase 7

    lin = 12; col = 13;

    int fase7[12][13] = {{0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0},
                         {0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1},
                         {1, 1, 0, 1, 0, 4, 1, 1, 0, 2, 2, 0, 1},
                         {1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1},
                         {1, 0, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0, 1},
                         {1, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1},
                         {1, 0, 2, 0, 0, 1, 1, 1, 0, 3, 3, 1, 0},
                         {1, 0, 2, 0, 2, 0, 2, 0, 3, 3, 3, 1, 0},
                         {1, 0, 0, 0, 0, 1, 1, 1, 3, 3, 3, 1, 0},
                         {1, 0, 2, 2, 0, 1, 0, 1, 3, 3, 3, 1, 0},
                         {1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0},
                         {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    if((arq = fopen("./Mapas/Fase7/level","w"))!=NULL) {
        fwrite(&lin, sizeof(int), 1, arq);
        fwrite(&col, sizeof(int), 1, arq);
        fwrite(fase7, sizeof(int), lin * col, arq);
        fclose(arq);
        printf("Fase 7 gerada! \n");
    }
    else {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }
    if((arq = fopen("./Mapas/Fase7/Recordes","w")) == NULL)
    {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }
    fclose(arq);

    //Geração do binário da fase 8

    lin = 16; col = 16;

    int fase8[16][16] = {{0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                         {0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 2, 0, 1},
                         {0, 0, 1, 0, 2, 1, 0, 2, 0, 1, 0, 0, 2, 0, 0, 1},
                         {0, 0, 1, 0, 0, 2, 0, 2, 0, 0, 1, 0, 0, 0, 0, 1},
                         {1, 1, 1, 0, 2, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1},
                         {1, 4, 1, 2, 0, 2, 0, 2, 0, 0, 1, 1, 0, 0, 0, 1},
                         {1, 0, 0, 0, 0, 2, 0, 1, 2, 1, 0, 0, 0, 1, 0, 1},
                         {1, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 2, 0, 2, 0, 1},
                         {1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                         {0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
                         {0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
                         {0, 0, 1, 3, 3, 3, 3, 3, 3, 1, 0, 0, 0, 0, 0, 0},
                         {0, 0, 1, 3, 3, 3, 3, 3, 3, 1, 0, 0, 0, 0, 0, 0},
                         {0, 0, 1, 3, 3, 3, 3, 3, 3, 1, 0, 0, 0, 0, 0, 0},
                         {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}};


    if((arq = fopen("./Mapas/Fase8/level","w"))!=NULL) {
        fwrite(&lin, sizeof(int), 1, arq);
        fwrite(&col, sizeof(int), 1, arq);
        fwrite(fase8, sizeof(int), lin * col, arq);
        fclose(arq);
        printf("Fase 8 gerada! \n");
    }
    else {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }
    if((arq = fopen("./Mapas/Fase8/Recordes","w")) == NULL)
    {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }
    fclose(arq);

    //Geração do binário da fase 9

    lin = 14; col = 17;
    int fase9[14][17] = {   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 3, 3, 3, 1},
                            {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 3, 3, 3, 1},
                            {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 3, 0, 3, 1},
                            {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 3, 3, 3, 1},
                            {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 3, 3, 3, 1},
                            {0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
                            {0, 0, 0, 0, 0, 1, 0, 2, 2, 2, 0, 1, 1, 0, 0, 0, 0},
                            {0, 1, 1, 1, 1, 1, 0, 0, 2, 0, 2, 0, 1, 1, 1, 1, 1},
                            {1, 1, 0, 0, 0, 1, 2, 0, 2, 0, 0, 0, 1, 0, 0, 0, 1},
                            {1, 4, 0, 2, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 2, 0, 1},
                            {1, 1, 1, 1, 1, 1, 0, 2, 2, 0, 2, 0, 1, 1, 1, 1, 1},
                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0}};

    if((arq = fopen("./Mapas/Fase9/level","w"))!=NULL) {
        fwrite(&lin, sizeof(int), 1, arq);
        fwrite(&col, sizeof(int), 1, arq);
        fwrite(fase9, sizeof(int), lin * col, arq);
        fclose(arq);
        printf("Fase 9 gerada! \n");
    }
    else {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }
    if((arq = fopen("./Mapas/Fase9/Recordes","w")) == NULL)
    {
        printf("Não foi possível gerar os níveis genéricos! \n");
        return false;
    }
    fclose(arq);

    //Fim da geração.
    //Se chegou até aqui,significa que não entrou em nenhuma falha na geração, logo:
    printf("*** Todas as fases foram geradas! ***\n");
    return true;
}

bool verificaArquivos(){
    /*
     * Essa função verifica se os arquivos e pastas necessárias para o jogo estão íntegros.
     */
    printf("Verificando arquivos do jogo:\n");
    struct stat st = {0};
    if(
            (stat("./Mapas", &st) == -1)||
            (stat("./Mapas/Fase1/level", &st) == -1)||
            (stat("./Mapas/Fase2/level", &st) == -1)||
            (stat("./Mapas/Fase3/level", &st) == -1)||
            (stat("./Mapas/Fase4/level", &st) == -1)||
            (stat("./Mapas/Fase5/level", &st) == -1)||
            (stat("./Mapas/Fase6/level", &st) == -1)||
            (stat("./Mapas/Fase7/level", &st) == -1)||
            (stat("./Mapas/Fase8/level", &st) == -1)||
            (stat("./Mapas/Fase9/level", &st) == -1))
    {
        mkdir("./Mapas", 0777);
        mkdir("./Mapas/Fase1", 0777);
        mkdir("./Mapas/Fase2", 0777);
        mkdir("./Mapas/Fase3", 0777);
        mkdir("./Mapas/Fase4", 0777);
        mkdir("./Mapas/Fase5", 0777);
        mkdir("./Mapas/Fase6", 0777);
        mkdir("./Mapas/Fase7", 0777);
        mkdir("./Mapas/Fase8", 0777);
        mkdir("./Mapas/Fase9", 0777);
        return geraFases();
    }
    else return true;
}

void verRecordes(){
    int c;
    int nLinhas;
    char fase[13];
    char caminhoFase[31];
    int i = 0;
    FILE *arq;
    system("clear");
    printf("\n██████╗  █████╗ ███╗   ██╗██╗  ██╗██╗███╗   ██╗ ██████╗ \n"
           "██╔══██╗██╔══██╗████╗  ██║██║ ██╔╝██║████╗  ██║██╔════╝ \n"
           "██████╔╝███████║██╔██╗ ██║█████╔╝ ██║██╔██╗ ██║██║  ███╗\n"
           "██╔══██╗██╔══██║██║╚██╗██║██╔═██╗ ██║██║╚██╗██║██║   ██║\n"
           "██║  ██║██║  ██║██║ ╚████║██║  ██╗██║██║ ╚████║╚██████╔╝\n"
           "╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝ ╚═════╝ \n\n");
    printf("Digite o número da fase correspondente para ver os recordes.\n1 a 9 para as fases originais ou 0 para os recordes da fase carregada.\n");
    scanf("%d", &c);

    if(c == 0) strcpy(fase,nomeFase);

    else sprintf(fase,"Fase%d",c);

    sprintf(caminhoFase,"./Mapas/%s/Recordes",fase);

    if((arq =fopen(caminhoFase,"r"))==NULL)
    {
        printf("Não foi possível abrir o arquivo\n");
        pause();
        return;
    }
    nLinhas = contaLinhas(caminhoFase);
    if(nLinhas == 0)
    {
        printf("Ainda não existem dados para esta fase, seja o primeiro!\n");
        pause();
        return;
    }
    struct Jogador jogador[nLinhas];
    while(!feof(arq))
    {
        fscanf(arq,"%s # %d\n",jogador[i].nome,&jogador[i].moves);
        i++;
    }
    fclose(arq);

    ordenar(jogador, nLinhas);

    printf("\nFase => %s\nMelhores jogadores:\n\n",fase);
    for(int i = 0; i < nLinhas;i++)
        printf("%dº -> %s - %d movimentos\n",i+1, jogador[i].nome, jogador[i].moves);
    printf("\n");
    pause();
}

int exibirMenu() {

    char fase;
    printf("\n \n"
           "███████╗ ██████╗ ██╗  ██╗ ██████╗ ██████╗  █████╗ ███╗   ██╗\n"
           "██╔════╝██╔═══██╗██║ ██╔╝██╔═══██╗██╔══██╗██╔══██╗████╗  ██║\n"
           "███████╗██║   ██║█████╔╝ ██║   ██║██████╔╝███████║██╔██╗ ██║\n"
           "╚════██║██║   ██║██╔═██╗ ██║   ██║██╔══██╗██╔══██║██║╚██╗██║\n"
           "███████║╚██████╔╝██║  ██╗╚██████╔╝██████╔╝██║  ██║██║ ╚████║\n"
           "╚══════╝ ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═══╝\n \n"
           "Feito por Marcos Paulo Paolino e Marco Cabral Campelo"
           "\n Selecione a fase:                            \n"
           "1 -> Fase 1     4 -> Fase 4     7 -> Fase 7     0 -> Fase Customizada\n"
           "2 -> Fase 2     5 -> Fase 5     8 -> Fase 8     H -> Ajuda\n"
           "3 -> Fase 3     6 -> Fase 6     9 -> Fase 9     R -> Ver Recordes\n\nQ -> Sair\n\n");

    scanf("%c", &fase);

    if(isdigit(fase)) return (fase-'0');

    else if (toupper(fase) == 'Q') return -1;

    else if(toupper(fase) == 'H') return -2;

    else if(toupper(fase) == 'R') return -3;

    else {
        system("clear");
        return exibirMenu();
    }
}

int vitoria(char *enderecoFase) {
    system("clear");
    printf("██████╗  █████╗ ██████╗  █████╗ ██████╗ ███████╗███╗   ██╗███████╗██╗\n██╔══██╗██╔══██╗██╔══██╗██╔══██╗██╔══██╗██╔════╝████╗  ██║██╔════╝██║\n██████╔╝███████║██████╔╝███████║██████╔╝█████╗  ██╔██╗ ██║███████╗██║\n██╔═══╝ ██╔══██║██╔══██╗██╔══██║██╔══██╗██╔══╝  ██║╚██╗██║╚════██║╚═╝\n██║     ██║  ██║██║  ██║██║  ██║██████╔╝███████╗██║ ╚████║███████║██╗\n╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═══╝╚══════╝╚═╝");

    printf("\n\n Você venceu com um total de %d movimentos!\n\n", moves);

    char nome[21];

    printf("\nDigite seu nome para registrá-lo, utilize apenas letras do alfabeto e números:\n");

    scanf("%s", nome);

    FILE *arq;
    if((arq = fopen(strcat(enderecoFase,"Recordes"),"a")) == NULL)
    {
        printf("Não foi possivel fazer o registro.\n");
        pause();
        return 0;
    }
    fprintf(arq,"%s # %d\n",nome,moves);
    fclose(arq);
    printf("Sua pontuação foi registrada!\n");
    pause();
    return 0;
}

bool verificaVitoria(int *movel, int *original, int lin, int col) {
    int nCaixas = 0, nCaixasCorretas = 0;

    for(int i  = 0; i < col; i++) {
        for(int j = 0; j < lin; j++) {

            if (original[i * lin + j] == 3) nCaixas++;
        }
    }
    for(int i  = 0; i < col; i++) {
        for (int j = 0; j < lin; j++) {
            if (original[i * lin + j] == 3 && movel[i * lin + j] == 2)
                nCaixasCorretas++;
            else if(original[i * lin + j] == 3 && movel[i * lin + j] != 2)
                return false;
        }
    }
    if(nCaixasCorretas == nCaixas)
        return true;
    return false;
}

void exibirPosicao(int *movel, int lin, int col){
    for(int i = 0; i < lin; i++)
        for (int j = 0; j < col; j++) {
            if(movel[i * col + j] == 4)
            {
                printf("\nx: %d y: %d \n",i,j);
                printf("Movimentos = %d \n", moves);
            }
        }
}

bool verificaMovimento(char direcao, int *movel, int lin, int col) {
    switch(direcao)
    {
        case 'w':
            for (int i = 0; i < lin; i++)
                for(int j = 0; j < col; j++)
                {
                    if(movel[i*col+j] == 4)
                    {
                        if(movel[i * col + j - col] == 0 || movel[i * col + j - col] ==  3) return true;

                        else if(movel[i * col + j - col] == 1) return false;

                        else if(movel[i * col + j - col] == 2) {

                            if(movel[i * col + j - (2*col)] != 0 && movel[i * col + j - (2*col)] != 3) return false;

                            else {
                                movel[i * col + j - col] = 0;
                                movel[i * col + j - (2 * col)] = 2;
                                return true;
                            }
                        }
                    }
                }
            break;
        case 'a':
            for (int i = 0; i < lin; i++)
                for(int j = 0; j < col; j++)
                {
                    if(movel[i*col+j] == 4)
                    {
                        if(movel[i * col + j -1] == 0 || movel[i * col + j -1] ==  3) return true;

                        else if(movel[i * col + j -1] == 1) return false;

                        else if(movel[i * col + j -1] == 2) {

                            if(movel[i * col + j - 2] != 0 && movel[i * col + j - 2] != 3) return false;

                            else {
                                movel[i * col + j - 2] = 0;
                                movel[i * col + j - 2] = 2;
                                return true;
                            }
                        }
                    }
                }
            break;
        case 's':
            for (int i = 0; i < lin; i++)
                for(int j = 0; j < col; j++)
                {
                    if(movel[i*col+j] == 4)
                    {
                        if(movel[i * col + j + col] == 0 || movel[i * col + j + col] ==  3) return true;

                        else if(movel[i * col + j + col] == 1) return false;

                        else if(movel[i * col + j + col] == 2) {

                            if (movel[i * col + j + 2*col] != 0 && movel[i * col + j + 2*col] != 3) return false;

                            else {
                                movel[i * col + j + col] = 0;
                                movel[i * col + j + (2 * col)] = 2;
                                return true;
                            }
                        }
                    }
                }
            break;
        case 'd':
            for (int i = 0; i < lin; i++)
                for(int j = 0; j < col; j++)
                {
                    if(movel[i*col+j] == 4)
                    {
                        if(movel[i * col + j + 1] == 0 || movel[i * col + j + 1] ==  3) return true;

                        else if(movel[i * col + j + 1] == 1) return false;

                        else if(movel[i * col + j + 1] == 2) {

                            if(movel[i * col + j + 2] != 0 && movel[i * col + j + 2] != 3) return false;

                            else {
                                movel[i * col + j + 1] = 0;
                                movel[i * col + j + 2] = 2;
                                return true;
                            }
                        }
                    }
                }
            break;
    }
    return false;
}

int movimentarPersonagem(int *movel, int lin, int col) {
    switch(getchar()) {
        case 'w':
            for(int i = 0; i < lin; i++)
                for (int j = 0; j < col; j++) {
                    if(movel[i*col + j] == 4)
                    {
                        if(verificaMovimento('w', movel, lin, col) == true) {
                            movel[i * col + j - col] = 4;
                            movel[i * col + j] = 0;
                            moves++;
                            return true;
                        }
                    }
                }
            break;
        case 'a':
            for(int i = 0; i < lin; i++)
                for (int j = 0; j < col; j++) {
                    if(movel[i*col + j] == 4)
                    {
                        if(verificaMovimento('a', movel, lin, col) == true) {
                            movel[i * col + j - 1] = 4;
                            movel[i * col + j] = 0;
                            moves++;
                            return true;
                        }
                    }
                }
            break;
        case 's':
            for(int i = 0; i < lin; i++)
                for (int j = 0; j < col; j++) {
                    if(movel[i*col + j]== 4)
                    {
                        if(verificaMovimento('s', movel, lin, col) == true) {
                            movel[i * col + j + col] = 4;
                            movel[i * col + j] = 0;
                            moves++;
                            return true;
                        }
                    }
                }
            break;
        case 'd':
            for(int i = 0; i < lin; i++)
                for (int j = 0; j < col; j++) {
                    if(movel[i*col + j] == 4)
                    {
                        if(verificaMovimento('d', movel, lin, col) == true) {
                            movel[i * col + j + 1] = 4;
                            movel[i * col + j] = 0;
                            moves++;
                            return true;
                        }
                    }
                }
            break;
        case 'q':
            return -1;
        default:
            return 0;
    }
    return 0;
}

void mostrarMapa(int *movel, int *original, int lin, int col) {
    system("clear");
    for(int i = 0; i < lin; i++)
        for(int j = 0; j < col; j++)
        {
            if(original[i*col + j] == 3 && movel [i*col+j] == 0)
            {
                movel[i * col + j] = original[i*col + j];
            }
        }
    for (int i = 0; i < lin; i++) {
        for (int j = 0; j < col; j++) {
            switch (movel[i*col+j]) {

                case 0:
                    printf(" ");
                    break;
                case 1:
                    printf("█");
                    break;
                case 2:
                    printf("#");
                    break;
                case 3:
                    printf("x");
                    break;
                case 4:
                    printf("@");
                    break;
                default:
                    printf("?");
            }
        }
        printf("\n");
    }
}

int jogo(int fase) {
    system("clear");
    moves = 0;
    char caminhoFase[31];
    char caminhoArquivo[31];
    int **original;
    int **movel;
    int lin;
    int col;

    if(fase == 0) {
        sprintf(caminhoFase, "./Mapas/%s/", nomeFase);
        sprintf(caminhoArquivo, "./Mapas/%s/level", nomeFase);
    }
    else {
        sprintf(caminhoFase, "./Mapas/Fase%d/", fase);
        sprintf(caminhoArquivo, "./Mapas/Fase%d/level", fase);
    }

    FILE *arq;

    if((arq = fopen(caminhoArquivo,"r")) == NULL)
    {
        printf("Não foi possível inicializar a fase.\n");
        pause();
        return 0;
    }

    fread(&lin,sizeof(int),1,arq);
    fclose(arq);
    printf("%d\n",lin);

    if((arq = fopen(caminhoArquivo,"r")) == NULL)
    {
        printf("Não foi possível inicializar a fase.\n");
        pause();
        return 0;
    }

    fseek(arq,sizeof(int),SEEK_CUR);
    fread(&col,sizeof(int),1,arq);
    fclose(arq);
    printf("%d\n",col);

    Mallocar2d(&original,lin,col);
    Mallocar2d(&movel,lin,col);

    if((arq = fopen(caminhoArquivo,"r")) == NULL)
    {
        printf("Não foi possível inicializar a fase.");
        pause();
        return 0;
    }
    fseek(arq, sizeof(int)*2,SEEK_CUR);
    fread(original,sizeof(int),lin*col,arq);
    fclose(arq);

    if((arq = fopen(caminhoArquivo,"r")) == NULL)
    {
        printf("Não foi possível inicializar a fase.");
        pause();
        return 0;
    }
    fseek(arq, sizeof(int)*2,SEEK_CUR);
    fread(movel,sizeof(int),lin*col,arq);
    fclose(arq);

    bool rodando = true;
    /*Aqui é o loop  principal do  jogo*/
    while (rodando) {
        if (verificaVitoria((int *) movel, (int *) original, lin, col)) {
            free(original);
            free(movel);
            return vitoria(caminhoFase);
        }
        mostrarMapa((int *) movel, (int *) original, lin, col);
        exibirPosicao((int *) movel, lin, col);
        if (movimentarPersonagem((int *) movel, lin, col) == -1) {
            rodando = false;
        }
    }
    free(original);
    free(movel);
    return 0;
}

int main(int argc, char * argv[]) {
    if(verificaArquivos() == false) return 0;
    if(argc == 2){
        strcpy(nomeFase,argv[1]);
        printf("Fase carregada :%s",nomeFase);
    }
    system("clear");
    while(true) {
        int level = exibirMenu();
        if (level != -1 &&  level != -2 && level != -3) {
            if (jogo(level) == -1)
                return 0;
        }
        else if(level == -1)
        {
            system("clear");
            return 0;
            /*
             * Limpa a tela e sai do jogo, opção q
             */
        }
        else  if(level == -2)
        {
            //Menu help
        }
        else if(level == -3)
        {
            /*
             * Menu RANKING
             */
            verRecordes();
        }
    }
}