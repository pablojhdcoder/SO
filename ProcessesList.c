#include "ProcessesList.h"

void createEmptyListP(ProcessList *P) {
    *P = PNULL;
}

bool isEmptyListP(ProcessList P) {
    return P == PNULL;
}

bool createNodeP(tPosP *p) {
    *p = malloc(sizeof(struct tNodeP));
    return *p != PNULL;
}

tPosP firstP(ProcessList P) {
    return P;
}

tPosP nextP(tPosP p) {
    return p->next;
}

tPosP lastPosP(ProcessList P) {
    tPosP p;
    if (P == PNULL) {
        return PNULL;
    }
    for (p = P; p->next != PNULL; p = p->next);
    return p;
}

void getFormattedTimeP(char *buffer, size_t bufferSize) {
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", localTime);
}

bool addProcess(ProcessList *P, pid_t pid, const char *commandLine) {
    tPosP p, q;
    if (!createNodeP(&p)) {
        return false;
    }
    p->data.pid = pid;
    getFormattedTimeP(p->data.launchTime, sizeof(p->data.launchTime));
    p->data.status = ACTIVE;
    p->data.returnValue = 0;
    strncpy(p->data.commandLine, commandLine, sizeof(p->data.commandLine) - 1);
    p->data.commandLine[sizeof(p->data.commandLine) - 1] = '\0';
    p->next = PNULL;

    if (*P == PNULL) {
        *P = p;
    } else {
        q = lastPosP(*P);
        q->next = p;
    }
    return true;
}

void removeProcess(ProcessList *P, tPosP pos) {
    if (!isEmptyListP(*P)) {
        tPosP q;
        if (pos->data.status == ACTIVE) {
            kill(pos->data.pid, SIGTERM); // Enviar señal de terminación
            int status;
            waitpid(pos->data.pid, &status, 0); // Esperar a que termine
        }
        if (pos == *P) {
            *P = (*P)->next;
        } else if (pos->next == PNULL) {
            for (q = *P; q->next != pos; q = q->next);
            q->next = PNULL;
        } else {
            q = pos->next;
            pos->data = q->data;
            pos->next = q->next;
            pos = q;
        }
        free(pos);
    } else {
        printf("La lista de procesos está vacía\n");
    }
}

void updateProcessStatus(ProcessList *P) {
    tPosP current = *P;
    int status;
    while (current != PNULL) {
        pid_t result = waitpid(current->data.pid, &status, WNOHANG);
        if (result == 0) {
            current = current->next;
            continue;
        }
        if (result == -1) {
            perror("waitpid");
            current = current->next;
            continue;
        }
        if (WIFEXITED(status)) {
            current->data.status = FINISHED;
            current->data.returnValue = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            current->data.status = SIGNALED;
            current->data.returnValue = WTERMSIG(status);
        } else if (WIFSTOPPED(status)) {
            current->data.status = STOPPED;
            current->data.returnValue = WSTOPSIG(status);
        }
        current = current->next;
    }
}

//Detalles de los procesos en el background
void listJobs(ProcessList P) {
    updateProcessStatus(&P);
    tPosP current = P;
    while (current != PNULL) {
        printf("PID: %d\n", current->data.pid);
        printf("Launch Time: %s\n", current->data.launchTime);
        printf("Status: ");
        switch (current->data.status) {
            case ACTIVE:
                printf("ACTIVE\n");
                break;
            case FINISHED:
                printf("FINISHED (Return Value: %d)\n", current->data.returnValue);
                break;
            case STOPPED:
                printf("STOPPED (Signal: %d)\n", current->data.returnValue);
                break;
            case SIGNALED:
                printf("SIGNALED (Signal: %d)\n", current->data.returnValue);
                break;
        }
        printf("Command Line: %s\n", current->data.commandLine);
        printf("Priority: %d\n", getpriority(PRIO_PROCESS, current->data.pid));
        printf("\n");
        current = current->next;
    }
}

//Remove los procesos terminados o signaled
void delJobs(ProcessList *P) {
    if (P == NULL || *P == PNULL) {
        printf("Lista de procesos vacía\n");
    }
    tPosP current = *P, prev = PNULL;
    while (current != PNULL) {
        if (current->data.status == FINISHED || current->data.status == SIGNALED) {
            if (prev == PNULL) {  //nodo inicial
                *P = current->next;
            } else {  //nodo intermedio
                prev->next = current->next;
            }
            tPosP temp = current;
            current = current->next;
            free(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void CleanProcessList(ProcessList *P) {
    tPosP current = *P;
    tPosP temp = PNULL;
    while (current != PNULL) {
        if (current->data.status == ACTIVE) {
            kill(current->data.pid, SIGTERM); // Enviar señal de terminación
            int status;
            waitpid(current->data.pid, &status, 0); // Esperar a que termine
        }
        temp = current;
        current = current->next;
        free(temp);
    }
}