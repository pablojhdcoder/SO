#include "DirectoryList.h"

void createEmptyListD(DirectoryList *D) {
    *D = DNULL;
}

bool isEmptyListD(DirectoryList D) {
    return D == DNULL;
}

bool createNodeD(tPosD *p) {
    *p = malloc(sizeof(struct tNodeD));
    return *p != DNULL;
}

tPosD firstD(DirectoryList D) {
    return D;
}

tPosD nextD(tPosD p) {
    return p->next;
}

tPosD lastPosD(DirectoryList D) {
    tPosD p;
    if (D == DNULL) {
        return DNULL;
    }
    for (p = D; p->next != DNULL; p = p->next);
    return p;
}

char* GetDirectory(tPosD p) {
    return p->directory;
}

bool addDirectoryD(DirectoryList *D, const char *directory) {
    tPosD p, q;
    if (!createNodeD(&p)) {
        return false;
    }
    strncpy(p->directory, directory, sizeof(p->directory) - 1);
    p->directory[sizeof(p->directory) - 1] = '\0';
    p->next = DNULL;

    if (*D == DNULL) {
        *D = p;
    } else {
        q = lastPosD(*D);
        q->next = p;
    }
    return true;
}

void removeDirectoryD(DirectoryList *D, tPosD pos) {
    if (!isEmptyListD(*D)) {
        tPosD q;
        if (pos == *D) {
            *D = (*D)->next;
        } else if (pos->next == DNULL) {
            for (q = *D; q->next != pos; q = q->next);
            q->next = DNULL;
        } else {
            q = pos->next;
            pos->directory[0] = '\0';
            pos->next = q->next;
            pos = q;
        }
        free(pos);
    } else {
        printf("The directory list is empty\n");
    }
}



char* SearchListFirstD(DirectoryList D) {
    if (isEmptyListD(D)) {
        return NULL;
    }
    return D->directory;
}

tPosD SearchDirectoryD(char *p, DirectoryList D) {
    for (tPosD t = D; t != DNULL; t = t->next) {
        if (strcmp(p, t->directory) == 0) {
            return t;
        }
    }
    return DNULL;
}

void ListDirectoryList(DirectoryList D) {
    if (!isEmptyListD(D)) {
        for (tPosD p = D; p != DNULL; p = p->next) {
            printf("%s\n", p->directory);
        }
    }
}

void cleanDirectoryList(DirectoryList *D) {
    tPosD p = DNULL;
    while (*D != DNULL) {
        p = *D;
        *D = (*D)->next;
        free(p);
    }
}