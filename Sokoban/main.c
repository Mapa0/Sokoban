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

int exibirMenu() {

    char fase;
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

    fase = getchar();

    if(fase == '0' || fase == '1' || fase == '2' || fase == '3')
        return fase;

    else if (fase == 'q')
        return -1;

    else {
        system("clear");
        printf("Opção inválida, digite novamente! ");
        return exibirMenu();
    }
}

int mostrarMapa(int movel[6][6], const int original[6][6]) {
    system("clear");
    int level[6][6];
    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 6; j++)
        {
            if(original[i][j] == 3 && movel [i][j] == 0)
            {
                level[i][j] = original[i][j];
            }
            else level[i][j] = movel[i][j];
        }
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            switch (level[i][j]) {

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

int jogo(char fase){
    system("clear");
    if (fase == 0)
    {
        /*fase customizada!*/
    }
    else
    {
        char movimento;
        bool cima,baixo,direita,esquerda,rodando=true;
        int movel[6][6] = {{1, 1, 1, 1, 1, 1},
                           {1, 0, 0, 0, 0, 1},
                           {1, 0, 0, 3, 0, 1},
                           {1, 0, 2, 0, 0, 1},
                           {1, 0, 0, 4, 0, 1},
                           {1, 1, 1, 1, 1, 1}};

        const int original[6][6] = {{1, 1, 1, 1, 1, 1},
                                    {1, 0, 0, 0, 0, 1},
                                    {1, 0, 0, 3, 0, 1},
                                    {1, 0, 2, 0, 0, 1},
                                    {1, 0, 0, 4, 0, 1},
                                    {1, 1, 1, 1, 1, 1}};
        while(rodando)
        {
        mostrarMapa(movel,original);
        switch(movimento = getchar()) {
            case 'w':
                for(int i = 0; i < 6; i++)
                    for (int j = 0; j < 6; j++) {
                        if(movel[i][j] == 4)
                        {
                            movel[i-1][j] = 4;
                            movel[i][j] = 0;
                        }
                    }
                break;
            case 'a':
                for(int i = 0; i < 6; i++)
                    for (int j = 0; j < 6; j++) {
                        if(movel[i][j] == 4)
                        {
                            movel[i][j-1] = 4;
                            movel[i][j] = 0;
                        }
                    }
                break;
            case 's':
                for(int i = 0; i < 6; i++)
                    for (int j = 0; j < 6; j++) {
                        if(movel[i][j] == 4)
                        {
                            movel[i+1][j] = 4;
                            movel[i][j] = 0;
                        }
                    }
                break;
            case 'd':
                for(int i = 0; i < 6; i++)
                    for (int j = 0; j < 6; j++) {
                        if(movel[i][j] == 4)
                        {
                            movel[i][j+1] = 4;
                            movel[i][j] = 0;
                        }
                    }
                break;
            case 'q':
                rodando = false;
                break;
            default:
                break;
            }
        }

    }
    return 0;
}

int main() {
    system("clear");
    int level = exibirMenu();
    if(level != -1)
        jogo(level);
    else{
        system("clear");
        return 0;
    }
    return 0;
}
