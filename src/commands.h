//
// Created by matthew on 1/16/24.
//

#ifndef AIMOS_COMMANDS_H
#define AIMOS_COMMANDS_H

/**
 * Uses ssh to log in to the given landing server and then ssh to the given cluster server
 * @param landing The username\@server address string for the landing server
 * @param server The username\@server address string for the cluster server
 * @return The exit code of the ssh command
 */
int shellCommand(char* landing, char* server);


/**
 * Uses ssh to log in to the given landing server
 * @param landing The username\@server address string for the landing server
 * @return The exit code of the ssh command
 */
int landingCommand(char* landing);


/**
 * Uses sshfs to mount the AiMOS file system to the given mount point
 * @param landing The username\@server address string for the landing server
 * @param mountSrc The path to the user's home directory on the remote landing server
 * @param mountDest The path to the local mount point
 * @return The exit code of the sshfs command
 */
int mountCommand(char* landing, char* mountSrc, char* mountDest);


/**
 * Uses fusermount to unmount the AiMOS file system from the given mount point
 * @param mountPoint The path to the local mount point
 * @return The exit code of the fusermount command
 */
int unmountCommand(char* mountPoint);

#endif //AIMOS_COMMANDS_H
