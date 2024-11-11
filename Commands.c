#include "Commands.h"

int ext_var1, ext_var2, ext_var3;
int ext_init_var1 = 10, ext_init_var2 = 20, ext_init_var3 = 30;  // Valores de ejemplo


//Imprime el nombre y/o los logins de los autores
void command_authors(char * pieces[]) {
    const char * names[] = {"Pablo Herrero","Tiago Volta"};
    const char * logins[] = {"pablo.herrero.diaz","tiago.velosoevolta"};
    int NumAuthors = 2;
    if(pieces[1] == NULL || strcmp(pieces[1],"-n")==0) {
        printf("Nombres de los autores: \n");
        for(int i=0; i<NumAuthors; i++) {
            printf("%s\n",names[i]);
        }
    }
    if (pieces[1] == NULL || strcmp(pieces[1],"-l") == 0) {
        printf("Logins de los autores: \n");
        for(int i=0; i<NumAuthors; i++) {
            printf("%s\n",logins[i]);
        }
    }
}

//Muestra el PID del proceso actual
void command_pid() {
    printf("PID de shell: %d\n", getpid());
}

//Muestra el PPID del proceso padre
void command_ppid() {
    printf("PID del padre del shell: %d\n", getppid());
}

//Comando que cambia el directorio de trabajo
void command_cd(char *pieces[]) {
    if (pieces[1] == NULL) {                            // Si no se especifica un directorio, imprime el directorio de trabajo actual
        char cwd[LENGTH_MAX_PATH];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Directorio actual: %s\n", cwd);
        } else {
            perror("Error obteniendo el directorio actual");
        }
    } else {                                            // Cambia el directorio de trabajo actual al directorio especificado
        if (chdir(pieces[1]) != 0) {
            perror("Error cambiando el directorio");
        }
    }
}

//Muestra la fecha y hora actuales
void command_date(char *pieces[]) {
    time_t t;
    struct tm *tm_info;
    char buffer[26];

    // Obtiene la hora actual
    time(&t);
    tm_info = localtime(&t);

    //Si no hay argumentos o si se especifica "-t", imprime solo la hora
    if (pieces[1] == NULL || strcmp(pieces[1], "-t") == 0) {
        // Si no hay argumentos o si se especifica "-t", imprime solo la hora
        strftime(buffer, 26, "%H:%M:%S", tm_info);
        printf("Hora actual: %s\n", buffer);
    }

    // Si no hay argumentos o si se especifica "-d", imprime solo la fecha
    if (pieces[1] == NULL || strcmp(pieces[1], "-d") == 0) {
        strftime(buffer, 26, "%Y-%m-%d", tm_info);
        printf("Fecha actual: %s\n", buffer);
    }
}

//Función auxiliar para repetir un comando guardado en el historial
static void repeatCommand(tPosH p,bool *finished, CommandListC *commandList, HistoryList *history, OpenFileList *openFileList, MemoryBlockList *memoryBlockList) {
    char *trozos[LENGTH_MAX_PHRASE];
    tItemH cadena;                                                          //Cadena que almacena el comando a repetir
    tItemH *comando = getItemH(p, history);                                 //Obtiene el comando a repetir
    printf("Ejecutando historic (%d): %s\n",p,*comando);
    strcpy(cadena,*comando);                                                //Copia el comando a cadena
    SplitString(*comando,trozos);                                           //Separa el comando en trozos
    processInput(finished,&cadena,trozos,commandList,history,openFileList, memoryBlockList); //Procesa el comando
}

//Mustra el historial de comandos introducidos, o repite un comando ya introducido o imprimer los últimos n comandos
void command_historic (char *pieces[],bool *finished,CommandListC *commandList, HistoryList *history, OpenFileList *openFileList, MemoryBlockList *memoryBlockList) {
    char *NoValidCharacter;

    //Si se proporciona un argumento numérico
    if (pieces[1] != NULL) {
        const long int n = strtol(pieces[1],&NoValidCharacter,10);     //Variable para almacenar el número si lo hay
        int number = (int) n;                                               //(int) convierte el long int a int, es necesario por el strtol que retorna un long int

        if(*NoValidCharacter == '\0') {                                     //Si todo el argumento es un número válido, entonces el puntero NoValidCharacter apuntará al \0, strtol convirtió la cadena exitosamente
            if (0 <= number && number <= lastH(*history)) {
                repeatCommand(number,finished,commandList,history,openFileList, memoryBlockList);
            }else if (number < 0) {
                number = -number;               //Cambiar el signo
                printLastNH(history,number);    //Imprime los últimos n comandos
            }else
                perror("No se han ejecutado comandos suficientes");
        }else if(NoValidCharacter == pieces[1]) {           //Si no se introdujo ningún número válido el puntero apuntará a la primera posición de pieces[1],
            perror("No se introdujo ningún número válido");
        }else{                                              //Se han introducido carácteres válidos antes de que el puntero apunte a uno no válido
            fprintf(stderr,"Parte de la cadena no es válida: %s\n", NoValidCharacter);
            printf("Parte numérica leída: %d\n", number);   //Se coge la parte que se ha leído exitosamente
            if (0 <= number && number <= lastH(*history)) {
                repeatCommand(number,finished,commandList,history,openFileList, memoryBlockList);
            }else if (number < 0) {
                number = -number;                  //Cambiar el signo
                printLastNH(history,number);       //Imprime los últimos n comandos
            }else
                perror("No se han ejecutado comandos suficientes");
            }
    }else {
        printListH(history);
    }
}

//Función que abre un archivo con un modo específico
void command_open(char *pieces[],OpenFileList *openFileList) {
    int i, df, mode = 0;

    // Si no se especificó un archivo, listar los archivos abiertos
    if (pieces[1] == NULL) {
        printListF(*openFileList);  // Lista los archivos abiertos
        return;
    }

    // Procesar los parámetros para determinar el modo de apertura del archivo
    for (i = 2; pieces[i] != NULL; i++) {
        if (!strcmp(pieces[i], "cr")) mode |= O_CREAT;        //Crea el archivo si no existe.
        else if (!strcmp(pieces[i], "ex")) mode |= O_EXCL;    //Genera un error si el archivo ya existe (solo con O_CREAT).
        else if (!strcmp(pieces[i], "ro")) mode |= O_RDONLY;  //Modo de apertura solo lectura.
        else if (!strcmp(pieces[i], "wo")) mode |= O_WRONLY;  //Modo de apertura solo escritura.
        else if (!strcmp(pieces[i], "rw")) mode |= O_RDWR;    //Modo de apertura para lectura y escritura.
        else if (!strcmp(pieces[i], "ap")) mode |= O_APPEND;  //Abre en modo de adición, escribe al final del archivo.
        else if (!strcmp(pieces[i], "tr")) mode |= O_TRUNC;   //Trunca el archivo a longitud 0 si ya existe (borra contenido).
        else break;
    }


    //Intentar abrir el archivo con el modo especificado
    if ((df = open(pieces[1], mode, 0777)) == -1) {   //Si mode=O_CREAT, open() crea el archivo, y entonces utilizará el tercer parámetro, en este caso 0777, que especifica en octal los permisos de archivo en UNIX, el 0777 significa que todos (propietario, grupo y otros) tienen permisos completos de lectura, escritura y ejecución
        perror("Imposible abrir archivo");
    } else {
        tItemF item = defineItem(df,mode,pieces[1]);  //Define un item con el descriptor, el modo y el nombre del archivo
        if(insertItemF(item,openFileList))            //Añade el archivo a la lista de archivos abiertos
            printf("Archivo abierto: Descriptor: %d -> %s %s\n", df, pieces[1], getFileMode(mode));
        else
            fprintf(stderr,"No se ha podido insertar %s",pieces[1]);
    }
}

