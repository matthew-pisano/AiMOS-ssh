//
// Created by matthew on 1/16/24.
//

#include <stdlib.h>
#include <string.h>
#include "commands.h"


int shellCommand(char* landing, char* server) {

    char* prefix = "ssh -o ProxyCommand=\"ssh -q -W %h:%p ";
    char* command = malloc(sizeof(char) * (strlen(landing) + strlen(prefix) + strlen(server) + 3));
    strcpy(command, prefix);
    strcat(command, landing);
    strcat(command, "\" ");
    strcat(command, server);

    int exitCode = system(command);
    free(command);
    return exitCode;
}

int landingCommand(char* landing) {

    char* command = malloc(sizeof(char) * (strlen(landing) + 5));
    strcpy(command, "ssh ");
    strcat(command, landing);

    int exitCode = system(command);
    free(command);
    return exitCode;
}

int mountCommand(char* landing, char* mountSrc, char* mountDest) {

    char* prefix = "sshfs -o allow_other,follow_symlinks \"";
    char* command = malloc(sizeof(char) * (strlen(landing) + strlen(prefix) + strlen(mountSrc) + strlen(mountDest) + 4));
    strcpy(command, prefix);
    strcat(command, landing);
    strcat(command, ":");
    strcat(command, mountSrc);
    strcat(command, "\" ");
    strcat(command, mountDest);

    int exitCode = system(command);
    free(command);
    return exitCode;
}

int unmountCommand(char* mountPoint) {

    char* command = malloc(sizeof(char) * (strlen(mountPoint) + 15));
    strcpy(command, "fusermount -u ");
    strcat(command, mountPoint);

    int exitCode = system(command);
    free(command);
    return exitCode;
}
