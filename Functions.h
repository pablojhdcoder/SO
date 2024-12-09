#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Commands.h"


void printPrompt();
void InitializateShellLists (CommandListC *c, HistoryList *h, OpenFileList *f, MemoryBlockList *b, ProcessList *p, DirectoryList *d);
void readInput(char *env[], bool *finished, CommandListC *commandList, HistoryList *history, OpenFileList *openFileList, MemoryBlockList *memoryBlockList, ProcessList *processList, DirectoryList *directoryList);
void processInput(bool *finished,tItemH *str, char *pieces[], char *envp[], CommandListC *commandList, HistoryList *history,OpenFileList *fileList, MemoryBlockList *memoryBlockList, ProcessList *processList, DirectoryList *directoryList);
void InsertPredefinedCommands(CommandListC *commandList);
int SplitString(char *str, char *pieces[]);






#endif //FUNCTIONS_H