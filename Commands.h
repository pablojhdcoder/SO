#ifndef COMMANDS_H
#define COMMANDS_H

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
void command_historic (char *pieces[],bool *finished,CommandListC *commandList, HistoryList *history, OpenFileList *openFileList, MemoryBlockList *memoryBlockList);
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
void command_memdump();
void command_memory(char *pieces[]);
void command_readfile();
void command_writefile(char *pieces[]);
void command_read();
void command_write(char *pieces[]);
void command_recurse();


#endif //COMMANDS_H