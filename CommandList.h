#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Implementar como estática de punteros
#define LENGTH_MAX_NAME 30     //Longitud máxima del nombre de un comando
#define LENGTH_MAX_DESCRIPTION 400  //Longitud máxima de la descripción de un comando

#define LENGTH_MAX_LIST 50  //Actualmente hay 49 comandos, entonces 50 nos llega como tamaño
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