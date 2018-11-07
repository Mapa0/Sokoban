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

int jogo(char fase){

    switch (fase) {

        case '1': {
            printf("\n Fase 1: \n");
            int level[6][6] = {{1, 1, 1, 1, 1, 0},
                               {1, 4, 0, 0, 1, 1},
                               {1, 3, 2, 2, 0, 1},
                               {1, 0, 0, 1, 0, 1},
                               {1, 0, 0, 0, 0, 0},
                               {1, 1, 1, 1, 1, 1}};

            for(int i = 0; i < 6; i++) {

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
                        case 3:
                            printf("x");
                            break;
                        case 4:
                            printf("@");
                            break;
                        default:
                            printf(" ");

                    }
                }
                printf("\n");
            }
            printf("\n\n\n\n\n\n\n\n\n\n\n");
        }
        case '2': {

            break;
        }
        case '3':
            break;
        case '0':
            break;
    }

    return 0;
}



int menu() {

    while(true) {
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

        fase = getc(stdin);

        if(fase == '0' || fase == '1' || fase == '2' || fase == '3')
            jogo(fase);

        else if (fase == 'q')
            return 0;

        else
        {
            system("clear");
            printf("Opção inválida, digite novamente! ");
        }
    }
}

int main() {
    system("clear");
    menu();
    return 0;
}
