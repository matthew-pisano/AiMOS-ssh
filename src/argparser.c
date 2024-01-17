//
// Created by matthew on 1/16/24.
//

#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include "argparser.h"
#include "commands.h"


void splitArgs(int argc, char** argv, int* numArgs, char*** args, int* numFlags, char*** flags) {

    // Count the number of args and flags in the command line arguments
    *numArgs = 0;
    *numFlags = 0;
    for (int i = 1; i < argc; i++)
        if (argv[i][0] == '-') (*numFlags)++;
        else (*numArgs)++;

    // Allocate memory for the args and flags
    *args = malloc(sizeof(char *) * (*numArgs));
    *flags = malloc(sizeof(char *) * (*numFlags));

    // Fill the args and flags arrays with counters to keep track of the next index to fill
    int flagCounter = 0;
    int argCounter = 0;
    for (int i = 1; i < argc; i++)
        if (argv[i][0] == '-') (*flags)[flagCounter++] = argv[i];
        else (*args)[argCounter++] = argv[i];
}

int getFlagVal(int numFlags, char** flags, char* searchFlag, char** val) {

    int flagLen = (int) strlen(searchFlag);
    for (int i=0; i<numFlags; i++) {
        // Check if the current -flag=val pair starts with the search flag
        if (strncmp(flags[i], searchFlag, flagLen) == 0) {
            // If the value to assign is null, return 0
            if (val == NULL) return 0;

            // Assign the value of the flag to the val pointer
            int valLen = (int) strlen(flags[i]) - flagLen;
            *val = malloc(sizeof(char) * (valLen + 1));
            memcpy(*val, &flags[i][flagLen], valLen);
            return 0;
        }
    }
    return 1;
}

int runCommand(char* command, char* user, char* landing, char* server, char* mountDest) {

    // If the command is to launch a cluster shell
    if (strcmp(command, "s") == 0 || strcmp(command, "shell") == 0) {
        if (user == NULL) {
            printf("%s", "Error: No user given\n");
            return 1;
        }

        return shellCommand(landing, server);
    }
    // If the command is to launch a landing shell
    else if (strcmp(command, "l") == 0 || strcmp(command, "landing") == 0) {
        if (user == NULL) {
            printf("%s", "Error: No user given\n");
            return 1;
        }

        return landingCommand(landing);
    }
    // If the command is to mount the AiMOS file system
    else if (strcmp(command, "m") == 0 || strcmp(command, "mount") == 0) {
        if (user == NULL) {
            printf("%s", "Error: No user given\n");
            return 1;
        }
        if (mountDest == NULL) {
            printf("%s", "Error: No mount point given\n");
            return 1;
        }

        // Create the mount source path string
        char* home = "/gpfs/u/home/NLUG/";
        char* mountSrc = malloc(sizeof(char) * (strlen(home) + strlen(user) + 1));
        strcpy(mountSrc, home);
        strcat(mountSrc, user);

        int exitCode = mountCommand(landing, mountSrc, mountDest);
        if (exitCode == 0) printf("Mounted %s%s:%s to %s\n", home, user, mountSrc, mountDest);
        free(mountSrc);
        return exitCode;
    }
    // If the command is to unmount the AiMOS file system
    else if (strcmp(command, "u") == 0 || strcmp(command, "unmount") == 0 || strcmp(command, "umount") == 0) {
        if (mountDest == NULL) {
            printf("%s", "Error: No mount point given\n");
            return 1;
        }

        int exitCode = unmountCommand(mountDest);
        if (exitCode == 0) printf("Unmounted %s\n", mountDest);
        return exitCode;
    }
    // If the command is invalid
    else {
        printf("%s", "Error: Invalid command\n");
        return 1;
    }
}

int processArgs(int numArgs, char** args, int numFlags, char** flags) {
    // Check if the help manu should be printed
    if (numArgs == 0 && numFlags == 0) {
        printf("%s", HELP);
        return 0;
    }

    if (getFlagVal(numFlags, flags, "-h\0", NULL) == 0 || getFlagVal(numFlags, flags, "--help\0", NULL) == 0) {
        printf("%s", HELP);
        return 0;
    }
    // Check if there is a command to process
    if (numArgs == 0) {
        printf("%s", "Error: No command given\n");
        return 1;
    }

    // Get the requested server number, exit if none given
    char* serverNum = malloc(sizeof(char) * 2);
    strcpy(serverNum, "1");
    if(getFlagVal(numFlags, flags, "-n=\0", &serverNum) != 0)
        getFlagVal(numFlags, flags, "--server-num=\0", &serverNum);
    if (strlen(serverNum) == 0) {
        printf("%s", "Error: No server number given\n");
        return 1;
    }
    // Ensure that the server number is two digits
    if (strlen(serverNum) == 1) {
        char* temp = malloc(sizeof(char) * 2);
        temp[0] = '0';
        temp[1] = serverNum[0];
        serverNum = temp;
    }

    // Get the given mount point if any is given
    char* mountDest = NULL;
    if (getFlagVal(numFlags, flags, "-m=\0", &mountDest) != 0)
        getFlagVal(numFlags, flags, "--mount=\0", &mountDest);
    // Get the given user if any is given
    char* user = NULL;
    if(getFlagVal(numFlags, flags, "-u=\0", &user) != 0)
        getFlagVal(numFlags, flags, "--user=\0", &user);

    // Create the landing and server strings if a user is given
    char* landing = NULL;
    char* server = NULL;
    if (user != NULL) {

        int landingLen = (int) strlen(user) + (int) strlen(serverNum) + 18;
        landing = malloc(sizeof(char) * landingLen);
        strncpy(landing, user, strlen(user));
        strcat(landing, "@blp");
        strcat(landing, serverNum);
        strcat(landing, ".ccni.rpi.edu");

        int serverLen = (int) strlen(user) + (int) strlen(serverNum) + 21;
        server = malloc(sizeof(char) * serverLen);
        strncpy(server, user, strlen(user));
        strcat(server, "@nplfen");
        strcat(server, serverNum);
        strcat(server, ".ccni.rpi.edu");
    }

    // Run the command
    int exitCode = runCommand(args[0], user, landing, server, mountDest);

    free(serverNum);
    if (mountDest != NULL) free(mountDest);
    if (user != NULL) {
        free(user);
        free(landing);
        free(server);
    }
    return exitCode;
}