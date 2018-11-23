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
/*
 * Esse typedef bool, foi feito para que exista um tipo bool.
 * Preferi trabalhar com false e true no lugar de 0 e 1, por questões de organização.
 */


int moves;
/*
 * Variável de escopo global.
 * Armazena os movimentos realizados pelo jogador.
 */

char nomeFase[13];
/*
 * Variável de escopo global.
 * Armazena o nome da fase customizada, opcionalmente passada como parâmetro ao iniciar o programa.
 */



struct Jogador{
    /*
     * Essa struct Jogador é usada para auxiliar no momento da leitura do Ranking.
     * Armazena o nome do jogador e seus movimentos.
     */
    int moves;
    char nome[21];
};

int contaLinhas(char *nome){
    /*
     * Esta função conta e retorna o número de linhas em um dado arquivo de texto.
     * Conta quantos '\n' existem. Cada '\n' seria uma nova linha.
     * A cadeia de caracteres nome, é o nome/caminho para determinado arquivo.
     */
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
    /*
     * Esta função foi criada no intuíto de permitir a leitura de informações importantes pelo usuário.
     * Ela "limpa" a entrada do programa, stdin, com o comando fflush e espera pela entrada do ENTER pelo usuário.
     * Após isso, limpa o terminal (linux).
     */
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
    /*
     * Como a mecânica de Sokoban desenvolvida aqui, envolvem inúmeras manipulações de matrizes e vetores, foi criada a função Mallocar2d.
     * Basicamente, recebe um ponteiro para uma matriz e 2 inteiros, número de linhas e colunas, respectivamente.
     * Após isso, executa a função malloc para alocar "ponteiros int" às linhas.
     * Então, em cada linha, é executado malloc novamente, mas alocando os devidos inteiros em cada coluna, não ponteiros.
     * Retorna false se algum erro ocorrer e verdadeiro se tudo for devidamente alocado.
     */
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
    /*
     * Esta função gera todos os arquivos necessários para as 9 fases genéricas funcionarem sem problemas.
     * Determinados os números de linhas e colunas, a matriz é preenchida. Todo mapa é  uma matriz.
     */
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
    /*
     *A função fopen gera os arquivos "level" e "Recordes" de cada fase.
     *level se trata de um binário que é preenchido aqui.
     * Seu conteúdo é basicamente 2 ints, lin e col, e a matriz[lin][col], também de inteiros.
    */

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

    /*
     * Se todas as gerações forem um sucesso, a função retorna true.
     * Caso tenha dado errado em algum ponto, retorna false.
     */
    printf("*** Todas as fases foram geradas! ***\n");
    return true;
}

bool verificaArquivos(){
    /*
     *Essa função avalia se a função geraFases() deve ser chamada.
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
    /*
     * Verifica a falta do arquivo level de cada fase essencial do jogo.
     * Nesse caso, verifica o "stat" do arquivo(biblioteca <sys/stat.h>).
     * Caso o arquivo esteja ausente, retorna -1, satisfazendo a condição do "if".
     */
    {
        /*
         * Cria todas as pastas necessárias para a execução do jogo.
         */
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
        /*
         * mkdir(*caminho, permissao); é um comando que cria um diretório.
         * caminho é o caminho para o diretório a ser criado.
         * 0777 é o tipo de permissão da pasta. Nesse caso, a permissão de escrita e leitura é global.
         */
        return geraFases();
        /*
         * Retorna verdadeiro ou falso, indicando se foi possível gerar as fases e o jogo está "jogável".
         * Caso não tenha dado nenhum erro na função "geraFases()", retorna true. Caso contrário, false.
         */
    }
    else return true;
    /*
     * Caso não tenha problema, a verificação de arquivos retorna true e o jogo executa normalmente.
     */
}

