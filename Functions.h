#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Commands.h"


void printPrompt();
void InitializateShellLists (CommandListC *c, HistoryList *h, OpenFileList *f, MemoryBlockList *b, ProcessList *p);
void readInput(bool *finished, CommandListC *commandList, HistoryList *history, OpenFileList *openFileList, MemoryBlockList *memoryBlockList, ProcessList *processList);
void processInput(bool *finished,tItemH *str,char *pieces[], CommandListC *commandList, HistoryList *history,OpenFileList *fileList, MemoryBlockList *memory_block_list, ProcessList *processList);
void InsertPredefinedCommands(CommandListC *commandList);
int SplitString(char *str, char *pieces[]);






#endif //FUNCTIONS_H