//Función que cierra un archivo a partir de su descriptor
void command_close(char *pieces[],OpenFileList *openFileList) {
    int df;
    char *NoValidCharacter, *name;

    // Si no hay un descriptor válido, lista los archivos abiertos
    //Como el operador , asocia a la izquierda entonces se realiza primero la asignación y luego se evalún condiciones sobre esa asignación
    if (pieces[1] == NULL || (df = (int)strtol(pieces[1], &NoValidCharacter, 10), *NoValidCharacter != '\0'||df < 0 )) {        //En el cast podría haber overflow si el número devuelto por strtol es mayor que el rango de un int o pérdida de precisión, pero como los df son valores pequeños no habrá problema
        printListF(*openFileList);  // Lista los archivos abiertos
        return;
    }

    // Intentar cerrar el archivo con el descriptor especificado
    if (close(df) == -1) {
        perror("Imposible cerrar archivo");
    } else {
        tPosF p = findFile(df,*openFileList);  //Busca el archivo en la lista de archivos abiertos
        name = getItemF(p).name;
        deleteOpenFileAtPositionF(p,openFileList);  //Elimina el archivo de la lista de archivos abiertos
        printf("Archivo cerrado: Descriptor: %d -> %s\n", df, name);
    }
}

//Función que duplica un archivo a partir de du descriptor
void command_dup(char *pieces[], OpenFileList *openFileList) {
    int df, duplicated;
    char aux[MAXNAME], *name, *NoValidCharacter;
    tItemF item;

    // Si no hay un descriptor válido, lista los archivos abiertos
    //Como el operador , asocia a la izquierda entonces se realiza primero la asignación y luego se evalún condiciones sobre esa asignación
    if (pieces[1] == NULL ||(df = (int)strtol(pieces[1],&NoValidCharacter,10), *NoValidCharacter != '\0'||df < 0 )) {
        printListF(*openFileList);  // Lista los archivos abiertos
        return;
    }

    //Intenta duplicar el descriptor
    if ((duplicated = dup(df)) == -1) {  //Si no se puede duplicar el descriptor, muestra un error
        perror("Imposible duplicar descriptor");
        return;
    }

    // Obtiene el nombre del archivo asociado con el descriptor original
    name = getItemF(findFile(df,*openFileList)).name;
    int flags = fcntl(duplicated, F_GETFL);      //Obtenemos los flags de estado del archivo, como los modos de apertura

    // Prepara una cadena para mostrar la información del duplicado
    if (snprintf(aux, MAXNAME, "duplicado de %d (%s)", df, name) >= 0) {
        item = defineItem(df,flags,aux);
        if(insertItemF(item,openFileList))                  // Añade el nuevo descriptor duplicado a la lista de archivos abiertos
            printf("Archivo duplicado: Descriptor: %d -> %s %s\n", df, name, getFileMode(flags));
        else
            fprintf(stderr,"No se ha podido duplicar %s",name);
    }
}

//Devuelve información sobre el sistema
void command_infosys() {
    struct utsname sysinfo;

    // Llama a la función uname para obtener la información del sistema
    if (uname(&sysinfo) == -1) {
        perror("Error al obtener la información del sistema");
        return;
    }
    printf("%s (%s), OS: %s - %s - %s \n",sysinfo.nodename,sysinfo.machine,sysinfo.sysname,sysinfo.release,sysinfo.version);
}

//Comando que muestra los comandos disponibles
void command_help(char * pieces[], CommandListC *commandList) {
    //Si se pasa un argumento con el comando help
    if (pieces[1] != NULL) {
        if (strcmp(pieces[1],"-all") == 0) {
            printCommandDescriptionListC(*commandList);      //Muestra la descripción de todos los comandos
            return;
        }
        //Busca la posición del comando en la lista de comandos
        int pos = FindCommandC(commandList,pieces[1]);
        if(pos != CNULL) {                               //Si encuentra el comando, lo imprime
            tCommandC command= getCommandC(pos,*commandList);
            printf("%s %s\n", command.name,command.description);
            return;
        }
        //Si el comando no es válido, imprime un error
        fprintf(stderr, "Comando %s no encontrado, \"help\" para ver los disponibles\n", pieces[1]);  // fprintf escribe una cadena de caracteres formateada en un archivo o flujo determinado, en este caso stderr, como es un flujo no bufferizado los errores se muestran de inmediato en la salida de errores
    }else{
        //Si no se pasa ningún argumento, imprime solo la lista de comandos disponibles
        printCommandListC(*commandList);    //Muestra los comandos sin sus descripciones
    }
}

//Comando que limpia todas las listas, cierra los archivos y finaliza el programa
void command_exit(bool *finished,OpenFileList *openFileList, HistoryList *history, CommandListC *commandList, MemoryBlockList *memoryBlockList) {

    // Limpiamos las listas utilizadas en el programa
    CleanCommandListC(commandList);     //Limpia la lista de comandos
    CleanListH(history);                //Limpia el historial de comandos
    CleanListF(openFileList);           //Limpia la lista de archivos abiertos y cierra los archivos abiertos
    cleanMemoryBlockList(memoryBlockList);  //Limpia la lista de bloques de memoria

    // Establece una bandera para indicar que la shell debe terminar
    *finished = true;

    // Imprime un mensaje de despedida
    printf("Saliendo de la shell...\n");
}

