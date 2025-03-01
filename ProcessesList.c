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
    strftime(buffer, bufferSize, "%Y/%m/%d %H:%M:%S", localTime);
}

bool addProcess(ProcessList *P, pid_t pid, const char *commandLine) {
    struct passwd *pw = getpwuid(getuid()); // Obtén el nombre del usuario actual.
    tPosP p, q;
    if (!createNodeP(&p)) {
        return false;
    }
    p->data.pid = pid;
    strncpy(p->data.owner, pw->pw_name, sizeof(p->data.owner) - 1);
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
        pid_t result = waitpid(current->data.pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
        if (result == 0) {
            current = current->next;
            continue;
        }
        if (result == -1) {
            current = current->next;
            continue;
        }
        if (result == current->data.pid) {
            if (WIFEXITED(status)) {
                current->data.status = FINISHED;
                current->data.returnValue = WEXITSTATUS(status);
            } else if (WIFSIGNALED(status)) {
                current->data.status = SIGNALED;
                current->data.returnValue = WTERMSIG(status);
            } else if (WIFSTOPPED(status)) {
                current->data.status = STOPPED;
                current->data.returnValue = WSTOPSIG(status);
            } else if (WIFCONTINUED(status)) {
                current->data.status = ACTIVE;
                current->data.returnValue = 0;
            }
            current = current->next;
        }
    }
}

//Detalles de los procesos en el background
void listJobs(ProcessList P) {
    tPosP current = P;
    char *statusStr;
    // Actualizar el estado de los procesos antes de imprimir
    updateProcessStatus(&P);

    while (current != PNULL) {
        // Obtener estado como cadena
        switch (current->data.status) {
            case FINISHED:
                statusStr = "TERMINADO";
                break;
            case ACTIVE:
                statusStr = "ACTIVO";
                break;
            case STOPPED:
                statusStr = "DETENIDO";
                break;
            case SIGNALED:
                statusStr = "SEÑALADO";
                break;
            default:
                statusStr = "DESCONOCIDO";
        }

        // Imprimir la información del proceso
        printf("%6d %10s p=%d %s %s (%03d) %s\n",
               current->data.pid,                   // PID del proceso
               current->data.owner,                // Propietario del proceso
               getpriority(PRIO_PROCESS, current->data.pid),             // Prioridad del proceso
               current->data.launchTime,                         // Fecha y hora de inicio
               statusStr,                          // Estado del proceso
               current->data.returnValue,          // Valor de retorno o señal
               current->data.commandLine);         // Línea de comandos

        current = current->next;
    }
}

//Remove los procesos terminados o signaled
void delJobs(ProcessList *P) {
    updateProcessStatus(P);
    tPosP current = *P, prev = PNULL;
    while (current != PNULL) {
        if (current->data.status == FINISHED || current->data.status == SIGNALED) {
            if (prev == PNULL) {
                *P = current->next;
            } else {
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