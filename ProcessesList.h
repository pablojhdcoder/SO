#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>  // Para waitpid()
#include <ctype.h>   // Para isdigit()
#include <sys/resource.h> // Para getpriority()
#include <pwd.h>
#include <unistd.h>

#define PNULL NULL

typedef enum {
    ACTIVE,
    FINISHED,
    STOPPED,
    SIGNALED
} ProcessStatus;

typedef struct {
    pid_t pid;
    char owner[32];
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
tPosP firstP(ProcessList P);
tPosP nextP(ProcessList P);
tPosP lastPosP(ProcessList P);
bool addProcess(ProcessList *P, pid_t pid, const char *commandLine);
void removeProcess(ProcessList *P, tPosP pos);
void updateProcessStatus(ProcessList *P);
void listJobs(ProcessList P);
void delJobs(ProcessList *P);
void CleanProcessList(ProcessList *P);

#endif //PROCESSES_H