//Sino se le pasa argumento imprime el directorio actual, si se le pasa argumento crea un archivo con ese nomnbre, si ya existe no lo crea
void command_makefile(char *pieces[]) {
    //Si no se pasa ningún argumento, imprime el directorio de trabajo actual
    if (pieces[1] == NULL) {                                //Si no se pasa ningún argumento, imprime el directorio de trabajo actual
        char cwd[LENGTH_MAX_PATH];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {         //Obtiene el directorio de trabajo actual
            printf("Directorio actual: %s\n", cwd);
        } else {
            perror("Error obteniendo el directorio actual");
        }
    }else {
        //Si el archivo ya existe, imprime un mensaje de error
        if (access(pieces[1],F_OK) != -1) {                 //Verifica si el arcchivo ya existe => devuelve -1 sino 0
            fprintf(stderr,": El archivo '%s' ya existe.\n", pieces[1]);
        }else {
            FILE *logFile = fopen(pieces[1], "w");  //Crea un archivo nuevo y lo abre en modo escritura
            if (logFile != NULL && fclose(logFile) != EOF) {     //Verifica si se pudo crear y cerrar el archivo
                printf("Archivo '%s' creado correctamente.\n", pieces[1]);      //Mensaje de confirmación si se creó correctamente
            }else
                perror("Error al cerrar el archivo");    //Imprime un error si no se pudo cerrar el archivo
        }
    }

}


//Función estática (privada) que determina la letra representativa del tipo de archivo según su modo (mode_t)
static char LetraTF (mode_t m){
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
    }
}

//Función estática (privada) que obtiene los permisos de un archivo en formato de cadena de texto
static char *GetPermissions(mode_t m) {
    char *permisos;
    //Reserva memoria para almacenar los permisos (10 caracteres + espacio extra) y comprueba si la asignación fue exitosa
    if ((permisos=(char *) malloc (12))==NULL)
        return NULL;
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';     //Permiso de lectura por el propietario           /*Como esta función utiliza malloc, hace con que la función sea autónoma
    if (m&S_IWUSR) permisos[2]='w';     //Permiso de escritura por el propietario           en cuanto a la gestión de memoria, no necesita preocuparse por la asignación
    if (m&S_IXUSR) permisos[3]='x';     //Permiso de ejecución por el propietario           de memoria previa.
    if (m&S_IRGRP) permisos[4]='r';     //Permiso de lectura por un grupo                   Evita posibles errores, como no reservar suficiente espacio para los permisos*/
    if (m&S_IWGRP) permisos[5]='w';     //Permiso de escritura por un grupo
    if (m&S_IXGRP) permisos[6]='x';     //Permiso de ejecución por un grupo
    if (m&S_IROTH) permisos[7]='r';     //Permiso de lectura por otros
    if (m&S_IWOTH) permisos[8]='w';     //Permiso de escritura por otros
    if (m&S_IXOTH) permisos[9]='x';     //Permiso de ejecución por otros
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}

//Crea un directorio
void command_makedir(char *pieces[]) {
    if (pieces[1] == NULL) {
        char cwd[LENGTH_MAX_PATH];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("Error obteniendo el directorio actual");
        }
        return;
    }
    struct stat st = {0};
    //Verifica si eldirectorio ya existe
    if (stat(pieces[1], &st) == 0 && S_ISDIR(st.st_mode)) {
        fprintf(stderr,"Imposible crear '%s', ya existe.\n", pieces[1]);
        return;
    }
    //Crea el directorio con permisos de lectura, escritura y ejecución para el usuario
    if (mkdir(pieces[1], 0777) == -1) { //Si no tiene permisos para crear el directorio o la ruta es inválida
        perror("No se puede crear el directorio");
    } else {
        printf("Directorio %s creado\n", pieces[1]);

        //Obtener información del nuevo directorio
        if (stat(pieces[1], &st) == 0) {  //Volver a llamar a stat para obtener el modo del nuevo directorio
            // Obtener permisos del directorio
            char *permisos = GetPermissions(st.st_mode);  //Convierte el modo a formato legible
            if (permisos != NULL) {
                printf("Permisos del nuevo directorio: %s\n", permisos);
                free(permisos);  //libera la memoria
            } else {
                perror("Error al asignar memoria para los permisos:");
            }
        } else {
            perror("Error al obtener información del nuevo directorio");
        }
    }
}

//Printea el directorio actual
void command_cwd() {
    char cwd[LENGTH_MAX_PATH]; //buffer para almacenar el directorio actual

    //Obtiene el directorio actual
    if(getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("Error obteniendo el directorio actual");
    }
}


//Funcion auxiliar para obtener los datos de los archivos de un directorio si hide está activo devuelve false y se salta ese archivo
static bool GetFileData(bool hide, struct stat *data, char *dir, char *name, char *filePath) {
    if (!hide && (name[0] == '.' || strcmp(name, "..") == 0)) {              //Si 'hide' es false y el nombre del archivo comienza con '.' o es "..", se oculta
        return false;           //No se obtienen datos de archivos ocultos ni del directorio padre
    }
    //Construye la ruta completa del archivo utilizando snprintf
    snprintf(filePath, LENGTH_MAX_INPUT, "%s/%s", dir, name);
    if (lstat(filePath, data) == -1) {           //Intenta obtener información del archivo mediante la llamada a stat
        fprintf(stderr, "Error al intentar acceder a %s: %s", name,strerror(errno));    //En caso de error, imprime un mensaje informando el nombre del archivo con problemas
        return false;
    }

    return true;        //Si se obtiene correctamente la información, retorna true
}

void print_file_info(char *name, struct stat *data, bool showLong, bool showLink, bool showAccessTime) {
    time_t t = data->st_atim.tv_sec;         //Almacenamos la hora en segundos desde 1970
    struct tm *tm_info = localtime(&t);
    char buffer[26];
    //Formatea la fecha y hora en una cadena
    strftime(buffer, 26, "%Y/%m/%d-%H:%M", tm_info);
    if (showLong) {
        char* permissions = GetPermissions(data->st_mode);       //Permisos
        //Propietario y grupo
        struct passwd *pw = getpwuid(data->st_uid);
        struct group *gr = getgrgid(data->st_gid);
        printf("%s   %ld (%ld) %s %s %s %9ld %s\n",buffer,data->st_nlink,data->st_ino,pw->pw_name,gr->gr_name,permissions,data->st_size,name);
        free(permissions);
        return;
    }if (showAccessTime) {
        printf("%ld %s %s\n",data->st_size,buffer,name);
    }if(showLink) {
        char link[LENGTH_MAX_PHRASE];
        ssize_t linklen = readlink(name,link,LENGTH_MAX_PHRASE);
        if(linklen == -1) {         //No tiene enlace simbólico
            printf("%10ld %s\n",data->st_size,name);
        }else {
            link[linklen] = '\0';  // Añadimos terminador nulo
            printf("%10ld %s -> %s\n",data->st_size,name,link);
        }
    }else
        printf("%10ld  %s\n",data->st_size,name);
}


