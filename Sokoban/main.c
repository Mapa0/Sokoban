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

typedef enum { false, true } bool;

int moves;

int exibirMenu() {

    int fase;
    printf("\n \n");
    printf("███████╗ ██████╗ ██╗  ██╗ ██████╗ ██████╗  █████╗ ███╗   ██╗\n");
    printf("██╔════╝██╔═══██╗██║ ██╔╝██╔═══██╗██╔══██╗██╔══██╗████╗  ██║\n");
    printf("███████╗██║   ██║█████╔╝ ██║   ██║██████╔╝███████║██╔██╗ ██║\n");
    printf("╚════██║██║   ██║██╔═██╗ ██║   ██║██╔══██╗██╔══██║██║╚██╗██║\n");
    printf("███████║╚██████╔╝██║  ██╗╚██████╔╝██████╔╝██║  ██║██║ ╚████║\n");
    printf("╚══════╝ ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═══╝\n \n");
    printf("Feito por Marcos Paulo Paolino e Marco Cabral Campelo");
    printf("\n Selecione a fase: \n");
    printf("1 -> Fase 1 \n2 -> Fase 2 \n3 -> Fase 3 \n0 -> Externo \nq -> Sair do Jogo\n\n");

    scanf("%d", & fase);

    if(fase == 0 || fase == 1 || fase == 2 || fase == 3)
        return fase;

    else if (fase == 'q')
        return -1;

    else {
        system("clear");
        printf("Opção inválida, digite novamente! ");
        return exibirMenu();
    }
}

int vitoria()
{
    system("clear");
    printf("██████╗  █████╗ ██████╗  █████╗ ██████╗ ███████╗███╗   ██╗███████╗██╗\n██╔══██╗██╔══██╗██╔══██╗██╔══██╗██╔══██╗██╔════╝████╗  ██║██╔════╝██║\n██████╔╝███████║██████╔╝███████║██████╔╝█████╗  ██╔██╗ ██║███████╗██║\n██╔═══╝ ██╔══██║██╔══██╗██╔══██║██╔══██╗██╔══╝  ██║╚██╗██║╚════██║╚═╝\n██║     ██║  ██║██║  ██║██║  ██║██████╔╝███████╗██║ ╚████║███████║██╗\n╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═══╝╚══════╝╚═╝");
    printf("\n\n Você venceu com um total de %d movimentos!\n\n", moves);
    printf("\nDigite seu nome para registrá-lo junto com os movimentos:\n");
    scanf("%c");
    printf("Deseja jogar novamente?");
    if(getchar() == 's' || getchar() == 'n')
        printf("ok");
    return 1;
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
    for(int i = 0; i < col; i++)
        for (int j = 0; j < lin; j++) {
            if(movel[i * lin + j] == 4)
            {
                printf("\nx: %d y: %d \n",i,j);
                printf("Movimentos = %d \n", moves);
            }
        }
}