void verRanking(){
    /*
     * A função verRecordes, exibe um RANKING com todos os dados salvos em uma determinada fase.
     * Esses dados são:
     * Nome do jogador e número de movimentos.
     * A ideia é exibir quais foram os melhores jogadores, quais cumpriram determinada fase com menos movimentos.
     */
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
    /*
     * Como a fase customizada é uma fase carregada na entrada do programa e fica salva na variavel global nomeFase,
     * strcpy copia o conteúdo dela para a variável local fase.
     */

    else sprintf(fase,"Fase%d",c);
    /*
     * As fases genéricas tem um nome padronizado, sendo possível acessá-las apenas variando o inteiro após "Fase".
     */
    sprintf(caminhoFase,"./Mapas/%s/Recordes",fase);
    /*
     * Esse comando determina qual vai ser o caminho para a Fase.
     */

    if((arq =fopen(caminhoFase,"r"))==NULL)
    {
        printf("Não foi possível abrir o arquivo\n");
        pause();
        return;
    }
    nLinhas = contaLinhas(caminhoFase);
    if(nLinhas == 0)
    {
        /*
         * Caso o arquivo de Recordes exista e não possua nenhuma linha, significa que ninguém passou de determinado nível ainda.
         */
        printf("Ainda não existem dados para esta fase, seja o primeiro!\n");
        pause();
        return;
    }
    struct Jogador jogador[nLinhas];
    /*
     * Declarando um vetor de Jogadores, baseado diretamente no número de recordes lidos (nLinhas).
     */
    while(!feof(arq))
    {
        fscanf(arq,"%s # %d\n",jogador[i].nome,&jogador[i].moves);
        i++;
        /*
         * Enquanto o ponteiro não chega ao final do arquivo, ele continua lendo informações do arquivo "Recordes".
         * As informações lidas são guardadas no array de structs Jogador.
         */
    }
    fclose(arq);

    ordenar(jogador, nLinhas);
    printf("\nFase => %s\nMelhores jogadores:\n\n",fase);
    for(int i = 0; i < nLinhas;i++)
        printf("%dº -> %s - %d movimentos\n",i+1, jogador[i].nome, jogador[i].moves);
    printf("\n");
    pause();
    /*
     * Os recordes lidos são ordenados com o chamado da função ordenar.
     * Após isso, os recordes são impressoes em ordem crescente de movimentos.
     * Após a impressão, o programa espera o  usuário ENTER para ter tempo de ler os recordes.
     */
}

int exibirMenu() {
    /*
     * Essa função exibe o  menu principal do jogo.
     * Dependendo do seu retorno, é possível ir para diversas fases ou funções.
     */
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
           "3 -> Fase 3     6 -> Fase 6     9 -> Fase 9     R -> Ver Ranking\n\nQ -> Sair\n\n");

    scanf("%c", &fase);
    if(isdigit(fase)) return (fase-'0');
        /*
         * O programa lê o input do usuário,verifica se determinado input foi, de fato, um dígito.
         * Caso seja um dígito, retorna um inteiro que determina a fase que será jogada.
         */
    else if (toupper(fase) == 'Q') return -1;
    /*
     * Se o usuário digitar q ou Q, retorna -1.
     * Esse valor faz o jogo ser terminado.
     */
    else if(toupper(fase) == 'H') return -2;
    /*
     * Se o  usuário digitar h ou H,  retorna -2.
     * Esse valor faz o programa exibir informações sobre o jogo.
     */
    else if(toupper(fase) == 'R') return -3;
    /*
     * Se o usuário digitar r ou R, retorna -3.
     * Esse valor faz o jogo exibir o menu de Ranking.
     */
    else {
        system("clear");
        return exibirMenu();
        /*
         * Caso não digite nada esperado,
         * retorna a propria função, chamando ela novamente para que receba input correto do usuário.
         */
    }
}

int vitoria(char *enderecoFase) {
    /*
     * Essa função parabeniza o  usuário por ter conseguido vencer o quebra-cabeça.
     * Além disso, coleta e armazena informações no arquivo "Recordes" de determinada fase.
     * Recebe como parâmetro, uma cadeia de caracteres com o endereço da fase.
     */
    char nome[21];
    system("clear");
    printf("██████╗  █████╗ ██████╗  █████╗ ██████╗ ███████╗███╗   ██╗███████╗██╗\n"
           "██╔══██╗██╔══██╗██╔══██╗██╔══██╗██╔══██╗██╔════╝████╗  ██║██╔════╝██║\n"
           "██████╔╝███████║██████╔╝███████║██████╔╝█████╗  ██╔██╗ ██║███████╗██║\n"
           "██╔═══╝ ██╔══██║██╔══██╗██╔══██║██╔══██╗██╔══╝  ██║╚██╗██║╚════██║╚═╝\n"
           "██║     ██║  ██║██║  ██║██║  ██║██████╔╝███████╗██║ ╚████║███████║██╗\n"
           "╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═══╝╚══════╝╚═╝"
           "\n\n Você venceu com um total de %d movimentos!\n\n", moves);

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
    /*
     * Imprime em uma formatação especial para permitir a leitura linha por linhas posteriormente.
     */
    fclose(arq);
    printf("Sua pontuação foi registrada!\n");
    pause();
    return 0;
    /*
     * Lê o nome do usuário e diz à ele se sua pontuação foi ou não registrada no arquivo.
     */
}