// Unified function to list directories and files
void command_list(char *name, bool showLong, bool showLink, bool showAccessTime, bool showHidden, bool isDir) {
    char filePath[LENGTH_MAX_INPUT];       //Variable para almacenar la ruta completa del archivo (directorio + nombre del archivo)
    if (isDir) {
        DIR *dir;
        struct dirent *entry;
        // Abrir directorio
        dir = opendir(name);
        if (dir == NULL) {
            fprintf(stderr, "Error al intentar acceder a %s: %s \n", name,strerror(errno));
            return;
        }

        printf("************%s\n", name);

        // Iterar sobre los archivos del directorio
        while ((entry = readdir(dir)) != NULL) {
            struct stat data;
            if (GetFileData(showHidden, &data, name , entry->d_name,filePath)) {
                print_file_info(entry->d_name, &data, showLong,showLink,showAccessTime);  // Usar la función auxiliar
            }
        }
        closedir(dir);
    } else {
        struct stat data;
        if (lstat(name, &data) == -1) {
            fprintf(stderr, "Error al intentar acceder a %s: %s \n", name,strerror(errno));
            return;;
        }
        print_file_info(name, &data,showLong,showLink,showAccessTime);  // Usar la función auxiliar
    }
}


//Intepreta los argumentos y llama a listDirectoryRecursively cpn los parametros adecuados
void command_list_File_Dir(char *pieces[]) {
    bool showHidden = false;
    bool showLong = false; //si se debe mostrar información detallada
    bool showLink = false; //si se deben mostrar los enlaces simbólicos
    bool showAccessTime = false; //si se deben mostrar las fechas de acceso
    bool isDir = (strcmp(pieces[0], "listfile") == 0) ? false : true;
    int i = 1;

    //analiza las opciones
    for (; pieces[i] != NULL && pieces[i][0] == '-'; i++) {
        if (strcmp(pieces[i], "-hid") == 0)
            showHidden = true;
        else if (strcmp(pieces[i], "-long") == 0)
            showLong = true;
        else if (strcmp(pieces[i], "-link") == 0)
            showLink = true;
        else if (strcmp(pieces[i], "-acc") == 0)
            showAccessTime = true;
        else {
            fprintf(stderr, "Opción no reconocida: %s\n", pieces[i]);
            return;
        }
    }

    //Si no se especifican directorios imprimimos el directorio actual
    if (pieces[i] == NULL) {
        char cwd[LENGTH_MAX_INPUT];
        if (getcwd(cwd, LENGTH_MAX_INPUT) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("Error obteniendo el directorio actual");
        }
    }
    //Listar directorios recursivamente
    while (pieces[i] != NULL) {
        command_list(pieces[i], showLong, showLink, showAccessTime,showHidden,isDir);
        i++;
    }
}

static void listDirectoryRecursively(char *dirName, bool showHidden, bool showLong, bool showLink, bool showAccessTime) {
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;
    char fullPath[LENGTH_MAX_INPUT];

    //abre el directorio especificado por dirName
    dir = opendir(dirName);
    if (dir == NULL) {
        perror("Error al abrir el directorio");
        return;
    }
    bool isLink = !showLink && lstat(dirName, &fileStat) == 0 && S_ISLNK(fileStat.st_mode);
    if (!isLink) {
        printf("************%s\n", dirName);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (GetFileData(showHidden, &fileStat, dirName , entry->d_name,fullPath)) {
            print_file_info(entry->d_name, &fileStat, showLong,showLink,showAccessTime);  // Usar la función auxiliar
        }

        //recursividad para subdirectorios (omitir '.' y '..')
        if (S_ISDIR(fileStat.st_mode) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            listDirectoryRecursively(fullPath, showHidden, showLong, showLink, showAccessTime);
        }
    }

    //cierra el directorio
    closedir(dir);
}


//Se puede fusionar con la reversa porque son iguales, si tiempo hacerlo

//Intepreta los argumentos y llama a listDirectoryRecursively cpn los parametros adecuados
void command_reclist(char *pieces[]) {
    bool showHidden = false; //si se deben mostrar los archivos ocultos
    bool showLong = false; //si se debe mostrar información detallada
    bool showLink = false; //si se deben mostrar los enlaces simbólicos
    bool showAccessTime = false; //si se deben mostrar las fechas de acceso
    int i = 1;

    //analiza las opciones
    for (; pieces[i] != NULL && pieces[i][0] == '-'; i++) {
        if (strcmp(pieces[i], "-hid") == 0)
            showHidden = true;
        else if (strcmp(pieces[i], "-long") == 0)
            showLong = true;
        else if (strcmp(pieces[i], "-link") == 0)
            showLink = true;
        else if (strcmp(pieces[i], "-acc") == 0)
            showAccessTime = true;
        else {
            fprintf(stderr, "Opción %s no reconocida: %s\n", pieces[i],strerror(errno));
            return;
        }
    }

    //Si no se especifican directorios imprimimos el directorio actual, si te fijas en la de referencia hacen eso
    if (pieces[i] == NULL) {
        // Si no se especifica un directorio, imprime el directorio de trabajo actual
        char cwd[LENGTH_MAX_INPUT];
        if (getcwd(cwd, LENGTH_MAX_INPUT) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("Error obteniendo el directorio actual");
        }
    }
    //Listar directorios recursivamente
    while (pieces[i] != NULL) {
        listDirectoryRecursively(pieces[i], showHidden, showLong, showLink, showAccessTime);
        i++;
    }
}


