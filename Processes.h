#ifndef PROCESSES_H
#define PROCESSES_H

#include "Commands.h"

#define PNULL NULL

typedef enum {
    ACTIVE,
    FINISHED,
    STOPPED,
    SIGNALED
} ProcessStatus;

typedef struct {
    pid_t pid;
    char launchTime[20];
    ProcessStatus status;
    int returnValue;
    char commandLine[256];
} ProcessInfo;

typedef struct tNodeP* tPosP;
typedef tPosP ProcessList;

struct tNodeP {
    ProcessInfo data;
    tPosP next;
};

void createEmptyListP(ProcessList *P);
bool isEmptyListP(ProcessList P);
bool createNode(tPosP *p);
tPosP firstP(ProcessList P);
tPosP next(ProcessList P);
tPosP lastPosP(ProcessList P);
bool addProcess(ProcessList *P, pid_t pid, const char *commandLine);
void removeProcess(ProcessList *P, tPosP pos);
void updateProcessStatus(ProcessList *P);
void listJobs(ProcessList P);
void delJobs(ProcessList *P);
#endif //PROCESSES_H
