/* Hoap-3 Video Capture Program                   */
/*                                                */
/*   vdevsrv.h  2004.12.20  takaishi              */
/*                                                */
/* Copyright(C) FUJITU AUTOMATION LIMITED  2004   */

#ifndef __VDEVSRV_H_
#define __VDEVSRV_H_

/* device name */
#define V4L_DEVICE  "/dev/video0"
#define V4L_DEVICE2 "/dev/video1"

/* frame size */
#define V4L_WIDTH  320
#define V4L_HEIGHT 240

/* init video device */
void vdev_driver_init();

/* Next Fram Capture */
unsigned char* get_next_frame_yuv(int);


#endif /* __VDEVSRV_H_ */