static void listDirectoryRecursivelyReverse(char *dirName, bool showHidden, bool showLong, bool showLink, bool showAccessTime){
    DIR *dir;
    struct dirent *entry;
    //se abre el directorio especidifcado por dirName
    dir = opendir(dirName);
    if (dir == NULL) {
        fprintf(stderr, "Error al intentar acceder a %s: %s\n", dirName,strerror(errno));  // Mensaje de error
        return;
    }

    char fullPath[LENGTH_MAX_INPUT];
    struct stat fileData;

    while ((entry = readdir(dir)) != NULL) {
        if ((!showHidden && entry->d_name[0] == '.') || strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {       //No se les aplica ya que al leer este tipo de directorios e intentar acceder a los subdirectorios pueden provocar una recursión infinita al seguir llamando a la misma carpeta o a sus padres.
            continue;  // Omitir archivos ocultos
        }
        snprintf(fullPath, LENGTH_MAX_INPUT, "%s/%s", dirName, entry->d_name); //ruta completa del archivo
        if (stat(fullPath, &fileData) == -1) {
            fprintf(stderr, "Error al intentar acceder a %s: %s\n", entry->d_name,strerror(errno));  // Mensaje de error
            continue;
        }
        if (S_ISDIR(fileData.st_mode)) {
            listDirectoryRecursivelyReverse(fullPath, showHidden, showLong, showLink, showAccessTime);
        }
    }

    // Ahora que hemos recorrido recursivamente, volvemos a listar el contenido del directorio actual
    rewinddir(dir);  // Volver al inicio del directorio para la segunda pasada (evitar cerrar/abrir)

    bool isLink = !showLink && lstat(dirName, &fileData) == 0 && S_ISLNK(fileData.st_mode);
    if (!isLink) {
        printf("************%s\n", dirName);
    }



    while ((entry = readdir(dir)) != NULL) {
        if (GetFileData(showHidden, &fileData, dirName , entry->d_name,fullPath)) {
            print_file_info(entry->d_name, &fileData, showLong,showLink,showAccessTime);  // Usar la función auxiliar
        }
    }
    closedir(dir);  // Cerrar el directorio
}



void command_revlist(char *pieces[]) {
        bool showHidden = false; //si se deben mostrar los archivos ocultos
        bool showLong = false; //si se debe mostrar información detallada
        bool showLink = false; //si se deben mostrar los enlaces simbólicos
        bool showAccessTime = false; //si se deben mostrar las fechas de acceso
        int i = 1;

        //Analiza   las opciones
        for (; pieces[i] != NULL && pieces[i][0] == '-'; i++) {
            if (strcmp(pieces[i], "-hid") == 0)
                showHidden = true;
            else if (strcmp(pieces[i], "-long") == 0)
                showLong = true;
            else if (strcmp(pieces[i], "-link") == 0)
                showLink = true;
            else if (strcmp(pieces[i], "-acc") == 0)
                showAccessTime = true;
            else {
                fprintf(stderr, "Opción %s no reconocida: %s", pieces[i],strerror(errno));
                return;
            }
        }
        if (pieces[i] == NULL) {
            // Si no se especifica un directorio, imprime el directorio de trabajo actual
            char cwd[LENGTH_MAX_PATH];
            if (getcwd(cwd, LENGTH_MAX_PATH) != NULL) {
                printf("%s\n", cwd);
            } else {
                perror("Error obteniendo el directorio actual");
            }
        }
        //Listar directorios recursivamente
        while (pieces[i] != NULL) {
            listDirectoryRecursivelyReverse(pieces[i], showHidden, showLong, showLink, showAccessTime);
            i++;
        }
}

//Función que borra archivos o directorios vacíos
void command_erase(char *pieces[]) {
    struct stat fileStat;
    //Recorre todos los argumentos que se pasan al comando
    for (int i = 1; pieces[i] != NULL; i++) {  //Cambiado a verificar que pieces[i] no sea NULL
        //Comprueba si el archivo o directorio existe
        if (stat(pieces[i], &fileStat) == 0) {
            //Si es un fichero regular, lo borra
            if (S_ISREG(fileStat.st_mode)) {
                if (unlink(pieces[i]) == 0) {
                    printf("El fichero '%s' ha sido borrado.\n", pieces[i]);
                } else {
                    perror("Error al borrar el fichero");
                }
            }
            //Si es un directorio y está vacío, intenta borrarlo
            else if (S_ISDIR(fileStat.st_mode)) {
                DIR *dir = opendir(pieces[i]);
                if (dir == NULL) {
                    perror("Error al abrir el directorio");
                    continue;
                }

                struct dirent *entry;
                int isEmpty = 1;  //Asume que el directorio está vacío

                //Recorre el contenido del directorio para comprobar si está vacío
                while ((entry = readdir(dir)) != NULL) {
                    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                        isEmpty = 0;  //El directorio no está vacío
                        break;
                    }
                }
                closedir(dir);
                //Si el directorio está vacío, intenta eliminarlo
                if (isEmpty) {
                    if (rmdir(pieces[i]) == 0) {
                        printf("El directorio vacío '%s' ha sido borrado.\n", pieces[i]);
                    } else {
                        perror("Error al borrar el directorio");
                    }
                    //Si el directorio no está vacío, muestra un mensaje de error
                } else {
                    printf("Imposible borrar '%s' no está vacío y no puede ser borrado.\n", pieces[i]);
                }
                //Si no es un fichero regular ni un directorio, muestra un mensaje de error
            } else {
                printf("'%s' no es un fichero regular ni un directorio.\n", pieces[i]);
            }
        } else {
            //Si no existe el archivo o directorio, muestra un mensaje de error
            printf("El fichero o directorio '%s' no existe.\n", pieces[i]);
        }
    }
}

//Función auxiliar que verifica si una ruta corresponde a un directorio
static bool isDirectory(char *path) {
    struct stat st;         //Estructura para almacenar información sobre el archivo o directorio

    //Si stat falla (retorna -1), imprime un mensaje de error y retorna 'false'
    if (stat(path, &st) == -1) {
        //Imprimir un mensaje de error detallado en stderr indicando que hubo un problema con 'path'
        fprintf(stderr, "Error al intentar acceder a %s: %s\n", path, strerror(errno));
        return false;
    }
    //Retorna 'true' si el 'st_mode' indica que la ruta es un directorio, de lo contrario, 'false'
    return S_ISDIR(st.st_mode);
}

//delrec [name1 name2 ..]	Borra ficheros o directorios no vacios recursivamente

