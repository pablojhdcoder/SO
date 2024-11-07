/*
* TITLE: Sistemas Operativos
 * SUBTITLE: Práctica 1
 * AUTHOR 1: Pablo Herrero Diaz LOGIN 1: pablo.herrero.diaz
 * AUTHOR 2: Tiago Da Costa Teixeira Veloso E Volta LOGIN 2: tiago.velosoevolta
 * GROUP: 2.3
 * DATE: 25 / 10 / 24
 */
#ifndef HISTORYLIST_H
#define HISTORYLIST_H

#include <stdbool.h>
#include <stdio.h>

#define MAX 4096  //Número máximo de comandos que se pueden almacenar.
#define HNULL (-1)  //Definición de un valor nulo para indicar que el historial está vacío o que no hay comandos disponibles.
#define LENGTH_MAX_H 256  //Longitud máxima permitida para cada comando en el historial.

typedef int tPosH;  //Representa la posición del último comando en el historial.
typedef char tItemH[LENGTH_MAX_H];  //Representa un comando en el historial

//Estructura que representa el historial de comandos
typedef struct {
    tItemH *data[MAX];  //Array de punteros a cadenas que almacena los comandos del historial.
    tPosH lastPos;  //Posición del último comando almacenado en el historial.
} HistoryList;


//Declaraciones de funciones
void createEmptyListH(HistoryList *L);
bool insertCommandH(tItemH *d, HistoryList *L);
tItemH *getItemH(tPosH p, HistoryList *L);
void printListH(HistoryList *L);
void printLastNH (HistoryList *L, const int n);
void CleanListH(HistoryList *L);
tPosH lastH(HistoryList L);

#endif //HISTORYLIST_H
