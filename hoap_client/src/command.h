/* Hoap-3 Video Capture Client Program            */
/*                                                */
/*   command.h  2004.12.20  takaishi                 */
/*                                                */
/* Copyright(C) FUJITU AUTOMATION LIMITED  2004   */

#ifndef COMMAND_H
#define COMMAND_H


#define BUF_SIZE (512)

#include "sock.h"


class command{


private:

    Sock* cSocket;
    char *bufCommand;
    char bufRecv[BUF_SIZE];
    char buf[BUF_SIZE];

public:

    command(Sock* s);
     ~command();


     ///* Next Fram Capture */
     int recvCommand();
     int sendCommand();
     int sendCommand(char *c);




};

#endif // COMMAND_H
