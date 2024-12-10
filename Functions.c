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

void InitializateShellLists (CommandListC *c, HistoryList *h, OpenFileList *f, MemoryBlockList *b, ProcessList *p, DirectoryList *d) {
    createEmptyListC(c);
    InsertPredefinedCommands(c);
    createEmptyListH(h);
    createEmptyListF(f);
    AddStandardFileDescriptorsToOpenFileList(f);
    createEmptyListB(b);
    createEmptyListP(p);
    createEmptyListD(d);
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
void readInput(char *env[], bool *finished, CommandListC *commandList, HistoryList *history, OpenFileList *openFileList, MemoryBlockList *memoryBlockList, ProcessList *processList, DirectoryList *directoryList){
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
            processInput(finished, &cadena, trozos, env, commandList, history, openFileList, memoryBlockList, processList, directoryList);  //Procesa la entrada
        }
    } else {
        perror("Error al leer la entrada");  //Imprime un mensaje de error si la lectura falla
    }
}

void InsertPredefinedCommands(CommandListC *commandList) {
    const char *Names[] = {
        "authors", "pid", "ppid", "cd", "date",
        "historic", "open", "close", "dup", "infosys",
        "makefile", "makedir", "cwd", "listfile", "listdir",
        "reclist", "revlist", "erase", "delrec",
        "allocate", "deallocate", "memfill", "memdump",
        "memory", "readfile", "writefile", "read",
        "write", "recurse", "getuid", "setuid", "showvar",
        "changevar", "subsvar", "environ", "fork", "search",
        "exec", "execpri", "fg", "fgpri", "back", "backpri",
        "listjobs", "deljobs", "help", "quit", "exit", "bye"
    };

    const char *Descriptions[] = {
        " [-n|-l] Muestra los nombres y/o logins de los autores",
        " [-p] Muestra el pid del shell o de su proceso padre",
        " Muestra el pid del proceso padre del shell",
        " [dir] Cambia (o muestra) el directorio actual del shell",
        " [-d|-t] Muestra la fecha y/o la hora actual",
        " [-c|-N|N] Muestra (o borra) el historico de comandos \n"
        "\t-N: muestra los N primeros \n"
        "\t-c: borra el historico \n"
        "\tN: repite el comando N",
        " fich m1 m2... Abre el fichero fich y lo anade a la lista de ficheros abiertos del shell",
        " df Cierra el descriptor df y elimina el correspondiente fichero de la lista de ficheros abiertos",
        " df Duplica el descriptor de fichero df y anade una nueva entrada a la lista ficheros abiertos",
        " Muestra informacion de la maquina donde corre el shell",
        " [name] Crea un fichero de nombre name",
        " [name] Crea un directorio de nombre name",
        " Muestra el directorio actual del shell",
        " [-long][-link][-acc] name1 name2... Lista ficheros",
        " [-hid][-long][-link][-acc] n1 n2... Lista contenidos de directorios",
        " [-hid][-long][-link][-acc] n1 n2... Lista recursivamente contenidos de directorios (subdirs despues)",
        " [-hid][-long][-link][-acc] n1 n2... Lista recursivamente contenidos de directorios (subdirs antes)",
        " [name1 name2...] Borra ficheros o directorios vacios",
        " [name1 name2...] Borra ficheros o directorios no vacios recursivamente",
        " [-malloc|-shared|-createshared|-mmap]... Asigna un bloque de memoria",
        " [-malloc|-shared|-delkey|-mmap|addr]... Desasigna un bloque de memoria",
        " addr cont byte Llena la memoria a partir de addr con byte",
        " addr cont Vuelca en pantalla los contenidos (cont bytes) de la posicion de memoria addr",
        " [-blocks|-funcs|-vars|-all|-pmap]... Muestra detalles de la memoria del proceso",
        " fiche addr cont Lee cont bytes desde fich a la direccion addr",
        " [-o] fiche addr cont Escribe cont bytes desde la direccion addr a fich",
        " df addr cont Transfiere cont bytes del fichero descrito por df a la direccion addr",
        " df addr cont Transfiere cont bytes desde la direccion addr al fichero descrito por df",
        " [n] Invoca a la funcion recursiva n veces",
        " Muestra las credenciales del proceso que ejecuta el shell",
        " [-l] id Cambia las credenciales del proceso que ejecuta el shell",
        " var Muestra el valor y las direcciones de la variable de entorno var",
        " [-a|-e|-p] var valor Cambia el valor de una variable de entorno",
        " [-a|-e] var1 var2 valor Sustituye la variable de entorno var1",
        " [-environ|-addr] Muestra el entorno del proceso",
        " El shell hace fork y queda en espera a que su hijo termine",
        " [-add|-del|-clear|-path]... Manipula o muestra la ruta de busqueda del shell",
        " VAR1 VAR2... prog args... Ejecuta, sin crear proceso, prog con argumentos",
        " prio prog args... Ejecuta, sin crear proceso, prog con prioridad cambiada",
        " prog args... Crea un proceso que ejecuta en primer plano prog con argumentos",
        " prio prog args... Crea un proceso que ejecuta en primer plano prog con prioridad cambiada",
        " prog args... Crea un proceso que ejecuta en segundo plano prog con argumentos",
        " prio prog args... Crea un proceso que ejecuta en segundo plano prog con prioridad cambiada",
        " Lista los procesos en segundo plano",
        " [-term][-sig] Elimina los procesos de la lista de procesos en segundo plano",
        " [cmd|-all] Muestra ayuda sobre los comandos",
        " Termina la ejecucion del shell",
        " Termina la ejecucion del shell",
        " Termina la ejecucion del shell"
    };

    // Calculamos la longitud de ambas listas
    int NumNames = sizeof(Names) / sizeof(Names[0]);

    // Copiamos los valores en el struct CommandList
    for (int i = 0; i < NumNames; i++) {
        if (!insertCommandC(commandList, Names[i], Descriptions[i], i)) {
            fprintf(stderr, "Error al insertar el comando '%s'\n", Names[i]);
        }
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

void processInput(bool *finished,tItemH *str,char *pieces[],char *envp[], CommandListC *commandList, HistoryList *history,OpenFileList *fileList, MemoryBlockList *memoryBlockList, ProcessList *processList, DirectoryList *directoryList){
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
            command_historic(pieces,finished,commandList,history,fileList,memoryBlockList, processList, directoryList);
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
            command_allocate(pieces, memoryBlockList, fileList);
            break;
        case 20:
            command_deallocate(pieces,memoryBlockList);
            break;
        case 21:
            command_memfill(pieces);
            break;
        case 22:
            command_memdump(pieces);
            break;
        case 23:
            command_memory(pieces, *memoryBlockList);
            break;
        case 24:
            command_readfile(pieces);
            break;
        case 25:
            command_writefile(pieces);
            break;
        case 26:
            command_read(pieces, fileList);
            break;
        case 27:
            command_write(pieces);
            break;
        case 28:
            command_recurse(pieces);
            break;
        case 29:
            command_getuid();
            break;
        case 30:
            command_setuid(pieces);
            break;
        case 31:
            command_showvar(pieces, envp);
            break;
        case 32:
            command_changevar(pieces, envp);
            break;
        case 33:
            command_subsvar(pieces, envp);
            break;
        case 34:
            command_environ(pieces, envp);
            break;
        case 35:
            command_fork(processList);
        break;
        case 36:
            command_search(pieces,directoryList);
            break;
        case 37:
            command_exec(pieces + 1,directoryList, envp);
            break;
        case 38:
            command_execpri(pieces + 1,directoryList, envp);
            break;
        case 39:
            command_fg(pieces + 1, envp, directoryList);
            break;
        case 40:
            command_fgpri(pieces + 1, envp, directoryList);
            break;
        case 41:
            command_back(pieces + 1, envp, directoryList, processList);
            break;
        case 42:
            command_backpri(pieces + 1, envp, directoryList, processList);
            break;
        case 43:
            command_listjobs(processList);
            break;
        case 44:
            command_deljobs(processList);
            break;
        case 45:
            command_help(pieces,commandList);
            break;
        case 46:
        case 47:
        case 48:
            command_exit(finished,fileList,history,commandList, memoryBlockList, directoryList, processList);
            break;
        default:
            executeExternalCommand(pieces, envp, directoryList);
    }
}
