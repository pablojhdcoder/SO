/*
 * TITLE: Sistemas Operativos
 * SUBTITLE: Practica 1
 * AUTHOR 1: Pablo Herrero Diaz LOGIN 1: pablo.herrero.diaz
 * AUTHOR 2: Tiago Da Costa Teixeira Veloso E Volta LOGIN 2: tiago.velosoevolta
 * GROUP: 2.3
 * DATE: 25 / 10 / 24
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

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

#include "CommandList.h"
#include "HistoryList.h"
#include "FileList.h"
#include "Commands.h"

void printPrompt();
void InitializateShellLists (CommandListC *c, HistoryList *h, OpenFileList *f);
void readInput(bool *finished, CommandListC *commandList, HistoryList *history,OpenFileList *openFileList);
void processInput(bool *finished,tItemH *str,char *pieces[], CommandListC *commandList, HistoryList *history,OpenFileList *fileList);
void InsertPredefinedCommands(CommandListC *commandList);
int SplitString(char *str, char *pieces[]);






#endif //FUNCTIONS_H
