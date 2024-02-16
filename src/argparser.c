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

int getnFlagVal(int numFlags, char** flags, char* searchFlag, char** val, const int* minValLen) {

    int flagLen = (int) strlen(searchFlag);
    for (int i=0; i<numFlags; i++) {
        // Check if the current -flag=val pair starts with the search flag
        if (strncmp(flags[i], searchFlag, flagLen) == 0) {
            // If the value to assign is null, return 0
            if (val == NULL) return 0;

            // Assign the value of the flag to the val pointer
            int valLen = (int) strlen(flags[i]) - flagLen;
            if (minValLen != NULL && valLen < *minValLen)
                *val = calloc(*minValLen + 1, sizeof(char));
            else
                *val = calloc(valLen + 1, sizeof(char));

            strcpy(*val, &flags[i][flagLen]);

            return 0;
        }
    }
    return 1;
}

int getFlagVal(int numFlags, char** flags, char* searchFlag, char** val) {
    return getnFlagVal(numFlags, flags, searchFlag, val, NULL);

}

int runCommand(char* command, char* project, char* user, char* landingNum, char* clusterNum, char* clusterName, char* mountDest) {

    int exitCode = -1;

    // If the command is to unmount the AiMOS file system
    if (strcmp(command, "u") == 0 || strcmp(command, "unmount") == 0 || strcmp(command, "umount") == 0) {

        if (mountDest == NULL) {
            printf("Error: No mount point given\n");
            return 1;
        }

        exitCode = unmountCommand(mountDest);
        if (exitCode == 0) printf("Unmounted %s\n", mountDest);
        return exitCode;
    }

    if (user == NULL) {
        printf("Error: No user given\n");
        return 1;
    }

    if (project == NULL) {
        printf("Error: No project given\n");
        return 1;
    }

    char* projUser = calloc(strlen(project) + strlen(user) + 2, sizeof(char));
    sprintf(projUser, "%s%s", project, user);

    char* landing = NULL;
    makeLandingServer(projUser, landingNum, &landing);

    // If the command is to launch a cluster shell
    if (strcmp(command, "c") == 0 || strcmp(command, "cluster") == 0) {

        if (clusterName == NULL) {
            printf("Error: No cluster name given\n");
            exitCode = 1;
        }
        else {
            char* clusterfen = NULL;
            makeClusterServer(projUser, clusterNum, clusterName, &clusterfen);

            exitCode = shellCommand(landing, clusterfen);
            free(clusterfen);
        }
    }
    // If the command is to launch a landing shell
    else if (strcmp(command, "l") == 0 || strcmp(command, "landing") == 0) {
        exitCode = landingCommand(landing);
    }
    // If the command is to mount the AiMOS file system
    else if (strcmp(command, "m") == 0 || strcmp(command, "mount") == 0) {

        if (mountDest == NULL) {
            printf("Error: No mount point given\n");
            exitCode = 1;
        }
        else {
            // Create the mount source path string
            char* home = "/gpfs/u/home";
            char* mountSrc = calloc(strlen(home) + strlen(project)*2 + strlen(user) + 4, sizeof(char));
            sprintf(mountSrc, "%s/%s/%s%s", home, project, project, user);

            exitCode = mountCommand(landing, mountSrc, mountDest);
            if (exitCode == 0) printf("Mounted %s%s:%s to %s\n", home, user, mountSrc, mountDest);
            free(mountSrc);
        }
    }
    // If the command is invalid
    else printf("Error: Invalid command\n");

    free(landing);
    free(projUser);

    return exitCode;
}

