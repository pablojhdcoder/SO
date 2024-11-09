/*
* TITLE: Sistemas Operativos
 * SUBTITLE: Práctica 2
 * AUTHOR 1: Pablo Herrero Diaz LOGIN 1: pablo.herrero.diaz
 * AUTHOR 2: Tiago Da Costa Teixeira Veloso E Volta LOGIN 2: tiago.velosoevolta
 * GROUP: 2.3
 * DATE: 22 / 11 / 24
 */

#ifndef MEMORYBLOCKLIST_H
#define MEMORYBLOCKLIST_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>

#define BNULL NULL
#define LENGTH_MAX_BLOCK 30

typedef enum {
    MALLOC_MEMORY,
    SHARED_MEMORY,
    MAPPED_FILE
}AllocationType;

//Struct para guardar la información requerida sobre la lista de bloques de memoria
typedef struct {
    void *address;                // Dirección del bloque de memoria
    size_t size;                  // Tamaño del bloque de memoria
    char allocationTime[LENGTH_MAX_BLOCK];        // Tiempo en el que se asignó el bloque
    AllocationType type;          // Tipo de asignación

    // Información adicional
    key_t smKey;                 // Clave de memoria, usado para memoria compartida
    char *fileName;               // Nombre del archivo (para archivos mapeados)
    int fileDescriptor;           // Descriptor del archivo (para archivos mapeados)
}tItemB;

typedef struct tNodeB* tPosB;
typedef tPosB MemoryBlockList;
MemoryBlockList memoryList;  // Lista de bloques de memoria asignados

struct tNodeB {
    tItemB data;
    tPosB next;
};


void createEmptyListB(MemoryBlockList *L);
bool isEmptyListB(MemoryBlockList *L);
bool createNode(tPosB *p);
tPosB lastPosB(MemoryBlockList L);
bool insertMemoryBlockB(MemoryBlockList *L, void *address, size_t size, AllocationType type, key_t Key, const char *fileName, int fileDescriptor);
//Mirar lo que puso gpt con la anterior implementación
void removeMemoryBlock(MemoryBlockList *L, size_t size);
void printMemoryBlockList(MemoryBlockList L);
void cleanMemoryBlockList(MemoryBlockList *L);






#endif //MEMORYBLOCKLIST_H