bool verificaVitoria(int *movel, int *original, int lin, int col) {
    /*
     * Como o nome indica, essa função verifica se o jogo foi ganho ou não.
     * Para isso, ela deve ser chamada toda vez que um novo movimento é feito.
     * Recebe 2 matrizes, o  número de linhas e colunas.
     * A matriz "móvel", originalmente tem todos os elemetos idênticos à original.
     * A diferença é que a original se mantém constante, essa varia conforme os movimentos do jogador.
     */
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
    if(nCaixasCorretas >= nCaixas)
        return true;
    /*
     * O primeiro loop for, conta quantas caixas existem na matriz original.
     * O segundo, comparando a matriz móvel com a original, verifica quantas caixas nas posições corretas existem.
     * Nesse caso, original[i * lin + j] == 3 && movel[i * lin + j] == 2, onde 3 é a "marcação no chão" e 2 é a "caixa".
     * Se o número de caixas corretas for igual ao de caixas, retorna true (Venceu a fase).
     * Caso contrário, retorna false.
     */
    return false;
}

void exibirPosicao(int *movel, int lin, int col){
    /*
     * Essa função imprime as coordenadas x e y do jogador na matriz e também imprime o número de movimentos realizados até então.
     */
    for(int i = 0; i < lin; i++)
        for (int j = 0; j < col; j++) {
            if(movel[i * col + j] == 4)
            {
                printf("\nx: %d y: %d \n",j,lin - 1- i);
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
    /*
     * Essa função é a base da movimentação do "@" pela matriz.
     * Recebe input do usuário, teclas WASD(4 direções) e Q(sair).
     *
     * Em cada um dos casos uma coisa é comum:
     * Foram utilizados 2 loops for para encontrar o "4"(Personagem) na matriz móvel.
     * A partir dessa posição, mudanças são feitas.
     */
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
                            /*
                             * Chama a função verificaMovimento para determinar se o movimento pretendido é possível.
                             * Caso seja, atualiza a matriz. O 4 agora está "um i" acima, graças à:
                             *                             movel[i * col + j - col] = 4;
                             * Atualiza também o número de movimentos e a posição antiga onde 4 se encontrava passa a ser 0.
                             */
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
                            /*
                             * Chama a função verificaMovimento para determinar se o movimento pretendido é possível.
                             * Caso seja, atualiza a matriz. O 4 agora está "um j" à esquerda, graças à:
                             *                             movel[i * col + j - 1] = 4;
                             * Atualiza também o número de movimentos e a posição antiga onde 4 se encontrava passa a ser 0.
                             */
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
                            /*
                             * Chama a função verificaMovimento para determinar se o movimento pretendido é possível.
                             * Caso seja, atualiza a matriz. O 4 agora está "um i" abaixo, graças à:
                             *                             movel[i * col + j + col] = 4;
                             * Atualiza também o número de movimentos e a posição antiga onde 4 se encontrava passa a ser 0.
                             */
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
                            /*
                             * Chama a função verificaMovimento para determinar se o movimento pretendido é possível.
                             * Caso seja, atualiza a matriz. O 4 agora está "um j" à direita, graças à:
                             *                             movel[i * col + j + 1] = 4;
                             * Atualiza também o número de movimentos e a posição antiga onde 4 se encontrava passa a ser 0.
                             */
                        }
                    }
                }
            break;
        case 'q':
            return -1;
            /*
             * Retorna -1, a fase é abandonada.
             */
        default:
            return 0;
            /*
             * Não move nada.
             */
    }
    return 0;
}

