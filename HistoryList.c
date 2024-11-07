/*
 * TITLE: Sistemas Operativos
 * SUBTITLE: Práctica 1
 * AUTHOR 1: Pablo Herrero Diaz LOGIN 1: pablo.herrero.diaz
 * AUTHOR 2: Tiago Da Costa Teixeira Veloso E Volta LOGIN 2: tiago.velosoevolta
 * GROUP: 2.3
 * DATE: 25 / 10 / 24
 */

#include "HistoryList.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


//Función que crea una lista vacía y ademas la inicializa
void createEmptyListH(HistoryList *L) {        //Se pasa la lista por referencia porque se quiere modificar su contenido
    L->lastPos = HNULL;                 //Asignar el valor nulo a la variable lastPos para inicializar la lista vacía
}

//Recupera el contenido de un elemento cualquiera de la lista
tItemH *getItemH(tPosH p, HistoryList *L){
    return L->data[p];                           //Devuelve el elemento dada la tPosHición el la lista
}

//Devuelve la última tPosHición con contenido de la lista
tPosH lastH(HistoryList L){
    return L.lastPos;                                   //Devuelve la última tPosHición con contenido de la lista
}

//Inserta un elemento en la lista antes de la tPosH indicada, si la tPosH es LNULL se añade al final
bool insertCommandH(tItemH *d, HistoryList *L) {      //Se pasa la lista por referencia porque se quiere modificar
    if (L->lastPos == MAX - 1)                      //Si la última tPosHición de la lista es la última tPosHición del array, no se puede insertar el elemento
        return false;
    else {
        L->lastPos++;  //Incrementar la última tPosH de la lista
        L->data[L->lastPos] = malloc(sizeof(tItemH));  //Reservar memoria para el nuevo elemento
        if (L->data[L->lastPos] == NULL) {  //Si no se ha podido reservar memoria, devolver false
            return false;
        }
        strcpy(*L->data[L->lastPos], *d);  //Copiar el contenido del elemento en la última tPosH nde la lista
        return true;
    }
}

//Funcion para imprimir la lista de historial
void printListH(HistoryList *L) {

    if (L->lastPos == -1) {  //La lista está vacía
        printf("La lista de historial está vacía.\n");
        return;
    }

    printf("History List:\n");
    for (int i = 0; i <= L->lastPos; i++) {  //Recorre la lista e imprimir cada elemento
        printf("%d: %s\n", i, *L->data[i]);  //Imprime el índice y el comando correspondiente
    }
}

//Función para imprimir los últimos n comandos del historial
void printLastNH(HistoryList *L, const int n) {

    if (L->lastPos >= n - 1) {  //Verifica si hay suficientes comandos en el historial para imprimir
        printf("Últimos %d comandos:\n", n);  //Imprime un encabezado indicando que se van a mostrar los últimos n comandos

        for (int i = L->lastPos - (n - 1); i <= L->lastPos; i++) {  //Itera desde la posición del último comando menos n hasta la posición del último comando
            printf("%d: %s\n", i, *L->data[i]);  //Imprime el índice y el comando correspondiente
        }
    } else {
        perror("No se han ejecutado comandos suficientes");  //Imprime un mensaje de error si no hay suficientes comandos
    }
}

//Función para limpiar el historial
void CleanListH(HistoryList *L) {

    for (int i = 0; i <= L->lastPos; i++) {  //Itera sobre cada comando almacenado en el historial
        free(L->data[i]);  //Libera la memoria ocupada por cada comando
    }
    L->lastPos = HNULL;  //Reinicia la posición del último comando a un valor nulo
}