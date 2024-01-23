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

int runCommand(char* command, char* user, char* landingNum, char* clusterNum, char* clusterName, char* mountDest) {

    // If the command is to launch a cluster shell
    if (strcmp(command, "c") == 0 || strcmp(command, "cluster") == 0) {
        if (user == NULL) {
            printf("Error: No user given\n");
            return 1;
        }
        if (clusterName == NULL) {
            printf("Error: No cluster name given\n");
            return 1;
        }
        char* landing = NULL;
        makeLandingServer(user, landingNum, &landing);
        char* clusterfen = NULL;
        makeClusterServer(user, clusterNum, clusterName, &clusterfen);

        int exitCode = shellCommand(landing, clusterfen);
        free(landing);
        free(clusterfen);
        return exitCode;
    }
    // If the command is to launch a landing shell
    else if (strcmp(command, "l") == 0 || strcmp(command, "landing") == 0) {
        if (user == NULL) {
            printf("Error: No user given\n");
            return 1;
        }
        char* landing = NULL;
        makeLandingServer(user, landingNum, &landing);
        int exitCode = landingCommand(landing);
        free(landing);
        return exitCode;
    }
    // If the command is to mount the AiMOS file system
    else if (strcmp(command, "m") == 0 || strcmp(command, "mount") == 0) {
        if (user == NULL) {
            printf("Error: No user given\n");
            return 1;
        }
        if (mountDest == NULL) {
            printf("Error: No mount point given\n");
            return 1;
        }

        // Create the mount source path string
        char* home = "/gpfs/u/home/NLUG/";
        char* mountSrc = malloc(sizeof(char) * (strlen(home) + strlen(user) + 1));
        mountSrc[0] = '\0';
        strcpy(mountSrc, home);
        strcat(mountSrc, user);

        char* landing = NULL;
        makeLandingServer(user, landingNum, &landing);
        int exitCode = mountCommand(landing, mountSrc, mountDest);
        if (exitCode == 0) printf("Mounted %s%s:%s to %s\n", home, user, mountSrc, mountDest);
        free(mountSrc);
        free(landing);
        return exitCode;
    }
    // If the command is to unmount the AiMOS file system
    else if (strcmp(command, "u") == 0 || strcmp(command, "unmount") == 0 || strcmp(command, "umount") == 0) {
        if (mountDest == NULL) {
            printf("Error: No mount point given\n");
            return 1;
        }

        int exitCode = unmountCommand(mountDest);
        if (exitCode == 0) printf("Unmounted %s\n", mountDest);
        return exitCode;
    }
    // If the command is invalid
    else {
        printf("Error: Invalid command\n");
        return 1;
    }
}

int processNodeNumFlag(int numFlags, char** flags, char* flagName, char* flagAlias, char** numVal) {
    // Get the requested node number
    if(getFlagVal(numFlags, flags, flagName, numVal) != 0)
        getFlagVal(numFlags, flags, flagAlias, numVal);

    // Ensure that the node number is not empty
    if (*numVal == NULL || strlen(*numVal) == 0) {
        *numVal = malloc(sizeof(char) * 3);
        *numVal[0] = '\0';
        strcpy(*numVal, "01");
    }
    // Ensure that the node number is two digits
    else if (strlen(*numVal) == 1) {
        char tmp = *numVal[0];
        *numVal[0] = '0';
        *numVal[1] = tmp;
    }
    // Ensure that the landing node number is not more than two digits
    else if (strlen(*numVal) > 2) {
        printf("Error: Flag %s number must be at most a two digit number\n", flagAlias);
        return 1;
    }
    return 0;
}

int makeLandingServer(char* user, char* landingNum, char** landing) {
    int landingLen = (int) strlen(user) + (int) strlen(landingNum) + 18;
    *landing = malloc(sizeof(char) * landingLen);
    *landing[0] = '\0';
    strcpy(*landing, user);
    strcat(*landing, "@blp");
    strcat(*landing, landingNum);
    strcat(*landing, ".ccni.rpi.edu");

    return 0;
}

int makeClusterServer(char* user, char* clusterNum, char* clusterName, char** cluster) {
    int clusterLen = (int) strlen(user) + (int) strlen(clusterNum) + (int) strlen(clusterName) + 18;
    *cluster = malloc(sizeof(char) * clusterLen);
    *cluster[0] = '\0';
    strcpy(*cluster, user);
    strcat(*cluster, "@");
    strcat(*cluster, clusterName);
    strcat(*cluster, "fen");
    strcat(*cluster, clusterNum);
    strcat(*cluster, ".ccni.rpi.edu");

    return 0;
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
        printf("Error: No command given\n");
        return 1;
    }

    char* landingNum = NULL;
    char* clusterNum = NULL;
    if (processNodeNumFlag(numFlags, flags, "-l=\0", "--landing-num=\0", &landingNum) == 1) return 1;
    if (processNodeNumFlag(numFlags, flags, "-c=\0", "--cluster-num=\0", &clusterNum) == 1) return 1;

    // Get the given mount point if any is given
    char* mountDest = NULL;
    if (getFlagVal(numFlags, flags, "-m=\0", &mountDest) != 0)
        getFlagVal(numFlags, flags, "--mount=\0", &mountDest);
    if (mountDest != NULL && strlen(mountDest) == 0) {
        printf("Error: Mount destination flag given but no destination specified\n");
        return 1;
    }

    // Get the given user if any is given
    char* user = NULL;
    if (getFlagVal(numFlags, flags, "-u=\0", &user) != 0)
        getFlagVal(numFlags, flags, "--user=\0", &user);
    if (user != NULL && strlen(user) == 0) {
        printf("Error: User flag given but no user specified\n");
        return 1;
    }

    // Get the given user if any is given
    char* clusterName = NULL;
    if (getFlagVal(numFlags, flags, "-n=\0", &clusterName) != 0)
        getFlagVal(numFlags, flags, "--cluster-name=\0", &clusterName);
    if (clusterName != NULL && strlen(clusterName) == 0) {
        printf("Error: Cluster name flag given but no name specified\n");
        return 1;
    }

    // Run the command
    int exitCode = runCommand(args[0], user, landingNum, clusterNum, clusterName, mountDest);

    free(landingNum);
    free(clusterNum);
    if (mountDest != NULL) free(mountDest);
    if (user != NULL) free(user);
    if (clusterName != NULL) free(clusterName);

    return exitCode;
}