//Función auxiliar que elimina un archivo o directorio de forma recursiva
static bool deleteRecursively(char *name) {
    char fullPath[LENGTH_MAX_FULLPATH];  //Almacena la ruta completa del archivo o directorio

    //Determinar si la ruta es absoluta o relativa
    if (name[0] == '/') {
        //Ruta absoluta, simplemente copiamos el nombre a fullPath
        strncpy(fullPath, name, LENGTH_MAX_FULLPATH - 1);
        fullPath[LENGTH_MAX_FULLPATH - 1] = '\0';  //Aseguramos la terminación nula
    } else {
        //Ruta relativa, obtenemos el directorio actual y concatenamos con el nombre
        char cwd[LENGTH_MAX_PATH];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {  // Error al obtener el directorio actual
            fprintf(stderr, "Error al obtener el directorio actual: %s", strerror(errno));
            return false;
        }
        snprintf(fullPath, LENGTH_MAX_FULLPATH, "%s/%s", cwd, name);  // Concatenamos
    }

    //Si es un directorio, llamamos recursivamente para eliminar su contenido
    if (isDirectory(fullPath)) {
        DIR *dir = opendir(fullPath);  //Abre el directorio
        if (dir == NULL) {
            //Error al abrir el directorio
            fprintf(stderr, "Error al intentar acceder a %s: %s\n", fullPath, strerror(errno));
            return false;
        }
        struct dirent *entry;
        char subPath[LENGTH_MAX_INPUT];  //Almacena la ruta completa de los archivos en el directorio
        //Leemos el contenido del directorio
        while ((entry = readdir(dir)) != NULL) {
            //Omitimos las entradas especiales '.' y '..' (directorio actual y padre)
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            //Construimos la ruta completa del archivo o subdirectorio
            snprintf(subPath, LENGTH_MAX_INPUT, "%s/%s", fullPath, entry->d_name);
            //Llamada recursiva para eliminar el contenido del subdirectorio
            if (!deleteRecursively(subPath)) {
                closedir(dir);  //Cerramos el directorio en caso de error
                return false;
            }
        }
        closedir(dir);  //Cerramos el directorio después de recorrer su contenido
        //Eliminamos el propio directorio vacío
        if (rmdir(fullPath) == -1) {
            //Error al eliminar el directorio
            fprintf(stderr, "Error al eliminar directorio %s: %s\n", fullPath, strerror(errno));
            return false;
        }
    } else {
        // Si es un archivo regular o un enlace simbólico, lo eliminamos
        if (unlink(fullPath) == -1) {
            //Error al eliminar el archivo
            fprintf(stderr, "Error al eliminar el archivo %s: %s\n", name, strerror(errno));
            fflush(stdout);  //Vaciar el búfer de salida estándar
            return false;
        }
    }
    return true;  //Eliminación exitosa
}


//Función que elimina archivos o directorios no vacíos de forma recursiva
void command_delrec(char *pieces[]) {
    if (pieces[1] == NULL) {
        //Si no se especifica un directorio, imprime el directorio de trabajo actual
        char cwd[LENGTH_MAX_PHRASE];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            //Error al obtener el directorio actual
            fprintf(stderr, "Error al obtener el directorio actual: %s\n", strerror(errno));
        }
    } else {
        //Recorre la lista de directorios o archivos a eliminar
        for (int i = 1; pieces[i] != NULL; ++i) {
            //Llama a la función deleteRecursively para eliminar cada archivo o directorio
            if (deleteRecursively(pieces[i])) {
                printf("%s eliminado\n", pieces[i]);  //Mensaje de confirmación de eliminación
            }
        }
    }
}



//Convierte la cadena a un número hexadecimal
void *cadtop(char *cadena) {
    //strtoull de la biblioteca estándar de C para convertir la cadena hexadecimal a un número sin signo de 64 bits
    //(unsigned long long int).
    unsigned long long int direccion = strtoull(cadena, NULL, 16); //16 indica la base hexadecimal.
    //Luego, ese número se interpreta como una dirección de memoria y se convierte en un puntero.
    return (void *)direccion;
}

//función recursiva que muestra la dirección de su parámetro
void Recursiva (int n)
{
  char automatico[TAMANO];
  static char estatico[TAMANO];

  printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

  if (n>0)
    Recursiva(n-1);       //permite observar cómo cambia la pila de llamadas en cada recursión.
}

//Llena una región de memoria con un valor específico
void LlenarMemoria (void *p, size_t cont, unsigned char byte)
{
  unsigned char *arr=(unsigned char *) p;
  size_t i;

  for (i=0; i<cont;i++)
		arr[i]=byte;
}

//función asigna memoria compartida
void * ObtenerMemoriaShmget (key_t clave, size_t tam)
{
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;

    if (tam)     /*tam distito de 0 indica crear */
        flags=flags | IPC_CREAT | IPC_EXCL; /*cuando no es crear pasamos de tamano 0*/
    if (clave==IPC_PRIVATE)  /*no nos vale*/
        {errno=EINVAL; return NULL;}
    if ((id=shmget(clave, tam, flags))==-1)
        return (NULL);
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        if (tam)
             shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s); /* si no es crear, necesitamos el tamano, que es s.shm_segsz*/
 /* Guardar en la lista   InsertarNodoShared (&L, p, s.shm_segsz, clave); */
    return (p);
}

//función gestiona la asignación de memoria compartida
void do_AllocateCreateshared (char *pieces[])
{
    key_t cl;
    size_t tam;
    void *p;

    if (pieces[0]==NULL || pieces[1]==NULL) {
        printMemoryBlockList(memoryBlockList);
        return;
    }

    cl=(key_t)  strtoul(pieces[0],NULL,10);
    tam=(size_t) strtoul(pieces[1],NULL,10);
    if (tam==0) {
        printf ("No se asignan bloques de 0 bytes\n");
        return;
    }
    // Obtener la memoria compartida
    if ((p = ObtenerMemoriaShmget(cl, tam)) != NULL) {
        printf("Asignados %lu bytes en %p\n", (unsigned long)tam, p);

        // Insertar en la lista de bloques de memoria
        if (!insertMemoryBlockB(&memoryBlockList, p, tam, SHARED_MEMORY, cl, NULL, -1)) {
            fprintf(stderr, "Error al insertar el bloque de memoria compartida en la lista\n");
        }
    } else {
        printf("Imposible asignar memoria compartida clave %lu:%s\n", (unsigned long)cl, strerror(errno));
    }
}

//función intenta asignar un segmento de memoria compartida existente
void do_AllocateShared (char *pieces[])
{
   key_t cl;
   size_t tam;
   void *p;

   if (pieces[0]==NULL) {
		printMemoryBlockList(memoryBlockList);
		return;
   }

   cl=(key_t)  strtoul(pieces[0],NULL,10);tam=(size_t) strtoul(pieces[1],NULL,10);

    // Intentar obtener la memoria compartida
    if ((p = ObtenerMemoriaShmget(cl, 0)) != NULL) {
        printf("Asignada memoria compartida de clave %lu en %p\n", (unsigned long)cl, p);

        // Insertar en la lista de bloques de memoria
        if (!insertMemoryBlockB(&memoryBlockList, p, tam, SHARED_MEMORY, cl, NULL, -1)) {
            fprintf(stderr, "Error al insertar el bloque de memoria compartida en la lista\n");
        }
    } else {
        printf("Imposible asignar memoria compartida clave %lu:%s\n", (unsigned long)cl, strerror(errno));
    }
}

//función mapea un archivo en la memoria del proceso usando mmap
void * MapearFichero (char * fichero, int protection)\
{
    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;

    if (protection&PROT_WRITE)
          modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
          return NULL;
    if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
           return NULL;
/* Guardar en la lista    InsertarNodoMmap (&L,p, s.st_size,df,fichero); */
/* Gurdas en la lista de descriptores usados df, fichero*/
    return p;
}

