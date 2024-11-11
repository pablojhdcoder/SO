#include "Functions.h"

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

//Función que imprime el prompt
void printPrompt(){
    printf("→ ");
    fflush(stdout);       //Vacía el buffer para que el prompt se muestre inmediatamente
}
static void AddStandardFileDescriptorsToOpenFileList(OpenFileList *L) {
    const char *names[3] = {"stdin","stdout","stderr"};
    int flags;
    tItemF item;
    for (int i = 0; i < 3; ++i) {
        flags = fcntl(i, F_GETFL);
        item = defineItem(i,flags,names[i]);
        insertItemF(item,L);
    }
}

void InitializateShellLists (CommandListC *c, HistoryList *h, OpenFileList *f, MemoryBlockList *b) {
    createEmptyListC(c);
    InsertPredefinedCommands(c);
    createEmptyListH(h);
    createEmptyListF(f);
    AddStandardFileDescriptorsToOpenFileList(f);
    createEmptyListB(b);
}

//Función auxiliar para dividir una cadena en palabras
int SplitString(char *str, char *pieces[]){
    int i = 1;
    if ((pieces[0] = strtok(str, " \n\t")) == NULL) //Divide la cadena por espacios, saltos de línea o tabulaciones
        return 0;
    while ((pieces[i] = strtok(NULL, " \n\t")) != NULL)  //Sigue dividiendo la cadena por los delimitadores
        i++;                                        //Incrementa el índice por cada palabra encontrada
    return i;                                       //Retorna el número de palabras encontradas
}

//Función auxiliar para añadir un comando al historial
static void AddToHistoryList(tItemH *command, HistoryList *lista){
    tItemH *newItem = command;        //Crea una referencia al comando
    insertCommandH(newItem, lista);   //Inserta el comando en la lista de historial
}

