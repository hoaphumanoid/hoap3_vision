/* Hoap-3 Video Capture Client Program            */
/*                                                */
/*   command.c  2004.12.28  takaishi              */
/*                                                */
/* Copyright(C) FUJITU AUTOMATION LIMITED  2004   */

/* MODIFIED                                       */
/*      command.cpp                               */
/*   Hoap-3 communication client program          */
/*   Daniel Hernández García 2010.11              */
/*   Universidad Carlos III de Madrid             */




#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "command.h"

#include <iostream>
#define DB(x)


command::command(Sock* s){
    cSocket = s;

}


command::~command(){

}


int command::sendCommand(){


    int len;
    strcpy(buf, bufCommand);    
    len=send(cSocket->getSoc(),buf,strlen(buf),0);
    if(len<0){
            /* error */
            perror("send");
            return 1;
    }

    return 0;
}



int command::sendCommand(char *c){

    int r;
    bufCommand = c;
  //std::cout << "c " << c << "buf" << bufCommand << std::endl;
    r = sendCommand();
    return r;
}


int command::recvCommand(){

    int len;
    int j=0;

    /* recv capture data */
    len=recv(cSocket->getSoc(),buf,sizeof(buf),MSG_WAITALL);
    if(len<0){
        /* error */
        perror("recv");
    }
    DB(fprintf(stderr,"RECVOK len=%d,cnt=%d\n",len,i));

    //copy from buf to yuvbuf
    for(j=0;j<len;j++){
        bufRecv[j]=buf[j];
        //       printf("%f(%d)::::%f\n", (float)buf[j], j, (float)*vbuf);
        //bufRecv++;
    }

    return(0);
}