//función llama a MapearFichero para mapear un archivo
void do_AllocateMmap(char *pieces[])
{
     char *perm;
     void *p;
     int protection=0;
    size_t tam;

     if (pieces[0]==NULL)
            {printMemoryBlockList(memoryBlockList); return;}
     if ((perm=pieces[1])!=NULL && strlen(perm)<4) {
            if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
            if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
            if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
     }
    // Mapear el archivo
    if ((p = MapearFichero(pieces[0], protection)) == NULL) {
        perror("Imposible mapear fichero");
    } else {
        printf("Fichero %s mapeado en %p\n", pieces[0], p);

        tam=(size_t) strtoul(pieces[1],NULL,10);

        // Insertar en la lista de bloques de memoria
        if (!insertMemoryBlockB(&memoryBlockList, p, tam, MAPPED_FILE, -1, pieces[0], -1)) {
            fprintf(stderr, "Error al insertar el bloque de memoria mapeada en la lista\n");
        }
    }
}

//función elimina un segmento de memoria compartida según su clave
void do_DeallocateDelkey (char *pieces[])
{
   key_t clave;
   int id;
   char *key=pieces[0];

   if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf ("      delkey necesita clave_valida\n");
        return;
   }
   if ((id=shmget(clave,0,0666))==-1){
        perror ("shmget: imposible obtener memoria compartida");
        return;
   }
   if (shmctl(id,IPC_RMID,NULL)==-1)
        perror ("shmctl: imposible eliminar memoria compartida\n");
}

//Lee un archivo en una región de memoria específica
ssize_t LeerFichero (char *f, void *p, size_t cont)
{
   struct stat s;
   ssize_t  n;
   int df,aux;

   if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
	return -1;
   if (cont==-1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
	cont=s.st_size;
   if ((n=read(df,p,cont))==-1){
	aux=errno;
	close(df);
	errno=aux;
	return -1;
   }
   close (df);
   return n;
}

//función lee datos de un archivo y los coloca en una dirección de memoria específica
void Cmd_ReadFile (char *pieces[])
{
   void *p;
   size_t cont=-1;  /*si no pasamos tamano se lee entero */
   ssize_t n;
   if (pieces[0]==NULL || pieces[1]==NULL){
	printf ("faltan parametros\n");
	return;
   }
   p=cadtop(pieces[1]);  /*convertimos de cadena a puntero*/
   if (pieces[2]!=NULL)
	cont=(size_t) atoll(pieces[2]);

   if ((n=LeerFichero(pieces[0],p,cont))==-1)
	perror ("Imposible leer fichero");
   else
	printf ("leidos %lld bytes de %s en %p\n",(long long) n,pieces[0],p);
}

//Ejecuta un comando equivalente a pmap para ver el mapa de memoria del proceso actual
void Do_pmap (void) /*sin argumentos*/
 { pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
   char elpid[32];
   char *argv[4]={"pmap",elpid,NULL};

   sprintf (elpid,"%d", (int) getpid());
   if ((pid=fork())==-1){
      perror ("Imposible crear proceso");
      return;
      }
   if (pid==0){
      if (execvp(argv[0],argv)==-1)
         perror("cannot execute pmap (linux, solaris)");

      argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;
      if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
         perror("cannot execute procstat (FreeBSD)");

      argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;
            if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
         perror("cannot execute procmap (OpenBSD)");

      argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
      if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
         perror("cannot execute vmmap (Mac-OS)");
      exit(1);
  }
    waitpid(pid,NULL,0);
}

//Es un gestor de asignación de memoria que recibe argumentos y decide qué tipo de asignación realizar
void command_allocate(char *pieces[], MemoryBlockList *memoryBlockList) {
    if (pieces[1] == NULL) {
        fprintf(stderr, "Error: Faltan argumentos para el comando allocate\n");
        return;
    }

    if (strcmp(pieces[1], "-malloc") == 0) {
        if (pieces[2] == NULL) {
            fprintf(stderr, "Error: Falta el tamaño para malloc\n");
            return;
        }
        size_t n = (size_t) strtoul(pieces[2], NULL, 10);
        void *address = malloc(n);
        if (address == NULL) {
            perror("Error al asignar memoria con malloc");
            return;
        }

        if (!insertMemoryBlockB(memoryBlockList, address, n, MALLOC_MEMORY, -1, NULL, -1)) {  //inserta en la lista
            fprintf(stderr, "Error al insertar el bloque malloc en la lista de memoria\n");
            free(address);
        } else {
            printf("Asignados %zu bytes usando malloc en la dirección %p\n", n, address);
        }

    } else if (strcmp(pieces[1], "-mmap") == 0) {
        do_AllocateMmap(pieces + 2);

    } else if (strcmp(pieces[1], "-create") == 0 && strcmp(pieces[2], "shared") == 0) {
        do_AllocateCreateshared(pieces + 3);

    } else if (strcmp(pieces[1], "-shared") == 0) {
        do_AllocateShared(pieces + 2);

    } else {
        fprintf(stderr, "Opción desconocida para allocate\n");
    }
}
void command_deallocate(){}

//Llena una región de memoria con un byte específico
void command_memfill(char *pieces[]) {
    if (pieces[1] == NULL || pieces[2] == NULL || pieces[3] == NULL) {
        fprintf(stderr, "Error: Faltan argumentos para el comando memfill\n");
        fprintf(stderr, "Uso: memfill <addr> <cont> <ch>\n");
        return;
    }

    void *addr = cadtop(pieces[1]);  // Convertir la dirección de memoria de cadena a puntero
    size_t cont = (size_t) strtoul(pieces[2], NULL, 10);  // Convertir el tamaño de la memoria
    unsigned char ch;

    if (isdigit(pieces[3][0])) {  // Verificar si el tercer argumento es un número o un carácter
        ch = (unsigned char) strtoul(pieces[3], NULL, 10); // Convertir el número a byte
    } else if (pieces[3][0] == '\'' && pieces[3][1] != '\0' && pieces[3][2] == '\'' && pieces[3][3] == '\0') {
        ch = pieces[3][1]; // Convertir el carácter entre comillas
    } else {
        fprintf(stderr, "Error: Formato de byte no válido (debe ser un número o un carácter entre comillas)\n");
        return;
    }

    LlenarMemoria(addr, cont, ch);  //Llenar la memoria con el carácter 'ch' para los 'cont' bytes
    printf("Memoria llenada con el carácter '%c' en la dirección %p para %zu bytes\n", ch, addr, cont);
}
void command_memdump(){}

//Print direcciones
void command_memory(char *pieces[]) {
    if (pieces[1] == NULL) {
        fprintf(stderr, "Error: Faltan argumentos para el comando memory\n");
        return;
    }

    if (strcmp(pieces[1], "-funcs") == 0) {
        // Imprimir direcciones de funciones del programa y de la biblioteca
        printf("Dirección de 3 funciones del programa:\n");
        printf("command_memory: %p\n", (void *)command_memory);
        printf("command_allocate: %p\n", (void *)command_allocate);
        printf("command_deallocate: %p\n", (void *)command_deallocate);
        printf("Dirección de 3 funciones de la biblioteca:\n");
        printf("printf: %p\n", (void *)printf);
        printf("malloc: %p\n", (void *)malloc);
        printf("free: %p\n", (void *)free);

    } else if (strcmp(pieces[1], "-vars") == 0) {
        // Variables externas y estáticas
        static int static_var1, static_var2, static_var3;
        static int static_init_var1 = 1, static_init_var2 = 2, static_init_var3 = 3;
        int auto_var1, auto_var2, auto_var3;

        printf("Dirección de 3 variables externas:\n");
        printf("ext_var1: %p\n", (void *)&ext_var1);
        printf("ext_var2: %p\n", (void *)&ext_var2);
        printf("ext_var3: %p\n", (void *)&ext_var3);

        printf("Dirección de 3 variables externas inicializadas:\n");
        printf("ext_init_var1: %p\n", (void *)&ext_init_var1);
        printf("ext_init_var2: %p\n", (void *)&ext_init_var2);
        printf("ext_init_var3: %p\n", (void *)&ext_init_var3);

        printf("Dirección de 3 variables estáticas:\n");
        printf("static_var1: %p\n", (void *)&static_var1);
        printf("static_var2: %p\n", (void *)&static_var2);
        printf("static_var3: %p\n", (void *)&static_var3);

        printf("Dirección de 3 variables estáticas inicializadas:\n");
        printf("static_init_var1: %p\n", (void *)&static_init_var1);
        printf("static_init_var2: %p\n", (void *)&static_init_var2);
        printf("static_init_var3: %p\n", (void *)&static_init_var3);

        printf("Dirección de 3 variables automáticas:\n");
        printf("auto_var1: %p\n", (void *)&auto_var1);
        printf("auto_var2: %p\n", (void *)&auto_var2);
        printf("auto_var3: %p\n", (void *)&auto_var3);

    } else if (strcmp(pieces[1], "-blocks") == 0) {
        // Imprimir la lista de bloques de memoria
        printf("****** Lista de bloques asignados para el proceso %d ******\n", getpid());
        printMemoryBlockList(memoryBlockList);

    } else if (strcmp(pieces[1], "-all") == 0) {
        // Mostrar todo (funciones, variables, bloques)
        command_memory((char *[]){"memory", "-funcs", NULL});
        command_memory((char *[]){"memory", "-vars", NULL});
        command_memory((char *[]){"memory", "-blocks", NULL});

    } else if (strcmp(pieces[1], "-pmap") == 0) {
        // Llamar a la función que muestra el mapa de memoria
        Do_pmap();

    } else {
        fprintf(stderr, "Opción desconocida para memory\n");
    }
}

void command_readfile(){}

//Escribe en un archivo desde una dirección de memoria específica
void command_writefile(char *pieces[]) {
    char *filename;
    void *addr;
    size_t cont;
    ssize_t written;
    int fd;

    if (pieces[1] == NULL || pieces[2] == NULL || pieces[3] == NULL) {
        fprintf(stderr, "Error: Faltan argumentos para el comando writefile\n");
        fprintf(stderr, "Uso: writefile <filename> <addr> <cont>\n");
        return;
    }

    // Verificar si el primer argumento es "-o" para sobrescribir
    if (strcmp(pieces[1], "-o") == 0) {
        filename = pieces[2];
        addr = (void *)strtoull(pieces[3], NULL, 16);
        cont = (size_t)strtoull(pieces[4], NULL, 10);
        fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0666);  // Sobrescribir el archivo
    } else {
        filename = pieces[1];
        addr = (void *)strtoull(pieces[2], NULL, 16);
        cont = (size_t)strtoull(pieces[3], NULL, 10);
        fd = open(filename, O_WRONLY | O_CREAT, 0666);  // No sobrescribir si no se pasa -o
    }

    if (fd == -1) {
        perror("Error al abrir el archivo");
        return;
    }

    written = write(fd, addr, cont);  // Escribir en el archivo desde la memoria
    if (written == -1) {
        perror("Error al escribir en el archivo");
        close(fd);
        return;
    } else if ((size_t)written != cont) {
        fprintf(stderr, "Advertencia: No se pudieron escribir todos los bytes\n");
    }

    printf("Escribió %zd bytes en el archivo %s desde la dirección de memoria %p\n", written, filename, addr);

    close(fd);
}
void command_read(){}

