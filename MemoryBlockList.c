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
            fprintf(stderr, "Unknown allocation type\n");
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
    *p = malloc(sizeof(struct tNodeB));
    return *p != BNULL;
}

tPosB firstB(MemoryBlockList L) {
    return L;
}

tPosB next(MemoryBlockList L) {
    return L->next;
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

void removeMemoryBlock(MemoryBlockList *L, tPosB pos) {
    if (!isEmptyListB(L)) {
        tPosB q;
        if (pos == *L) {
            *L = (*L) -> next;
        }else if (pos->next == BNULL) {
            for(q = *L; q->next != pos; q = q->next);
            q->next = BNULL;
        }else {
            q = pos->next;
            pos->data = q->data;
            pos->next = q->next;
            pos = q;
        }
        free(pos);
    } else {
        printf("La lista de bloques assignados está vacía\n");
    }
}

static void printBlockDetails(tPosB p) {
    printf("Address: %p\n", p->data.address);
    printf("Size: %zu\n", p->data.size);
    printf("Allocation Time: %s\n", p->data.allocationTime);  //     // Ya se guarda la hora formateada previamente en la estructura, así que ahora solo la imprimimos.
    printf("Type: %s\n", CategoryToString(p->data.type));
    if (p->data.type == SHARED_MEMORY) {
        printf("Shared Memory Key: %d", p->data.smKey);
    } else if (p->data.type == MAPPED_FILE) {
        printf("File Name: %s", p->data.fileName);
        printf("File Descriptor: %d", p->data.fileDescriptor);
    }
    printf("\n");
}



void printAllBlocks(MemoryBlockList L) {
    if (!isEmptyListB(&L)) {
        printf("******Lista de bloques asignados para el proceso %d\n", getpid());
        for (tPosB p = L; p != BNULL; p = p->next) {
            printBlockDetails(p);
        }
    } else {
        printf("******Lista de bloques asignados para el proceso %d\n", getpid());
    }
}

void printEspecificBlocks(MemoryBlockList L, AllocationType type) {
    if (!isEmptyListB(&L)) {
        printf("******Lista de bloques asignados con %s para el proceso %d\n", CategoryToString(type), getpid());
        for (tPosB p = L; p != BNULL; p = p->next) {
            if (p->data.type == type) {
                printBlockDetails(p);
            }
        }
    } else {
        printf("******Lista de bloques asignados %s para el proceso %d\n", CategoryToString(type),getpid());
    }
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

