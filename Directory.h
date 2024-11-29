#ifndef DIRECTORY_H
#define DIRECTORY_H

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
bool addDirectory(DirectoryList *D, const char *directory);
void removeDirectory(DirectoryList *D, tPosD pos);
char* SearchListFirst(DirectoryList D);
char* SearchListNext(tPosD *p);

#endif //DIRECTORY_H