//Lo mismo que writefile pero usa un descriptor de fichero ya abierto.
void command_write(char *pieces[]) {
    int fd;
    void *addr;
    size_t cont;
    ssize_t written;
    errno = 0;

    // Verificar que se proporcionen los tres argumentos necesarios
    if (pieces[1] == NULL || pieces[2] == NULL || pieces[3] == NULL) {
        fprintf(stderr, "Error: Faltan argumentos para el comando write\n");
        fprintf(stderr, "Uso: write <file_descriptor> <addr> <cont>\n");
        return;
    }

    // Convertir el descriptor de archivo de cadena a entero
    fd = strtol(pieces[1], NULL, 10);
    if (fd <= 0) {   // Verificar si el descriptor de archivo es válido
        fprintf(stderr, "Error: Descriptor de archivo inválido\n");
        return;
    }

    // Convertir addr de cadena hexadecimal a puntero
    addr = (void *)strtoull(pieces[2], NULL, 16);
    if (addr == NULL) {
        fprintf(stderr, "Error: Dirección de memoria no válida\n");
        return;
    }

    // Convertir cont de cadena a tamaño entero
    cont = (size_t)strtoull(pieces[3], NULL, 10);

    // Escribir en el archivo desde la memoria
    written = write(fd, addr, cont);
    if (written == -1) {
        perror("Error al escribir en el archivo");
        return;
    } else if ((size_t)written != cont) {
        // Advertencia si no se escriben todos los bytes solicitados
        fprintf(stderr, "Advertencia: No se escribieron todos los bytes solicitados (%zu bytes escritos de %zu solicitados)\n", written, cont);
    }

    // Mensaje de confirmación
    printf("Escribió %zd bytes en el archivo desde la dirección de memoria %p\n", written, addr);
}
void command_recurse(){}