/* Hoap-3 Video Capture Server Program            */
/*                                                */
/*   main.c  2005.01.14  takaishi                 */
/*                                                */
/* Copyright(C) FUJITU AUTOMATION LIMITED  2004   */

#include "vdevsrv.h"
#include "command.h"

#include "socksrv.h"
#include "socksrv2.h"

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include <mbuff.h>
#include <MemMan.h>
#define NAME_OF_MEMORY "DataSharedMemory"
SM* pSM;


void *MainFunc(void *arg)
{
	pSM = (SM*) mbuff_alloc(NAME_OF_MEMORY, sizeof(SM));
	if(pSM == NULL) {
		perror("mbuff_alloc failed");
		return NULL;
	}

	while (1)
	{
		usleep(50);
		CommandLoop();
	}
	mbuff_free(NAME_OF_MEMORY, (void*)pSM);
	return NULL;
}

void *VisionServer(void *arg)
{
	/* initial setting of socket */
	InitSocket();
	
	/* initial setting of signal */
	InitSignal();
	
	/* loop */
	InitCommand();
	ConnectLoop();
	
	/* close of socket */
	CloseSocket();
	
	return NULL;
}

void *CommandServer(void *arg)
{
	/* initial setting of socket */
	InitSocket2();
	
	/* initial setting of signal */
	InitSignal2();
	
	/* loop */
	InitCommand2();
	ConnectLoop2();
	
	/* close of socket */
	CloseSocket2();
	
	return NULL;
}

int main()
{
	/* initial setting of video device */
	vdev_driver_init();
	
	/* Create main thread */
	pthread_t mainpth;
	pthread_create(&mainpth, NULL, MainFunc, (void *) "main processing...");
	
	/* Create vision thread */
	pthread_t servervisionpth;
	pthread_create(&servervisionpth, NULL, VisionServer, (void *) "vision server processing...");
	
	/* Create command thread */
	pthread_t servercommandpth;
	pthread_create(&servercommandpth, NULL, CommandServer, (void *) "command server processing...");
	
	/* wait for threads to finish before continuing */
	pthread_join(mainpth, NULL);
	
	return(0);
}
