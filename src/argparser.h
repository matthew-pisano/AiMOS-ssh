//
// Created by matthew on 1/16/24.
//

#ifndef AIMOS_ARGPARSER_H
#define AIMOS_ARGPARSER_H

#define HELP "--Help Menu--\n\
Usage: aimos [options] [command] \n\
Options: \n\
    -h --help                         :Show this message \n\
    -n=NUM, --server-num=NUM          :The number of the server to log into \n\
    -u=USER, --user=USER              :The username to log in with \n\
    -m=MOUNTPOINT, --mount=MOUNTPOINT :The mount point for the aimos file system\n\
Commands: \n\
    s shell                           :Enters aimos through an ssh shell \n\
    l landing                         :Enters an aimos landing server through ssh shell \n\
    m mount                           :Mounts the aimos file system to the given mount point \n\
    u unmount umount                  :Unmounts the aimos file system\n"

int getFlagVal(int numFlags, char **flags, char *searchFlag, char **val);
void splitArgs(int argc, char *argv[], int *numArgs, char **args[], int *numFlags, char **flags[]);
int runCommand(char* command, char* user, char* landing, char* server, char* mountDest);
int processArgs(int numArgs, char **args, int numFlags, char **flags);

#endif //AIMOS_ARGPARSER_H
