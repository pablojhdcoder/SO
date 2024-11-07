/*
* TITLE: Sistemas Operativos
 * SUBTITLE: Práctica 1
 * AUTHOR 1: Pablo Herrero Diaz LOGIN 1: pablo.herrero.diaz
 * AUTHOR 2: Tiago Da Costa Teixeira Veloso E Volta LOGIN 2: tiago.velosoevolta
 * GROUP: 2.3
 * DATE: 25 / 10 / 24
 */
#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>

//Implementar como estática de punteros
#define LENGTH_MAX_NAME 15     //Longitud máxima del nombre de un comando
#define LENGTH_MAX_DESCRIPTION 250   //Longitud máxima de la descripción de un comando

#define LENGTH_MAX_LIST 23  //Lista de 23 punteros a structs
#define CNULL (-1)   //Valor nulo para la lista de comandos

typedef int tPosC;     //Definimos tPosC como un alias de tipo int para manejar posiciones en la lista de comandos

//Estructura de un comando con nombre, descripción e ID
typedef struct {
    char name[LENGTH_MAX_NAME];  //Nombre del comando
    char description[LENGTH_MAX_DESCRIPTION];  //Descripción del comando
    int ID;  //ID del comando
}tCommandC;

//Definición de la lista de comandos que contiene punteros a structs tCommandC
typedef struct {
    tCommandC *commands[LENGTH_MAX_LIST];  //Array de punteros a structs tCommandC
    tPosC lastPos;  //Última posición de la lista
}CommandListC;

//Declaraciones de funciones
void createEmptyListC(CommandListC *list);
void printCommandListC(CommandListC list);
void printCommandDescriptionListC(CommandListC list);
void CleanCommandListC(CommandListC *list);
bool insertCommandC(CommandListC *list, const char name[LENGTH_MAX_NAME], const char description[LENGTH_MAX_DESCRIPTION], const int ID);
tPosC FindCommandC(CommandListC *list, const char name[LENGTH_MAX_NAME]);
tCommandC getCommandC(tPosC p, CommandListC L);



#endif //COMMANDLIST_H
