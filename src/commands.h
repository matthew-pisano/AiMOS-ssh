//
// Created by matthew on 1/16/24.
//

#ifndef AIMOS_COMMANDS_H
#define AIMOS_COMMANDS_H

int shellCommand(char* landing, char* server);
int landingCommand(char* landing);
int mountCommand(char* landing, char* mountSrc, char* mountDest);
int unmountCommand(char* mountPoint);

#endif //AIMOS_COMMANDS_H
