//
// Created by pablojhd on 4/11/24.
//

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

typedef struct tNode* tPosB;
typedef tPosB MemoryBlockList;

struct tNode {
    tItemB data;
    tPosB next;
};


void createEmptyListB(MemoryBlockList *L);
bool isEmptyListB(MemoryBlockList *L);
tPosB lastPosB(MemoryBlockList L);
bool insertMemoryBlockB(MemoryBlockList *L, void *address, size_t size, AllocationType type, key_t Key, const char *fileName, int fileDescriptor);
//Mirar lo que puso gpt con la anterior implementación
void removeMemoryBlock(size_t n);
void printMemoryBlockList();
void cleanMemoryBlockList();






#endif //MEMORYBLOCKLIST_H
