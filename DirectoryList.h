#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DNULL NULL

typedef struct tNodeD* tPosD;
typedef tPosD DirectoryList;

struct tNodeD {
    char directory[256];
    tPosD next;
};

void createEmptyListD(DirectoryList *D);
bool isEmptyListD(DirectoryList D);
bool createNodeD(tPosD *p);
tPosD firstD(DirectoryList D);
tPosD nextD(tPosD p);
tPosD lastPosD(DirectoryList D);
char* GetDirectory(tPosD p);
bool addDirectoryD(DirectoryList *D, const char *directory);
void removeDirectoryD(DirectoryList *D, tPosD pos);
char* SearchListFirstD(DirectoryList D);
tPosD SearchDirectoryD(char *p, DirectoryList D);
void ListDirectoryList(DirectoryList D);
void cleanDirectoryList(DirectoryList *D);

#endif //DIRECTORY_H