//Función auxiliar para leer la entrada introducida por el usuario
void readInput(bool *finished, CommandListC *commandList, HistoryList *history, OpenFileList *openFileList, MemoryBlockList *memoryBlockList) {
    char input[LENGTH_MAX_INPUT];  //Buffer para almacenar la entrada del usuario

    if (fgets(input, LENGTH_MAX_INPUT, stdin) != NULL) {  //Lee la entrada del usuario desde la consola
        char *trozos[LENGTH_MAX_INPUT];  //Array para almacenar los trozos de la entrada
        tItemH cadena;                    //Variable para almacenar la cadena de entrada
        strcpy(cadena, input);            //Guarda una copia de la cadena de entrada en el historial
        size_t len = strlen(cadena);      //Obtiene la longitud de la cadena
        //Si la cadena tiene longitud mayor que 0 y termina con un salto de línea
        if (len > 0 && cadena[len - 1] == '\n') {
            cadena[len - 1] = '\0';       //Reemplaza '\n' por '\0' para evitar problemas al imprimir en el historial
        }

        int NumTrozos = SplitString(input, trozos);  //Divide la cadena en trozos (palabras) y devuelve el número de trozos

        if (NumTrozos > 0) {  //Si se han encontrado trozos, procesa la entrada
            processInput(finished, &cadena, trozos, commandList, history, openFileList, memoryBlockList);  //Procesa la entrada
        }
    } else {
        perror("Error al leer la entrada");  //Imprime un mensaje de error si la lectura falla
    }
}
//Debería de poner los comandos aquí en una función que los devuelva con sus descripciones y luego inicializar la lista de comandos con una función del TAD?
void InsertPredefinedCommands(CommandListC *commandList) {
   const char *Names[] = {
        "authors", "pid", "ppid", "cd", "date",
        "historic", "open", "close", "dup", "infosys",
        "makefile", "makedir","cwd", "listfile", "listdir",
        "reclist", "revlist", "erase", "delrec",
        "allocate","deallocate", "memfill","memdump",
        "memory", "readfile", "writefile", "read",
        "write", "recurse","help", "quit", "exit", "bye"
    };
    const char *Descriptions[] = {
        " [-n|-l] Muestra los nombres y/o logins de los autores",
        " [-p] Muestra el pid del shell o de su proceso padre",
        " Muestra el pid del proceso padre del shell",
        " [dir]	Cambia (o muestra) el directorio actual del shell",
        " [-d|-t] Muestra la fecha y/o la hora actual",
        " [-c|-N|N]	Muestra (o borra)el historico de comandos \n"
        "\t-N: muestra los N primeros \n"
        "\t-c: borra el historico \n"
        "\tN: repite el comando N",
        " fich m1 m2...	Abre el fichero fich y lo anade a la lista de ficheros abiertos del shell \n"
        "\tm1, m2..es el modo de apertura (or bit a bit de los siguientes) \n"
        "\tcr: O_CREAT	ap: O_APPEND \n"
        "\tex: O_EXCL 	ro: O_RDONLY \n"
        "\trw: O_RDWR 	wo: O_WRONLY \n"
        "\ttr: O_TRUNC ",
        " df Cierra el descriptor df y elimina el correspondiente fichero de la lista de ficheros abiertos",
        " df Duplica el descriptor de fichero df y anade una nueva entrada a la lista ficheros abiertos",
        " Muestra informacion de la maquina donde corre el shell",
        " [name] Crea un fichero de nombre name",
        " [name] Crea un directorio de nombre name",
        "Muestra el directorio actual del shell",
        " [-long][-link][-acc] name1 name2 ..	lista ficheros; \n"
        "\t-long: listado largo \n"
        "\t-acc: acesstime \n"
        "\t-link: si es enlace simbolico, el path contenido ",
        " [-hid][-long][-link][-acc] n1 n2 .. lista contenidos de directorios \n"
        "\t-long: listado largo \n"
        "\t-hid: incluye los ficheros ocultos \n"
        "\t-acc: acesstime \n"
        "\t-link: si es enlace simbolico, el path contenido",
        " [-hid][-long][-link][-acc] n1 n2 .. lista recursivamente contenidos de directorios (subdirs despues) \n"
        "\t-hid: incluye los ficheros ocultos \n"
        "\t-long: listado largo \n"
        "\t-acc: acesstime \n"
        "\t-link: si es enlace simbolico, el path contenido",
        " [-hid][-long][-link][-acc] n1 n2 .. lista recursivamente contenidos de directorios (subdirs antes) \n"
        "\t-hid: incluye los ficheros ocultos \n"
        "\t-long: listado largo \n"
        "\t-acc: acesstime \n"
        "\t-link: si es enlace simbolico, el path contenido ",
        " [name1 name2 ..] Borra ficheros o directorios vacios",
        " [name1 name2 ..] Borra ficheros o directorios no vacios recursivamente",
        " [-malloc|-shared|-createshared|-mmap]... Asigna un bloque de memoria \n"
        "\t-malloc tam: asigna un bloque malloc de tamano tam \n"
        "\tcreateshared cl tam: asigna (creando) el bloque de memoria compartida de clave cl y tamano tam \n"
        "\t-shared cl: asigna el bloque de memoria compartida (ya existente) de clave cl"
        "\t-mmap fich perm: mapea el fichero fich, perm son los permisos",
        " [-malloc|-shared|-delkey|-mmap|addr]..	Desasigna un bloque de memoria \n"
        "\t-malloc tam: desasigna el bloque malloc de tamano tam \n"
        "\t-shared cl: desasigna (desmapea) el bloque de memoria compartida de clave cl \n"
        "\t-delkey cl: elimina del sistema (sin desmapear) la clave de memoria cl \n"
        "\t-mmap fich: desmapea el fichero mapeado fich \n"
        "\taddr: desasigna el bloque de memoria en la direccion addr",
        " addr cont byte Llena la memoria a partir de addr con byte",
        " addr cont Vuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr",
        " [-blocks|-funcs|-vars|-all|-pmap] ..	Muestra muestra detalles de la memoria del proceso \n"
        "\t-blocks: los bloques de memoria asignados \n"
        "\t-funcs: las direcciones de las funciones \n"
        "\t-vars: las direcciones de las variables \n"
        "\t:-all: todo \n"
        "\t-pmap: muestra la salida del comando pmap(o similar)",
        " fiche addr cont 	Lee cont bytes desde fich a la direccion addr",
        " [-o] fiche addr cont 	Escribe cont bytes desde la direccion addr a fich (-o sobreescribe)",
        " df addr cont	Transfiere cont bytes del fichero descrito por df a la dirección addr",
        " df addr cont	Transfiere cont bytes desde la dirección addr al fichero descrito por df",
        " [n]	Invoca a la funcion recursiva n veces",
        " [cmd|-all] Muestra ayuda sobre los comandos \n"
        "\t-cmd: info sobre el comando cmd \n"
        "\t-all: lista todos los comandos con su información ",
        " Termina la ejecucion del shell",
        " Termina la ejecucion del shell",
        " Termina la ejecucion del shell"
    };

    // Obtenemos el numero total de comandos dividiendo el tamaño total entre el tamaño de un comando
    int NumComandos = sizeof(Names) / sizeof(Names[0]);
    // Copiamos los valores en el struct CommandList
    for (int i = 0; i < NumComandos; i++) {
        if (!insertCommandC(commandList,Names[i],Descriptions[i],i))
            perror ("Error insertando los comandos predefinidos");
    }
}


