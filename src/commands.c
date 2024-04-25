//
// Created by matthew on 1/16/24.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "commands.h"


int shellCommand(char* landing, char* clusterfen) {

    char* prefix = "ssh -o ProxyCommand=\"ssh -q -W %h:%p";
    char command[strlen(landing) + strlen(prefix) + strlen(clusterfen) + 5];
    sprintf(command, "%s %s\" %s", prefix, landing, clusterfen);

    return system(command);
}

int landingCommand(char* landing) {

    char command[strlen(landing) + 6];
    sprintf(command, "ssh %s", landing);

    return system(command);
}

int mountCommand(char* landing, char* mountSrc, char* mountDest) {

    char* prefix = "sshfs -o allow_other,follow_symlinks";
    char command[strlen(landing) + strlen(prefix) + strlen(mountSrc) + strlen(mountDest) + 9];
    sprintf(command, "%s \"%s:%s\" \"%s\"", prefix, landing, mountSrc, mountDest);

    return system(command);
}

int unmountCommand(char* mountPoint) {

    char command[strlen(mountPoint) + 13];
    sprintf(command, "umount -l \"%s\"", mountPoint);

    return system(command);
}
