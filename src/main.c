#include <stdio.h>
#include <stdlib.h>
#include "argparser.h"


int main(int argc, char *argv[]) {
    int numArgs, numFlags;
    char **args;
    char **flags;

    splitArgs(argc, argv, &numArgs, &args, &numFlags, &flags);

    int exitCode = processArgs(numArgs, args, numFlags, flags);

    free(args);
    free(flags);

    printf("Process exited with code %d\n", exitCode);
    return exitCode;
}