//Obtenemos el ID del comando para luego poder elegir en el switch, además aprovechamos y guardamos en el historial
static int getCommandId(tItemH *str, char *pieces[], CommandListC *commandList, HistoryList *history) {
    int id = FindCommandC(commandList, pieces[0]);  //Busca el ID del comando en la lista de comandos
    if (id != CNULL) {                              //Si el comando es válido (ID encontrado)
        AddToHistoryList(str, history);             //Añade el comando al historial
        return id;                                  //Retorna el ID del comando
    }
    return -1;                                      //Si el comando no es válido, retorna -1
}
//Procesa el comando introducido //Se puede hacer privada??
void processInput(bool *finished,tItemH *str,char *pieces[], CommandListC *commandList, HistoryList *history,OpenFileList *fileList, MemoryBlockList *memoryBlockList){
    switch (getCommandId(str,pieces,commandList,history)) {
        case 0:
            command_authors(pieces);
            break;
        case 1:
            command_pid();
            break;
        case 2:
            command_ppid();
            break;
        case 3:
            command_cd(pieces);
            break;
        case 4:
            command_date(pieces);
            break;
        case 5:
            command_historic(pieces,finished,commandList,history,fileList,memoryBlockList);
            break;
        case 6:
            command_open(pieces,fileList);
            break;
        case 7:
            command_close(pieces,fileList);
            break;
        case 8:
            command_dup(pieces,fileList);
            break;
        case 9:
            command_infosys();
            break;
        case 10:
            command_makefile(pieces);
            break;
        case 11:
            command_makedir(pieces);
            break;
        case 12:
            command_cwd();
            break;
        case 13:
        case 14:
            command_list_File_Dir(pieces);
            break;
        case 15:
            command_reclist(pieces);
            break;
        case 16:
            command_revlist(pieces);
            break;
        case 17:
            command_erase(pieces);
            break;
        case 18:
            command_delrec(pieces);
            break;
        case 19:
            command_allocate(pieces, memoryBlockList);
            break;
        case 20:
            command_deallocate();
            break;
        case 21:
            command_memfill(pieces);
            break;
        case 22:
            command_memdump();
            break;
        case 23:
            command_memory(pieces);
            break;
        case 24:
            command_readfile();
            break;
        case 25:
            command_writefile(pieces);
            break;
        case 26:
            command_read();
            break;
        case 27:
            command_write(pieces);
            break;
        case 28:
            command_recurse();
            break;
        case 29:
            command_help(pieces,commandList);
            break;
        case 30:
        case 31:
        case 32:
        command_exit(finished,fileList,history,commandList, memoryBlockList);
            break;
        default:
            perror("Comando no válido, introduce \"help\" para ver los disponibles");
    }
}
