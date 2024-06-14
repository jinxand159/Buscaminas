#include <stdio.h>
#include <stdlib.h>
#include <time.h>


const int altura = 4;
const int anchura = 5;
const int bombas = 5;

void rellenar (int tablero[altura][anchura]);
void dibujarTablero(int tablero[altura][anchura], int pantalla[altura][anchura]);
void limpiarPantalla(int pantalla[altura][anchura]);
int descubrirCasilla(int tablero[altura][anchura], int pantalla[altura][anchura], int posX, int posY, int accion);
void rellenarPantalla(int pantalla[altura][anchura]);
int finPartida(int pantalla[altura][anchura]);

int main (){
    /*El tablero será donde el juego se desarrolla para el desarrollador, es decir tiene información
      de donde están las minas, mientras que la pantalla es lo que el usuario tocaría. 0 significa que
      está oculto y 1 que fue desvelado.*/
    int gameOver = 0, posX=0, posY=0, alerta1=0, alerta2=0, accion=0, victoria=0;
    int tablero[altura][anchura];
    int pantallaDebug[altura][anchura];
    int pantalla[altura][anchura];
    system("clear");
    rellenar(tablero);
    rellenarPantalla(pantallaDebug);
    limpiarPantalla(pantalla);

    /*bucle de juego*/
    while(gameOver == 0 && victoria == 0){
        //dibujarTablero(tablero, pantallaDebug);
        //printf("\n\n");
        dibujarTablero(tablero, pantalla);
        do{
            if(alerta1 == 1){
                printf("\n\t\t\t Por favor, introduzca una casilla de 0 a %d y 0 a %d\n", altura-1, anchura-1);
            }
            else if(alerta2 == 1){
                printf("\n\t\t\t La casilla que ha elegido ya ha sido explorada, elija otra.\n");
            }
            alerta1 = 0;
            alerta2 = 0;
            printf("Elegir una casilla, introduzca la posición horizontal: ");
            scanf("%d", &posX);
            printf("Elegir una casilla, introduzca la posición vertical : ");
            scanf("%d", &posY);

            printf("Elegir acción: 0 para explorar y 1 para poner bandera. ");
            scanf("%d", &accion);
            /*Provocar alerta1*/
            if(!(posX<altura && posX >=0 && posY<anchura && posY >=0)){
                alerta1 = 1;
            }
            else if(pantalla[posX][posY] == 1){
                alerta2 = 1;
            }
        }while(alerta1 == 1 || alerta2 == 1);
        gameOver = descubrirCasilla(tablero, pantalla, posX, posY, accion);
        if(gameOver == 0){
            victoria = finPartida(pantalla);
        }
        system("clear");
    }
    printf("\n\n");
    dibujarTablero(tablero, pantalla);
    if(gameOver == 1){
        printf("\n\t\t\t\tFIN HAS PERDIDO\n");
    }
    else{
        printf("\n\t\t\t\tFIN HAS GANADO\n");
    }
   
}

void rellenar(int tablero[altura][anchura]){
    int minas=0, i, j, posH, posV, dx, dy, minasAlrededor;

    // Inicializar tablero
    for(i = 0; i < altura; i++){
        for(j = 0; j < anchura; j++){
            tablero[i][j] = 0;
        }
    }

    srand(time(NULL));
    for(minas=0 ; minas<bombas ; minas++){
        do{
            posH = rand() % altura;
            posV = rand() % anchura;
        } while(tablero[posH][posV] == -1); /*Asegurarse de que no haya mina en la posición elegida*/
        tablero[posH][posV] = -1;
    }

    for(i=0 ; i<altura ; i++){
        for(j=0 ; j<anchura ; j++){
            /*Quitar caso de mina*/
            if(tablero[i][j] != -1){
                minasAlrededor = 0;

                /*Contar número de minas alrededor sin contar la posición misma*/
                for(dx = -1 ; dx<2 ; dx++){
                    for(dy = -1 ; dy<2 ; dy++){
                        if(!(dy==0 && dx==0) && i+dx>=0 && i+dx<altura && 
                                j+dy>=0 && j+dy<anchura && tablero[i+dx][j+dy] == -1){
                            minasAlrededor++;
                        }
                    }
                }
                tablero[i][j] = minasAlrededor;
            }
        }
    }
}

