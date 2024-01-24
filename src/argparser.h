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
    -h --help                         : Show this message\n\
    -n=NAME, --cluster-name=NAME      : The name of the cluster to enter (dcs, npl, ...)\n\
    -l=NUM, --landing-num=NUM         : The number of the landing node to log into\n\
    -n=NUM, --cluster-num=NUM         : The number of the cluster front end node to log into\n\
    -u=USER, --user=USER              : The username to log in with\n\
    -m=MOUNTPOINT, --mount=MOUNTPOINT : The mount point for the AiMOS file system\n\
Commands: \n\
    c cluster                         : Enters AiMOS through a cluster front end node shell\n\
    l landing                         : Enters an AiMOS landing node through ssh shell\n\
    m mount                           : Mounts the AiMOS file system to the given mount point\n\
    u unmount umount                  : Unmounts the AiMOS file system\n"


/**
 * Gets the value of the given flag whe in the form -f=VAL or --flag=VAL
 * @param numFlags The number of flags given by the user
 * @param flags An array of the flags given by the user
 * @param searchFlag The flag to search for
 * @param val A pointer to the string to assign the value to
 * @return Zero if the flag was found, one otherwise
 */
int getFlagVal(int numFlags, char** flags, char* searchFlag, char** val);


/**
 * Gets the value of the given flag whe in the form -f=VAL or --flag=VAL
 * @param numFlags The number of flags given by the user
 * @param flags An array of the flags given by the user
 * @param searchFlag The flag to search for
 * @param val A pointer to the string to assign the value to
 * @param minValLen A pointer to the minimum length of the value to assign.  This value is ignored if the value is longer than the minimum length or if the minimum length is null
 * @return Zero if the flag was found, one otherwise
 */
int getnFlagVal(int numFlags, char** flags, char* searchFlag, char** val, const int* minValLen);



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
 * Makes the server address to log into the given landing node
 * @param user The AiMOS username to use
 * @param landingNum The number of the landing node to use
 * @param landing A pointer to the string to assign the address to
 * @return Zero if the command was created successfully, one otherwise
 */
int makeLandingServer(char* user, char* landingNum, char** landing);


/**
 * Makes the cluster address to log into the given cluster front end node
 * @param user The AiMOS username to use
 * @param clusterNum The number of the cluster front end node to use
 * @param clusterName The name of the cluster to use
 * @param cluster A pointer to the string to assign the address to
 * @return Zero if the command was created successfully, one otherwise
 */
int makeClusterServer(char* user, char* clusterNum, char* clusterName, char** cluster);


/**
 * Runs the appropriate command based on the given arguments
 * @param command The command given to the user
 * @param user The AiMOS username to use, if given
 * @param landing The landing node to use, if given
 * @param clusterName The cluster to use, if given
 * @param mountDest The mount point to use, if given
 * @return The exit code of the command that was run
 */
int runCommand(char* command, char* user, char* landingNum, char* clusterNum, char* clusterName, char* mountDest);

/**
 * Find the given flag or alias in the given flags array and assign the value to the given val pointer
 * @param numFlags The number of flags given by the user
 * @param flags The array of flags given by the user
 * @param flagName The name of the flag to search for
 * @param flagAlias The alias of the flag to search for if the name is not found
 * @param numVal A pointer to the string to assign the value to
 * @return Zero of the operation completed successfully, one otherwise
 */
int processNodeNumFlag(int numFlags, char** flags, char* flagName, char* flagAlias, char** numVal);


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