void mostrarMapa(int *movel, int *original, int lin, int col) {
    /*
     * Essa função é uma das principais do jogo inteiro.
     * Ela exibe o mapa, exibe a matriz, não como números, mas converte esses números para caractéres mais "Agradáveis".
     */
    system("clear");
    for(int i = 0; i < lin; i++)
        for(int j = 0; j < col; j++)
        {
            if(original[i*col + j] == 3 && movel [i*col+j] == 0)
            {
                movel[i * col + j] = original[i*col + j];
                /*
                 * Esses "fors" são apenas para garantir que o jogador não irá apagar os "x" que estão no chão quando andar por cima deles.
                 * Sempre que o mapa vai ser mostrado, ele verifica se onde na matriz original é 3, agora é 0.
                 * (O valor pode passar a ser 0 por vários motivos, como o jogador andando  por cima ou uma caixa sendo empurrada.)
                 * Se for, altera o valor da matriz movel para 3 novamente.
                 */
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
    /*
     * Esses fors são para a "Construção da fase". Dependendo do inteiro lido  na matriz, um caracter diferente será impresso na tela.
     * Diferentes valores acarretam diferentes caracteres:
     * Valor 0: " " -> Vazio
     * Valor 1: "█" -> Parede
     * Valor 2: "#" -> Caixa
     * Valor 3: "x" -> Destino da Caixa
     * Valor 4: "@" -> Jogador
     * Valor n: "?" -> Não identificado/Corrompido.
     * Em geral, se ver qualquer "?" em algum mapa, provavelmente significa que algo está errado com o seu binário.
     */
}

int jogo(int fase) {
    /*
     * Essa função jogo, é onde a maioria das coisas acontecem.
     * Onde existe o principal "loop" do jogo.
     */
    system("clear");
    moves = 0;
    /*
     * Zera o valor da variável global moves.
     */
    char caminhoFase[31];
    char caminhoArquivo[31];
    int **original;
    int **movel;
    int lin;
    int col;
    /*
     * Declara 2 matrizes fundamentais: original e movel.
     * original -> Seus valores nunca são alterados.
     * movel -> Nela, todas as alterações acontecem.
     * Em nosso projeto, são necessárias 2 matrizes originalmente idênticas para determinar a vitória do jogo e uma série de outras mecânicas desenvolvidas.
     * Por ser uma matriz "dinâmica" (não sabemos  originalmente seu tamanho), declaramos int lin e int col também, numero de linhas e colunas da matriz.
     */
    if(fase == 0) {
        sprintf(caminhoFase, "./Mapas/%s/", nomeFase);
        sprintf(caminhoArquivo, "./Mapas/%s/level", nomeFase);
        /*
         * Caso a fase optada seja a customizada, caminhoFase e caminhoArquivo são determinados usando a variável global nomeFase.
         */
    }
    else {
        sprintf(caminhoFase, "./Mapas/Fase%d/", fase);
        sprintf(caminhoArquivo, "./Mapas/Fase%d/level", fase);
        /*
         * Caso a fase optada seja uma das 9 originais, caminhoFase e caminhoArquivo são determinados usando o valor do parâmetro fase recebido.
         */
    }

    FILE *arq;
    //Criação  do  ponteiro  do arquivo.

    if((arq = fopen(caminhoArquivo,"r")) == NULL)
    {
        printf("Não foi possível inicializar a fase.\n");
        pause();
        return 0;
    }

    fread(&lin,sizeof(int),1,arq);
    fclose(arq);
    printf("%d\n",lin);
    /*
     * Tenta abrir o arquivo em que o mapa está gravado.
     * Lê e armazena o número de linhas, primeira int do binário, e imprime.
     */
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
    /*
     * Tenta abrir o arquivo em que o mapa está gravado.
     * Usa o comando fseek para avançar 1 int à frente, no arquivo binário.
     * Lê e armazena o número de colunas, segunda int do binário, e imprime.
     */

    Mallocar2d(&original,lin,col);
    Mallocar2d(&movel,lin,col);
    /*
     * Convoca a função Mallocar2d 2 vezes, para alocar a memória das matrizes original e móvel baseada nos valores de lin e col.
     */
    if((arq = fopen(caminhoArquivo,"r")) == NULL)
    {
        printf("Não foi possível inicializar a fase.");
        pause();
        return 0;
    }
    fseek(arq, sizeof(int)*2,SEEK_CUR);
    fread(original,sizeof(int),lin*col,arq);
    fclose(arq);
    /*
     * Tenta abrir o arquivo em que o mapa está gravado.
     * Usa a função fseek para avançar 2 ints à frente no arquivo binário.
     * Lê e armazena toda uma matriz[lin][col] de inteiros, partindo da terceira int (após ler linhas e colunas), e imprime.
     */
    if((arq = fopen(caminhoArquivo,"r")) == NULL)
    {
        printf("Não foi possível inicializar a fase.");
        pause();
        return 0;
    }
    fseek(arq, sizeof(int)*2,SEEK_CUR);
    fread(movel,sizeof(int),lin*col,arq);
    fclose(arq);
    /*
     * Tenta abrir o arquivo em que o mapa está gravado.
     * Usa a função fseek para avançar 2 ints à frente no arquivo binário.
     * Lê e armazena toda uma matriz[lin][col] de inteiros, partindo da terceira int (após ler linhas e colunas), e imprime.
     */

    bool rodando = true;
    /*
     * Aqui é o loop fundamental do jogo.
     */
    while (rodando) {
        if (verificaVitoria((int *) movel, (int *) original, lin, col)) {
            free(original);
            free(movel);
            return vitoria(caminhoFase);
            /*
             * Chama a função verificaVitoria para ver se as condições de vitória foram atingidas.
             * Caso tenha sido, libera a memória alocada em original e móvel com a função free()
             * e convoca a função vitória.
             */
        }
        mostrarMapa((int *) movel, (int *) original, lin, col);
        /*
         * Essencial convocar essa função.
         * Cada vez que o loop acontece, um mapa atualizado é mostrado.
         */
        exibirPosicao((int *) movel, lin, col);
        /*
         * Convoca a função exibirPosicao, para imprimir na tela os movimentos feitos e as coordenadas x e y do personagem.
         */
        if (movimentarPersonagem((int *) movel, lin, col) == -1) {
            rodando = false;
            /*
             * Chama a função movimentarPersonagem após ter chamado todas as outras funções.
             * Caso seu retorno seja -1, o  jogo para de rodar.
             * É importante chamar a função neste ponto para garantir que o que está sendo impresso na tela é fiel ao posicionamento real do personagem.
             */
        }
    }
    free(original);
    free(movel);
    return 0;
    /*
     * Quando chega nesta etapa, o jogo não está mais rodando.
     * Sendo assim, as matrizes são liberadas e voltamos ao menu com o "return 0;".
     */
}

int main(int argc, char * argv[]) {
    if(verificaArquivos() == false) return 0;
    /*
     * Logo ao iniciar o programa, ocorre a verificação dos arquivos, para garantir o funcionamento das fases fundamentais.
     * Caso retorne falso, o programa é terminado.
     */
    if(argc == 2){
        strcpy(nomeFase,argv[1]);
        printf("Fase carregada :%s",nomeFase);
        /*
         * Verifica se a contagem de argumentos inseridos na execução do programa é 2. (Exemplo: "./Sokoban  FaseExtra")
         *                                                                                      Argv0^         Argv1^
         *
         * Caso seja 2, o segundo argumento é armazenado na variável global como nome da fase customizada.
         */
    }
    system("clear");
    while(true) {
        int level = exibirMenu();
        /*
         * Salva o retorno de exibirMenu() em level.
         * level representa a escolha feita no menu principal.
         * Qual fase vai ser jogada ou se deseja ver rankings, por exemplo.
         */
        if (level != -1 &&  level != -2 && level != -3) {
            jogo(level);
            /*
             * Caso o jogador tenha escolhido uma fase numa menu, a informação sobre qual fase foi escolhida é levada ao jogo pela função jogo(level).
             * Caso tenha decidido sair do jogo, obter ajuda ou ver o Ranking, recebeu -1, -2 ou -3 como retorno.
             */
        }
        else if(level == -1)
        {
            system("clear");
            return 0;
            /*
             * Limpa a tela do terminal e sai do jogo, opção Q.
             */
        }
        else  if(level == -2)
        {
            /*
             * Exibe a página de ajudas.
             */
        }
        else if(level == -3)
        {
            /*
             * Exibe o menu de ranking.
             */
            verRanking();
        }
    }
}