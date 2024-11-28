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
bool isEmptyListB(MemoryBlockList L) {
    return L == BNULL;
}

bool createNodeB(tPosB *p) {
    *p = malloc(sizeof(struct tNodeB));
    return *p != BNULL;
}

tPosB firstB(MemoryBlockList L) {
    return L;
}

tPosB nextB(MemoryBlockList L) {
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

void getFormattedTimeB(char *buffer, size_t bufferSize) {
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    strftime(buffer, bufferSize, "%b %d %H:%M:%S", localTime);
}

//Ojo strdup en filename
bool insertMemoryBlockB(MemoryBlockList *L, void *address, size_t size, AllocationType type, key_t Key, const char *fileName, int fileDescriptor) {
    tPosB p,q;
    if(!createNodeB(&p)) {
        return false;
    }
    p->data.address = address;
    p->data.size = size;
    getFormattedTimeB(p->data.allocationTime, sizeof(p->data.allocationTime));
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
    if (!isEmptyListB(*L)) {
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
    /*
     * 1. Dirección en donde está almacenado el bloque de memoria (p->data.address)
     * 2. Tamaño del bloque de memoria (p->data.size)
     * 3. Fecha en que se alocó el bloque de memoria (p->data.allocationTime)
     * 4. Clave de memoria compartida, si se usó memoria compartida (p->data.smKey)
     * 5. Nombre del fichero, si el bloque se alocó mapeando un archivo (p->data.fileName)
     * 6. Descriptor de archivo, si el bloque está asociado con un archivo mapeado (p->data.fileDescriptor)
     * 7. Tipo de recurso con el que se alocó el bloque de memoria (p->data.type, puede ser: MALLOC_MEMORY, SHARED_MEMORY o MAPPED_FILE)
     */
    // Ya se guarda la hora formateada previamente en la estructura, así que ahora solo la imprimimos.
    printf("%20p\t%zu\t%s\t", p->data.address, p->data.size, p->data.allocationTime);
    if (p->data.type == SHARED_MEMORY) {
        printf("shared (key %d)", p->data.smKey);
    } else if (p->data.type == MAPPED_FILE) {
        printf("%s (descriptor %d)", p->data.fileName, p->data.fileDescriptor);
    } else {
        printf("%s", CategoryToString(p->data.type));
    }
    printf("\n");
}



void printAllBlocks(MemoryBlockList L) {
    printf("******Lista de bloques asignados para el proceso %d\n", getpid());
    if (!isEmptyListB(L)) {
        for (tPosB p = L; p != BNULL; p = p->next) {
            printBlockDetails(p);
        }
    } else {
        printf("No hay bloques asignados\n");
    }
}

void printEspecificBlocks(MemoryBlockList L, AllocationType type) {
    printf("******Lista de bloques asignados con %s para el proceso %d\n", CategoryToString(type), getpid());
    if (!isEmptyListB(L)) {
        for (tPosB p = L; p != BNULL; p = p->next) {
            if (p->data.type == type) {
                printBlockDetails(p);
            }
        }
    } else {
        printf("No hay bloques asignados con %s\n", CategoryToString(type));
    }
}

void cleanMemoryBlockList(MemoryBlockList *L) {
    tPosB p = BNULL;
    while (*L != BNULL) {
        p = *L;
        *L = (*L)->next;
            switch (p->data.type) {
                case MALLOC_MEMORY:
                    free(p->data.address);
                break;
                case MAPPED_FILE:
                    if(munmap(p->data.address,p->data.size) == -1) {
                        perror("Error al desmapear el archivo");
                    }
                    free(p->data.fileName);
                break;
                case SHARED_MEMORY:
                    if (shmdt(p->data.address) == -1)
                        perror("Error al desvincular memoria compartida");
                break;
            }
        free(p);
    }
}

