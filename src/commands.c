//
// Created by matthew on 1/16/24.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "commands.h"


int shellCommand(char* landing, char* clusterfen) {

    char* prefix = "ssh -o ProxyCommand=\"ssh -q -W %h:%p";
    char* command = malloc(sizeof(char) * (strlen(landing) + strlen(prefix) + strlen(clusterfen) + 5));
    sprintf(command, "%s %s\" %s", prefix, landing, clusterfen);

    int exitCode = system(command);
    free(command);
    return exitCode;
}

int landingCommand(char* landing) {

    char* command = malloc(sizeof(char) * (strlen(landing) + 6));
    sprintf(command, "ssh %s", landing);

    int exitCode = system(command);
    free(command);
    return exitCode;
}

int mountCommand(char* landing, char* mountSrc, char* mountDest) {

    char* prefix = "sshfs -o allow_other,follow_symlinks";
    char* command = malloc(sizeof(char) * (strlen(landing) + strlen(prefix) + strlen(mountSrc) + strlen(mountDest) + 9));
    sprintf(command, "%s \"%s:%s\" \"%s\"", prefix, landing, mountSrc, mountDest);

    int exitCode = system(command);
    free(command);
    return exitCode;
}

int unmountCommand(char* mountPoint) {

    char* command = malloc(sizeof(char) * (strlen(mountPoint) + 18));
    sprintf(command, "fusermount -u \"%s\"", mountPoint);

    int exitCode = system(command);
    free(command);
    return exitCode;
}
