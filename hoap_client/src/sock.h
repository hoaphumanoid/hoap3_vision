/* Hoap-3 Video Capture Client Program            */
/*                                                */
/*   sock.h  2004.12.13  takaishi                 */
/*                                                */
/* Copyright(C) FUJITU AUTOMATION LIMITED  2004   */

/* MODIFIED                                       */
/*      sock.h                                  */
/*   Hoap-3 communication client program          */
/*     Sockect Class                              */
/*   Daniel Hernández García 2010.11              */
/*   Universidad Carlos III de Madrid             */

#ifndef SOCK_H
#define SOCK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>

/* End Signal */
//void EndSignal(int sig);

class Sock{

private:

    struct servent *se;
    struct hostent *servhost;
    struct sockaddr_in server;
    char *hostname;
    char *servname; char *protocol;
    int Soc;

public:

    Sock(char *host, char *serv, char *ptcl);
     ~Sock();

    /* initial setting of socket */
    int InitSocket();
    /* close socket */
    int CloseSocket();

    /* initial setting of signal */
   // int InitSignal();
    int getSoc(){return Soc;};
    int EndSoc();
};





#endif // SOCK_H