int processNodeNumFlag(int numFlags, char** flags, char* flagName, char* flagAlias, char** numVal) {
    int padTo = 2;
    // Get the requested node number
    if(getnFlagVal(numFlags, flags, flagName, numVal, &padTo) != 0)
        getnFlagVal(numFlags, flags, flagAlias, numVal, &padTo);

    // Ensure that the node number is not empty
    if (*numVal == NULL || strlen(*numVal) == 0){
        // Use memory allocation to ensure that calling free is always valid
        *numVal = calloc(3, sizeof(char));
        (*numVal)[0] = '0';
        (*numVal)[1] = '1';
    }

    // Ensure that the node number is two digits
    else if (strlen(*numVal) == 1) {
        char tmp = (*numVal)[0];
        (*numVal)[0] = '0';
        (*numVal)[1] = tmp;
    }
    // Ensure that the landing node number is not more than two digits
    else if (strlen(*numVal) > 2) {
        printf("Error: Flag %s number must be at most a two digit number\n", flagAlias);
        return 1;
    }
    return 0;
}

int makeLandingServer(char* projUser, char* landingNum, char** landing) {
    int landingLen = (int) strlen(projUser) + (int) strlen(landingNum) + 19;
    *landing = malloc(sizeof(char) * landingLen);
    sprintf(*landing, "%s@blp%s.ccni.rpi.edu", projUser, landingNum);

    return 0;
}

int makeClusterServer(char* projUser, char* clusterNum, char* clusterName, char** cluster) {
    int clusterLen = (int) strlen(projUser) + (int) strlen(clusterNum) + (int) strlen(clusterName) + 19;
    *cluster = malloc(sizeof(char) * clusterLen);
    sprintf(*cluster, "%s@%sfen%s.ccni.rpi.edu", projUser, clusterName, clusterNum);

    return 0;
}

int processArgs(int numArgs, char** args, int numFlags, char** flags) {
    // Check if the help manu should be printed
    if (numArgs == 0 && numFlags == 0) {
        printf("%s", HELP);
        return 0;
    }

    if (numArgs > 1) {
        printf("Error: Too many arguments given\n");
        return 1;
    }

    if (getFlagVal(numFlags, flags, "-h", NULL) == 0 || getFlagVal(numFlags, flags, "--help", NULL) == 0) {
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
    if (processNodeNumFlag(numFlags, flags, "-l=", "--landing-num=", &landingNum) == 1) return 1;
    if (processNodeNumFlag(numFlags, flags, "-c=", "--cluster-num=", &clusterNum) == 1) return 1;

    // Get the given mount point if any is given
    char* mountDest = NULL;
    if (getFlagVal(numFlags, flags, "-m=", &mountDest) != 0)
        getFlagVal(numFlags, flags, "--mount=", &mountDest);
    if (mountDest != NULL && strlen(mountDest) == 0) {
        printf("Error: Mount destination flag given but no destination specified\n");
        return 1;
    }

    // Get the given user if any is given
    char* user = NULL;
    if (getFlagVal(numFlags, flags, "-u=", &user) != 0)
        getFlagVal(numFlags, flags, "--user=", &user);
    if (user != NULL && strlen(user) == 0) {
        printf("Error: User flag given but no user specified\n");
        return 1;
    }

    // Get the given project if any is given
    char* project = NULL;
    if (getFlagVal(numFlags, flags, "-p=", &project) != 0)
        getFlagVal(numFlags, flags, "--project=", &project);
    if (project != NULL && strlen(project) == 0) {
        printf("Error: Project flag given but no project specified\n");
        return 1;
    }

    // Get the given user if any is given
    char* clusterName = NULL;
    if (getFlagVal(numFlags, flags, "-n=", &clusterName) != 0)
        getFlagVal(numFlags, flags, "--cluster-name=", &clusterName);
    if (clusterName != NULL && strlen(clusterName) == 0) {
        printf("Error: Cluster name flag given but no name specified\n");
        return 1;
    }

    // Run the command
    int exitCode = runCommand(args[0], project, user, landingNum, clusterNum, clusterName, mountDest);

    if (landingNum != NULL) free(landingNum);
    if (clusterNum != NULL) free(clusterNum);
    if (mountDest != NULL) free(mountDest);
    if (user != NULL) free(user);
    if (clusterName != NULL) free(clusterName);

    return exitCode;
}