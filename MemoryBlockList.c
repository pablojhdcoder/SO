//
// Created by pablojhd on 4/11/24.
//

#include "MemoryBlockList.h"

#include <unistd.h>

//Función útil para los comandos, hacer la de StringToCategory
const char *CategoryToString(AllocationType type){
    switch (type) {
        case MALLOC_MEMORY:
            return "malloc";
        case SHARED_MEMORY:
            return "shared";
        case MAPPED_FILE:
            return "mapped";
        default:
            return "undefined";
    }
}

void createEmptyListB(MemoryBlockList *L) {
    *L = BNULL;
}
bool isEmptyListB(MemoryBlockList *L) {
    return *L == BNULL;
}

bool createNode(tPosB *p) {
    *p = malloc(sizeof(struct tNode));
    return *p != BNULL;
}


tPosB lastPosB(MemoryBlockList L) {
    tPosB p;
    if (L == BNULL) {
        return BNULL;
    }
    for (p = L; p->next!=BNULL; p=p->next);
    return p;
}

void getFormattedTime(char *buffer, size_t bufferSize) {
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    strftime(buffer, bufferSize, "%b %d %H:%M:%S", localTime);
}

//Ojo strdup en filename
bool insertMemoryBlockB(MemoryBlockList *L, void *address, size_t size, AllocationType type, key_t Key, const char *fileName, int fileDescriptor) {
    tPosB p,q;
    if(!createNode(&p)) {
        return false;
    }
    p->data.address = address;
    p->data.size = size;
    getFormattedTime(p->data.allocationTime, sizeof(p->data.allocationTime));
    p->data.type = type;
    p->data.smKey = (type == SHARED_MEMORY) ? Key : -1;
    p->data.fileName = (type == MAPPED_FILE && fileName != BNULL) ? strdup(fileName) : BNULL;     //strdup asigna dinámicamente suficiente memoria para almacenar una copia de la cadena dada,Copia la cadena en la nueva memoria asignada. Devuelve un puntero a la nueva cadena duplicada, cuando se borre el elemento acordarse de hacer free(fileName)
    p->data.fileDescriptor = (type == MAPPED_FILE) ? fileDescriptor : -1;
    p->next = BNULL;

    if (*L == BNULL) {
        *L = p;
    }else {
        q = lastPosB(*L);
        q->next = p;
    }
    return true;
}

void removeMemoryBlock(MemoryBlockList *L, size_t size) {
    if (!isEmptyListB(L)) {
        tPosB q;
        for (tPosB p = *L; p != BNULL; p=p->next) {
            if (p->data.size == size) {
                if (p==*L) {
                    *L = (*L)->next;
                }else if (p->next == BNULL) {
                    for(p = *L; p->next != p; p = p->next);
                    p->next = BNULL;
                }else {
                    q = p->next;
                    p->data = q->data;
                    p->next = q->next;
                    p = q;
                }
                // Liberamos la memoria usada por el nodo
                if (p->data.fileName != BNULL) {
                    free(p->data.fileName);  // Liberar la memoria de fileName si fue asignada
                }
                free(p);
                return;
            }
        }
        printf("No block with the given size found.\n");
    }else
        printf("The memory block list is empty.\n");

}
void printMemoryBlockList(MemoryBlockList L) {
    if(!isEmptyListB(&L)) {
        printf("******Lista de bloques asignados malloc para el proceso %d",getpid());
        for (tPosB p = L; p!=BNULL; p=p->next) {
            // Ya se guarda la hora formateada previamente en la estructura, así que ahora solo la imprimimos.
            printf("Address: %p", p->data.address);
            printf("Size: %zu", p->data.size);
            printf("Allocation Time: %s", p->data.allocationTime);  // Mostrar la hora formateada guardada
            printf("Type: %s", CategoryToString(p->data.type));
            if (p->data.type == SHARED_MEMORY) {
                printf("Shared Memory Key: %d", p->data.smKey);
            } else if (p->data.type == MAPPED_FILE) {
                printf("File Name: %s", p->data.fileName);
                printf("File Descriptor: %d", p->data.fileDescriptor);
            }
            printf("\n");
        }
    }else
        printf("******Lista de bloques asignados para el proceso %d",getpid());
}
void cleanMemoryBlockList(MemoryBlockList *L)  {
    tPosB p = BNULL;
    while (*L != BNULL) {
        p = *L;
        *L = (*L)->next;
        if (p->data.fileName != BNULL) {
            free(p->data.fileName);
        }
        free(p);
    }
}


