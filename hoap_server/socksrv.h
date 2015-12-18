/* Hoap-3 Video Capture Server Program            */
/*                                                */
/*   socksrv.h  2004.12.20  takaishi              */
/*                                                */
/* Copyright(C) FUJITU AUTOMATION LIMITED  2004   */

#ifndef __SOCKSRV_H_
#define __SOCKSRV_H_

/* initial setting of socket */
int InitSocket();

/* close of socket */
int CloseSocket();

/* initial setting of signal */
int InitSignal();

/* connet loop */
int ConnectLoop();

#endif /* __SOCKSRV_H_ */