void dibujarTablero(int tablero[altura][anchura], int pantalla[altura][anchura]){
    int i, j, k;
    char pieza = ' ';
    printf("\t\t \t");
    for(k = 0 ; k < anchura ; k++){
        printf("  %d ",k);
    }
    printf("\n");
    for(i = 0; i < altura; i++){
        printf("\t\t%d\t", i);
        for(j = 0; j < anchura; j++){
            pieza = '_';
            /*Si no se ha descubierto, la pieza que se pinta es O, en otro caso es o mina o el número
              asignado o en otro caso vacío*/
            if(pantalla[i][j] == 0){
                pieza = 'O';
            }
            else if(pantalla[i][j] == -1){ /*Caso poner bandera*/
                pieza = 'P';
            }
            else if(tablero[i][j] == -1){
                pieza = 'X';
            }
            else if(tablero[i][j] != 0){
                pieza = 48 + tablero[i][j];
            }
            printf("| %c ", pieza);
            if(j == anchura-1){
                printf("|");
            }
        }
        printf("\n");
    }
}

void limpiarPantalla(int pantalla[altura][anchura]){
    int i, j;

    // Inicializar tablero
    for(i = 0; i < altura; i++){
        for(j = 0; j < anchura; j++){
            pantalla[i][j] = 0;
        }
    }
}

/*
PRE: la posición que se pasa tiene que ser oculta y en rango.
POS: actualiza tanto el tablero como la pantalla dado las posiciones dadas. Devuelve 1 si se ha finalizado
    la partida, 0 en caso contrario.
*/
int descubrirCasilla(int tablero[altura][anchura], int pantalla[altura][anchura], int posX, int posY, int accion){
    int fin = 0, reaccionEnCadena = 1;

    if(accion == 1){
        pantalla[posX][posY] = -1;
        reaccionEnCadena = 0;
        return fin;
    }
    
    pantalla[posX][posY] = 1;
    
    if(tablero[posX][posY] == -1){
        return 1;
    }

    /*Si es una casilla sin número, hay que difundir hasta encontrarnos con casillas con numero.*/
    if(tablero[posX][posY] == 0){
        /*Comprobar que la casilla de la pantalla sea valida y esté oculta antes de explorar por ahí*/
        if(posX+1 < altura &&  pantalla[posX+1][posY] == 0){
            fin = descubrirCasilla(tablero, pantalla, posX+1, posY, 0);
        }
        if(fin == 0 && posX-1 >= 0 && pantalla[posX-1][posY] == 0){
            fin = descubrirCasilla(tablero, pantalla, posX-1, posY, 0);
        }
        if(fin == 0 && posY+1 < anchura &&  pantalla[posX][posY+1] == 0){
            fin = descubrirCasilla(tablero, pantalla, posX, posY+1, 0);
        }
        if(fin == 0 && posY-1 >= 0 && pantalla[posX][posY-1] == 0){
            fin = descubrirCasilla(tablero, pantalla, posX, posY-1, 0);
        }
    }
    return fin;
}

void rellenarPantalla(int pantalla[altura][anchura]){
    int i, j;

    // Inicializar tablero
    for(i = 0; i < altura; i++){
        for(j = 0; j < anchura; j++){
            pantalla[i][j] = 1;
        }
    }
}

/*Mientras hayan casillas exploradas, fin de partida cuando todas las casillas han sido exploradas.*/
int finPartida(int pantalla[altura][anchura]){
    int i=0, j=0, casillaExplorado = 1;

    while(i<altura && casillaExplorado == 1){
        j=0;
        while(j<anchura && casillaExplorado == 1){
            if(pantalla[i][j] == 0){
                casillaExplorado = 0;
            }
            j++;
        }
        i++;
    }
    return casillaExplorado;
}