bool verificaMovimento(char direcao, int * movel, int lin, int col) {
    switch(direcao)
    {
        case 'w':
            for (int i = 0; i < col; i++)
                for(int j = 0; j < lin; j++)
                {
                    if(movel[i*lin+j] == 4)
                    {
                        if(movel[i * lin + j - lin] == 0 || movel[i * lin + j - lin] ==  3) return true;

                        else if(movel[i * lin + j - lin] == 1) return false;

                        else if(movel[i * lin + j - lin] == 2) {

                            if(movel[i * lin + j - (2*lin)] != 0 && movel[i * lin + j - (2*lin)] != 3) return false;

                            else {
                                movel[i * lin + j - lin] = 0;
                                movel[i * lin + j - (2 * lin)] = 2;
                                return true;
                            }
                        }
                    }
                }
            break;
        case 'a':
            for (int i = 0; i < col; i++)
                for(int j = 0; j < lin; j++)
                {
                    if(movel[i*lin+j] == 4)
                    {
                        if(movel[i * lin + j -1] == 0 || movel[i * lin + j -1] ==  3) return true;

                        else if(movel[i * lin + j -1] == 1) return false;

                        else if(movel[i * lin + j -1] == 2) {

                            if(movel[i * lin + j - 2] != 0 && movel[i * lin + j - 2] != 3) return false;

                            else {
                                movel[i * lin + j - 2] = 0;
                                movel[i * lin + j - 2] = 2;
                                return true;
                            }
                        }
                    }
                }
            break;
        case 's':
            for (int i = 0; i < col; i++)
                for(int j = 0; j < lin; j++)
                {
                    if(movel[i*lin+j] == 4)
                    {
                        if(movel[i * lin + j + lin] == 0 || movel[i * lin + j + lin] ==  3) return true;

                        else if(movel[i * lin + j + lin] == 1) return false;

                        else if(movel[i * lin + j + lin] == 2) {

                            if (movel[i * lin + j + 2*lin] != 0 && movel[i * lin + j + 2*lin] != 3) return false;

                            else {
                                movel[i * lin + j + lin] = 0;
                                movel[i * lin + j + (2 * lin)] = 2;
                                return true;
                            }
                        }
                    }
                }
            break;
        case 'd':
            for (int i = 0; i < col; i++)
                for(int j = 0; j < lin; j++)
                {
                    if(movel[i*lin+j] == 4)
                    {
                        if(movel[i * lin + j + 1] == 0 || movel[i * lin + j + 1] ==  3) return true;

                        else if(movel[i * lin + j + 1] == 1) return false;

                        else if(movel[i * lin + j + 1] == 2) {

                            if(movel[i * lin + j + 2] != 0 && movel[i * lin + j + 2] != 3) return false;

                            else {
                                movel[i * lin + j + 1] = 0;
                                movel[i * lin + j + 2] = 2;
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
            for(int i = 0; i < col; i++)
                for (int j = 0; j < lin; j++) {
                    if(movel[i*lin + j] == 4)
                    {
                        if(verificaMovimento('w', movel, lin, col) == true) {
                            movel[i * lin + j - lin] = 4;
                            movel[i * lin + j] = 0;
                            moves++;
                            return true;
                        }
                    }
                }
            break;
        case 'a':
            for(int i = 0; i < col; i++)
                for (int j = 0; j < lin; j++) {
                    if(movel[i*lin + j] == 4)
                    {
                        if(verificaMovimento('a', movel, lin, col) == true) {
                            movel[i * lin + j - 1] = 4;
                            movel[i * lin + j] = 0;
                            moves++;
                            return true;
                        }
                    }
                }
            break;
        case 's':
            for(int i = 0; i < col; i++)
                for (int j = 0; j < lin; j++) {
                    if(movel[i*lin + j]== 4)
                    {
                        if(verificaMovimento('s', movel, lin, col) == true) {
                            movel[i * lin + j + lin] = 4;
                            movel[i * lin + j] = 0;
                            moves++;
                            return true;
                        }
                    }
                }
            break;
        case 'd':
            for(int i = 0; i < col; i++)
                for (int j = 0; j < lin; j++) {
                    if(movel[i*lin + j] == 4)
                    {
                        if(verificaMovimento('d', movel, lin, col) == true) {
                            movel[i * lin + j + 1] = 4;
                            movel[i * lin + j] = 0;
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
    for(int i = 0; i < col; i++)
        for(int j = 0; j < lin; j++)
        {
            if(original[i*lin + j] == 3 && movel [i*lin+j] == 0)
            {
                movel[i * lin + j] = original[i*lin + j];
            }
        }
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < lin; j++) {
            switch (movel[i*lin+j]) {

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

int jogo(int fase){
    system("clear");
    moves = 0;
    if (fase == 0)
    {
        /*fase customizada!*/

    }
    else if (fase == 1)
    {
        /*Declaração de variáveis sobre informações do Mapa e dos controles*/
        int lin = 19, col = 11;
        int original[11][19] = {{0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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

        int movel[11][19] = {{0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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

        bool rodando=true;

        /*Aqui é onde o  jogo roda de fato*/
        while(rodando)
        {
            if(verificaVitoria((int *)movel,(int *)original,lin,col))
            {
                return vitoria();
            }
            mostrarMapa((int *)movel,(int *)original, lin, col);
            exibirPosicao((int *)movel,lin,col);
            if(movimentarPersonagem((int *)movel, lin, col) == -1)
            {
                return -1;
            }
        }

    }
    else if(fase == 2)
    {
        int lin = 10, col = 10;
        int original[10][10] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                {1, 0, 0, 2, 0, 0, 0, 3, 0, 1},
                                {1, 0, 2, 0, 2, 0, 0, 0, 0, 1},
                                {1, 0, 0, 0, 0, 0, 0, 3, 0, 1},
                                {1, 0, 4, 0, 0, 0, 1, 1, 1, 1},
                                {1, 0, 0, 0, 1, 0, 1, 1 ,0, 1},
                                {1, 0, 0, 0, 1, 3, 0, 1 ,0, 0},
                                {1, 0, 0, 0, 1, 0, 0, 1 ,0, 0},
                                {1, 1, 1, 1, 1, 1, 1, 1, 0, 0}};

        int movel[10][10] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                             {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                             {1, 0, 0, 2, 0, 0, 0, 3, 0, 1},
                             {1, 0, 2, 0, 2, 0, 0, 0, 0, 1},
                             {1, 0, 0, 0, 0, 0, 0, 3, 0, 1},
                             {1, 0, 4, 0, 0, 0, 1, 1, 1, 1},
                             {1, 0, 0, 0, 1, 0, 1, 1 ,0, 1},
                             {1, 0, 0, 0, 1, 3, 0, 1 ,0, 0},
                             {1, 0, 0, 0, 1, 0, 0, 1 ,0, 0},
                             {1, 1, 1, 1, 1, 1, 1, 1, 0, 0}};

        bool rodando=true;
        /*Aqui é onde o  jogo roda de fato*/
        while(rodando)
        {
            if(verificaVitoria((int *)movel,(int *)original,lin,col))
            {
                return vitoria();
            }
            mostrarMapa((int *)movel,(int *)original, lin, col);
            exibirPosicao((int *)movel,lin,col);
            if(movimentarPersonagem((int *)movel, lin, col) == -1)
            {
                return -1;
            }
        }
    }
    return 0;
}

int main() {
    system("clear");
    int level = exibirMenu();
    if(level != -1) {
        if (jogo(level) == -1)
            return main();
    }
    else{
        system("clear");
        return 0;
    }
    return 0;
}
