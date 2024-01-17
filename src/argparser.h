//
// Created by matthew on 1/16/24.
//

#ifndef AIMOS_ARGPARSER_H
#define AIMOS_ARGPARSER_H

/**
 * The help message to print when the -h or --help flag is given
 */
#define HELP "--Help Menu--\n\
Usage: aimos [options] [command] \n\
Options: \n\
    -h --help                         :Show this message \n\
    -n=NUM, --server-num=NUM          :The number of the server to log into \n\
    -u=USER, --user=USER              :The username to log in with \n\
    -m=MOUNTPOINT, --mount=MOUNTPOINT :The mount point for the AiMOS file system\n\
Commands: \n\
    s shell                           :Enters AiMOS through a cluster shell \n\
    l landing                         :Enters an AiMOS landing server through ssh shell \n\
    m mount                           :Mounts the AiMOS file system to the given mount point \n\
    u unmount umount                  :Unmounts the AiMOS file system\n"


/**
 * Gets the value of the given flag whe in the form -f=VAL or --flag=VAL
 * @param numFlags The number of flags given by the user
 * @param flags An array of the flags given by the user
 * @param searchFlag The flag to search for
 * @param val A pointer to the string to assign the value to
 * @return Zero if the flag was found, one otherwise
 */
int getFlagVal(int numFlags, char **flags, char *searchFlag, char **val);


/**
 * Splits the given command line arguments into an array of arguments and an array of flags
 * @param argc The number of total arguments given by the user
 * @param argv The array of arguments given by the user
 * @param numArgs A pointer that will have its value assigned to the number of program arguments given
 * @param args A pointer that will have its value assigned to an array of program arguments
 * @param numFlags A pointer that will have its value assigned to the number of program flags given
 * @param flags A pointer that will have its value assigned to an array of program flags
 */
void splitArgs(int argc, char *argv[], int *numArgs, char **args[], int *numFlags, char **flags[]);


/**
 * Runs the appropriate command based on the given arguments
 * @param command The command given to the user
 * @param user The AiMOS username to use, if given
 * @param landing The landing server to use, if given
 * @param server The cluster server to use, if given
 * @param mountDest The mount point to use, if given
 * @return The exit code of the command that was run
 */
int runCommand(char* command, char* user, char* landing, char* server, char* mountDest);


/**
 * Processes the given arguments into args and flags and runs the appropriate command
 * @param numArgs The number of arguments given by the user
 * @param args The array of arguments given by the user
 * @param numFlags The number of flags given by the user
 * @param flags The array of flags given by the user
 * @return The exit code of the command that was run
 */
int processArgs(int numArgs, char **args, int numFlags, char **flags);

#endif //AIMOS_ARGPARSER_H
