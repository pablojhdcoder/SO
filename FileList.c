#include "FileList.h"
#include "HistoryList.h"

//Función auxiliar para crear un nuevo nodo
bool createNodeF(tPosF *p) {
    *p = malloc(sizeof(struct tNode));  //Asigna memoria para un nuevo nodo
    return *p != FNULL; // Devuelve true si la asignación es exitosa
}

//Inicializa la lista de archivos abiertos como vacía
void createEmptyListF(OpenFileList *L){  //Se pasa por referencia porque se quiere modificar
    *L = FNULL;  //Hacemos que L apunte a un NULL
}

//Verifica si la lista de archivos abiertos está vacía
bool isEmptyList(OpenFileList L){
    return L == FNULL;  //Miramos si L apunta a  NULL
}

//Retorna la primera posición de la lista
tPosF firstF(OpenFileList L){
    return L;  //El primer elemento de la lista es el nodo al que apunta L
}

tPosF lastF(OpenFileList L){
    tPosF p;
    if (L == FNULL) {  // Si la lista está vacía, no hay último nodo
        return FNULL;
    }
    for(p = L; p->next != FNULL; p = p->next);                  //p recorre la lista hasta que encuentre un nodo cuyo campo next es NULL, es decir es decir el campo next de la última posición, el cual consta como NULL por no apuntar a otro nodo.
    return p;
}

//Retorna el nodo siguiente al nodo proporcionado
tPosF nextF(tPosF p){
    return p->next;  //Devuelve el nodo al que apunta el campo next de la variable introducida
}


//Inserta un nuevo archivo en la lista de archivos abiertos
bool insertItemF(tItemF d, OpenFileList *L) {
    tPosF q, p;
    if (!createNodeF(&q)) {   //Crea un nuevo nodo
        return false;   //Retorna false si no se pudo asignar memoria
    }
    q->data = d;   //Asigna los datos al nuevo nodo
    q->next = FNULL;
    if (*L == FNULL) {   //Caso 1: inserción en lista vacía
        *L = q;   //El nuevo nodo es el primer nodo
    } else {   //Caso 2: inserción en lista no vacía
        p = lastF(*L);   //Busca el último nodo
        p->next = q;   //Añade el nuevo nodo al final
    }
    return true;
}

//Busca un archivo en la lista por su descriptor
tPosF findFile(const int df, OpenFileList L) {
    tPosF p;
    if (isEmptyList(L)) {   //Si la lista está vacía
        p = FNULL;   //Retorna NULL
    } else {
        for (p = L; p != NULL && p->data.df != df; p = p->next);   //Recorre la lista hasta encontrar el archivo
    }
    return p;   //Retorna la posición o NULL si no se encuentra
}

//Define un nuevo archivo con su descriptor, modo y nombre
tItemF defineItem(const int df, const int mode, const char *name) {
    tItemF p;
    p.df = df;   //Asigna el descriptor de archivo
    p.mode = mode;   //Asigna el modo del archivo
    strncpy(p.name, name, MAXNAME - 1);   //Copia el nombre del archivo, asegurándose de dejar espacio para el terminador null
    p.name[MAXNAME - 1] = '\0';   //Añade el terminador null al final del nombre
    return p;
}


//Función para obtener el nombre de un fichero por su descriptor
tItemF getItemF(tPosF p) {
    return p->data;  //Devuelve el campo data de la posicion p
}

//Función para eliminar un fichero de la lista
void deleteOpenFileAtPositionF(tPosF p, OpenFileList *L) {
    tPosF q;
    if (p == *L) {   //Caso 1: eliminación en el primer nodo
        *L = (*L)->next;
    } else if (p->next == FNULL) {   //Caso 2: eliminación en el último nodo
        for (q = *L; q->next != p; q = q->next);   //Busca el nodo anterior al último
        q->next = FNULL;    //Elimina el enlace al nodo a eliminar
    } else {   //Caso 3: eliminación en el medio
        q = p->next;
        p->data = q->data;   //Copia los datos del siguiente nodo al actual
        p->next = q->next;   //Ajusta los enlaces
        p = q;   //El nodo a eliminar es el siguiente
    }
    free(p);   //Libera la memoria del nodo eliminado
}

void CleanListF(OpenFileList *L) {
    tPosF p = FNULL;
    int df = 0;
    while (*L != FNULL) {
        p = *L;
        *L = (*L)->next;
        df = p->data.df;
        // Intenta cerrar el descriptor de archivo si es válido y no es estándar, ya que si cerramos stdin, stdout o stderr, cualquier operación de lectura, escritura o manejo de errores asociada con la entrada/salida estándar fallará o producirá comportamientos impredecibles antes de que el programa llegue a su fin
        if (df >= 3) {
            if (close(df) == -1) {
                fprintf(stderr,"Error al cerrar el archivo con df: %d\n",df);
            }
        }
        free(p);
    }
}




char *getFileMode(int flags) {
    static char modeString[128];  // Cadena estática para almacenar la representación de los modos
    modeString[0] = '\0';  // Asegurar que la cadena comienza vacía

    // Concatenar los modos activados
    //O_ACCMODE solo se utiliza para determinar si el modo es de lectura, escritura o lectura/escritura
    if (flags & O_CREAT)  strcat(modeString, "O_CREAT ");
    if (flags & O_EXCL)   strcat(modeString, "O_EXCL ");
    if (flags & O_APPEND) strcat(modeString, "O_APPEND ");
    if (flags & O_TRUNC)  strcat(modeString, "O_TRUNC ");
    if ((flags & O_ACCMODE) == O_RDONLY) strcat(modeString, "O_RDONLY ");
    if ((flags & O_ACCMODE) == O_WRONLY) strcat(modeString, "O_WRONLY ");
    if ((flags & O_ACCMODE) == O_RDWR)   strcat(modeString, "O_RDWR ");

    return modeString;  // Devolver la cadena construida
}


// Función para listar los ficheros abiertos
void printListF(OpenFileList L) {
    tPosF p = L;
    if (p == FNULL) {       //Si la lista está vacía
        printf("No hay ficheros abiertos\n");
    } else {
        while (p != FNULL) {
            printf("Descriptor: %d -> %s %s\n", p->data.df, p->data.name, getFileMode(p->data.mode));
            p = p->next;
        }
    }
}
