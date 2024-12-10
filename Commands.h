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
#include <sys/resource.h> // Para getpriority()

#include "CommandList.h"
#include "HistoryList.h"
#include "FileList.h"
#include "MemoryBlockList.h"
#include "ProcessesList.h"
#include "DirectoryList.h"
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
void command_historic (char *pieces[],bool *finished,CommandListC *commandList, HistoryList *history, OpenFileList *openFileList, MemoryBlockList *memoryBlockList, ProcessList *processList, DirectoryList *directoryList);
void command_open(char *pieces[],OpenFileList *openFileList);
void command_close(char *pieces[],OpenFileList *openFileList);
void command_dup(char *pieces[], OpenFileList *openFileList);
void command_infosys();
void command_help(char * pieces[], CommandListC *commandList);
void command_exit(bool *finished,OpenFileList *openFileList, HistoryList *history, CommandListC *commandList, MemoryBlockList *memoryBlockList, DirectoryList *directoryList, ProcessList *processList);

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
void command_showvar(char *pieces[], char *env[]);
void command_changevar(char *pieces[], char *env[]);
void command_subsvar(char *pieces[], char *env[]);
void command_environ(char *pieces[], char *envp[]);
void command_fork(ProcessList *P);
void command_search(char *pieces[], DirectoryList *directoryList);
void command_exec(char *pieces[], DirectoryList *directoryList, char *env[]);
void command_execpri(char *pieces[], DirectoryList *directorylist, char *env[]);
void command_fg(char *pieces[], char *env[],  DirectoryList *directoryList);
void command_fgpri(char *pieces[], char *env[], DirectoryList *directoryList);
void command_back(char *pieces[], char *env[], DirectoryList *directoryList, ProcessList *processList);
void command_backpri(char *pieces[], char *env[], DirectoryList *directoryList, ProcessList *processList);
void command_listjobs(ProcessList *P);
void command_deljobs(ProcessList *P);

void executeExternalCommand(char *pieces[], char *env[], DirectoryList *directoryList);


#endif //COMMANDS_H