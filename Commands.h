#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>     // Para open()
#include <sys/types.h> // Para definir tipos de datos como mode_t
#include <sys/stat.h>  // Para definir permisos de archivo
#include <sys/utsname.h> // Para obtener información del sistema
#include <unistd.h>  // Para close()
#include <pwd.h>        // Para obtener nombre de usuario
#include <grp.h>        // Para obtener nombre del grupo
#include <dirent.h> //Para obtener información del contenido de un directorio
#include <sys/mman.h>  // Para mmap()
#include <sys/shm.h>  // Para shmget(), shmat(), shmdt(), shmctl()
#include <sys/wait.h>  // Para waitpid()
#include <ctype.h>   // Para isdigit()

#include "CommandList.h"
#include "HistoryList.h"
#include "FileList.h"
#include "MemoryBlockList.h"
#include "Processes.h"
#include "Functions.h"

#define LENGTH_MAX_INPUT 512  //Longitud máxima de la entrada del usuario
#define LENGTH_MAX_OUTPUT 256 //Longitud máxima de la salida
#define LENGTH_MAX_PHRASE 128 //Longitud máxima de la frase
#define LENGTH_MAX_PATH 128  //Longitud máxima de la ruta
#define LENGTH_MAX_FULLPATH 256  //Longitud máxima de la ruta completa
#define TAMANO 2048


void command_authors(char * pieces[]);
void command_pid();
void command_ppid();
void command_cd(char *pieces[]);
void command_date(char *pieces[]);
void command_historic (char *pieces[],bool *finished,CommandListC *commandList, HistoryList *history, OpenFileList *openFileList, MemoryBlockList *memoryBlockList, ProcessList *processList);
void command_open(char *pieces[],OpenFileList *openFileList);
void command_close(char *pieces[],OpenFileList *openFileList);
void command_dup(char *pieces[], OpenFileList *openFileList);
void command_infosys();
void command_help(char * pieces[], CommandListC *commandList);
void command_exit(bool *finished,OpenFileList *openFileList, HistoryList *history, CommandListC *commandList, MemoryBlockList *memoryBlockList);

void command_makefile(char *pieces[]);
void command_makedir(char *pieces[]);
void command_list_File_Dir(char *pieces[]);
void command_cwd();
void command_reclist(char *pieces[]);
void command_revlist(char *pieces[]);
void command_delrec (char *pieces[]);
void command_erase(char *pieces[]);

void command_allocate(char *pieces[], MemoryBlockList *memoryBlockList, OpenFileList *openFileList);
void command_deallocate(char *pieces[],MemoryBlockList *memblocks);
void command_memfill(char *pieces[]);
void command_memdump(char *pieces[]);
void command_memory(char *pieces[], MemoryBlockList memoryBlockList);
void command_readfile(char *ar[]);
void command_writefile(char *pieces[]);
void command_read(char *ar[], OpenFileList *file_list);
void command_write(char *pieces[]);
void command_recurse(char *pieces[]);

void command_getuid();
void command_setuid(char *pieces[]);



#endif //COMMANDS_H