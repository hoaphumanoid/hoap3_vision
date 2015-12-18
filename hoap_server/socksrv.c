/* Hoap-3 Video Capture Server Program            */
/*                                                */
/*   socksrv.c  2004.12.20  takaishi              */
/*                                                */
/* Copyright(C) FUJITU AUTOMATION LIMITED  2004   */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>

#include "socksrv.h"
#include "command.h"

#define DB(x)

int Soc=0;


int InitSocket()
{
	struct servent *se;
	struct sockaddr_in addr;
	int opt;

	DB(fprintf(stderr,"InitSocket()\n"));

	/* get of service infomation */
	if((se=getservbyname("hoapserver","tcp"))==NULL){
		perror("getservbyname");
		fprintf(stderr,"Please add 'hoapserver XXXX/tcp' to a /etc/services\n");
		exit(-1);
		return(-1);
	}
	
	/* make socket */
	if((Soc=socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("socket");
		return(-1);
	}
	DB(fprintf(stderr,"socket:%d\n",Soc));

	/* setting of socket_option */
	opt=1;
	if(setsockopt(Soc,SOL_SOCKET,SO_REUSEADDR,(char*)&opt,sizeof(int)) !=0){
		perror("setsockoopt");
		return(-1);
	}
	DB(fprintf(stderr,"setsockopt:OK\n"));

	/* bind */
	memset((char*)&addr,0,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_port=se->s_port;
	if(bind(Soc,(struct sockaddr*)&addr,sizeof(addr))==-1){
		perror("bind");
		return(-1);
	}
	DB(fprintf(stderr,"bind:OK\n"));

	return(0);
}

int CloseSocket()
{
	DB(fprintf(stderr,"CloseSocket()\n"));
	if(Soc>0){
		/* close socket */
		close(Soc);
	}
	Soc=0;

	return(0);
}

void EndSignal(int sig)
{
	DB(fprintf(stderr,"EndSignal()\n"));
	
	CloseSocket();
	exit(0);
}

int InitSignal()
{
	DB(fprintf(stderr,"InitSignal()\n"));
	/* initial signal */
	signal(SIGINT,EndSignal);
	signal(SIGQUIT,EndSignal);
	signal(SIGTERM,EndSignal);

	return(0);
}

int ConnectLoop()
{
	int acc;
	struct sockaddr_in from;
	int len;
	char client_address[80];

	DB(fprintf(stderr,"ConnectLoop()\n"));

	/* rady socket */
	listen(Soc,SOMAXCONN);
	fprintf(stderr,"Server Ready\n");

	/* loop of connect */
	while(1){
		len=sizeof(from);
		/* connect */
		acc=accept(Soc,(struct sockaddr *)&from,(socklen_t*) &len);
		/*  error check */
		if(acc<0){
			perror("accept");
		}
		/* client */
		CommandLoop(acc,client_address);
		/* connection close*/
		close(acc);
		acc=0;
	}
	return(0);
}

