/* Hoap-3 Video Capture Client Program            */
/*                                                */
/*   sock.c  2004.12.13  takaishi                 */
/*                                                */
/* Copyright(C) FUJITU AUTOMATION LIMITED  2004   */

/* MODIFIED                                       */
/*      sock.cpp                                  */
/*   Hoap-3 communication client program          */
/*     Sockect Class                              */
/*   Daniel Hernández García 2010.11              */
/*   Universidad Carlos III de Madrid             */



#include "vget.h"
#include "command.h"
#include "sock.h"

#define DB(x)

#include <iostream>


Sock::Sock(char *host, char *serv, char *ptcl){

    hostname = host;
    servname = serv;
    protocol = ptcl;
    Soc = 0;    
    InitSocket();
    std::cout << "socket: " << Soc << std::endl;
    //InitSignal();

}

Sock::~Sock(){

    //EndSignal(0);
}



int Sock::InitSocket()
{

        DB(fprintf(stderr,"InitSocket(%s)\n",hostname));
        /* get of service infomation */
        if((se=getservbyname(servname,protocol))==NULL){  //must be variable servname, servproto
                perror("getservbyname");
                return(-1);
        }
        /* get of host infomation */
        if((servhost=gethostbyname(hostname))==NULL){
                perror("gethostbyname");
                return(-1);
        }

        /* make Socket */
        if((Soc=socket(AF_INET,SOCK_STREAM,0)) < 0){
                perror("socket");
                return(-1);
        }
        DB(fprintf(stderr,"socket:%d\n",Soc));


        /* connect of Socket */
        memset((char *)&server,0,sizeof(server));
        server.sin_family=AF_INET;
        server.sin_port=se->s_port;
        memcpy((char*)&server.sin_addr,servhost->h_addr,servhost->h_length);
        if(connect(Soc,(struct sockaddr*)&server,sizeof(server))==-1){
                perror("connect");
                return(-1);
        }
        return(0);
}

int Sock::CloseSocket()
{
        DB(fprintf(stderr,"CloseSocket()\n"));
        if(Soc>0){
                /* close Socket */
                close(Soc);
        }
        Soc=0;
        return(0);
}


int Sock::EndSoc(){
        char buf[10];
        int len;
        strcpy(buf,"end\n\0");
        DB(fprintf(stderr,"CMD:%s\n",buf));
        len=send(Soc,buf,strlen(buf),0);
        if(len<0){
                /* error */
                perror("send");
        }
        return(0);
}

//int Sock::InitSignal()
//{
//        DB(fprintf(stderr,"InitSignal()\n"));
//        /* initial signal */
//        signal(SIGINT,EndSignal);
//        signal(SIGQUIT,EndSignal);
//        signal(SIGTERM,EndSignal);

//        return(0);
//}


//void EndSignal(int sig)
//{

//        Sock::EndSoc();
//        Sock::CloseSocket();
//        exit(0);
//}
