# AiMOS SSH Manager

This is a simple SSH manager for AiMOS. It allows you to connect to any of the nodes on the AiMOS cluster node with a single command.  This is achieved through logging into a landing node shell initially, and then logging into a cluster shell from there.  The AiMOS cluster uses 2-factor authentication, so several authentication prompts will be required.

## Features

This program supports:

* Logging directly into a cluster shell through an intermediate landing shell
* Logging into a landing node shell directly
* Mounting a directory on the landing node to your local machine
* Unmounting a directory on the landing node from your local machine

## Usage

This program takes in one of several command arguments and several flags from the user.  For example:

```bash
aimos s -n=1 --user=$USER
```

will log into a cluster shell on node 1 with the username of the current user.  A complete list of commands and flags can be found within the help menu:

```bash
aimos --help
```

> [!IMPORTANT]
> All flags must be given in the format of -f=VALUE or --flag=VALUE.  The only exception to this is the -h or --help flag, which does not require a value. 


## Quick Start

1. Create and enter a build directory within the project root directory

    ```bash
    mkdir build && cd build
    ```
2. Make the aimos executable using cmake

    ```bash
    cmake .. && make
    ```
3. Run the aimos executable with the desired command and flags

    ```bash
    ./aimos --help
    ```