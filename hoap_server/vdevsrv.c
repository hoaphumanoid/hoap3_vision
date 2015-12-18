/* Hoap-3 Video Capture Program                   */
/*                                                */
/*   vdevsrv.c  2004.12.13  takaishi              */
/*                                                */
/* Copyright(C) FUJITU AUTOMATION LIMITED  2004   */

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev.h>
#include <stdio.h>
#include <stdlib.h>

#include "vdevsrv.h"


/* Video Struct� */
static struct video_channel vch;
static struct video_mbuf vbuf;
static struct video_mmap vmm;

/* Capture Data Buffer */
static unsigned char* buf;
static unsigned char* buf2;

/* File Descriptor */
static int fd;
static int fd2;

/* Video Dev Iint */
void vdev_driver_init(){
	if ((fd = open(V4L_DEVICE, O_RDWR)) <= 0) {
		perror("Video Device Open");
		exit(-1);
	}
	if ((fd2 = open(V4L_DEVICE2, O_RDWR)) <= 0) {
		perror("Video Device Open2");
		exit(-1);
	}

	/* Get of mmap information */
	if (ioctl(fd, VIDIOCGMBUF, &vbuf) < 0){
		perror("VIDIOCGMBUF");
		exit(-1);

		/* double buffer check */
		if (vbuf.frames < 2){
			perror("NO DOUBLE BUFFER");
			exit(-1);
		}
	}

	/* mmap */
	if ((buf = (unsigned char*)mmap(0, vbuf.size, PROT_READ
				   	| PROT_WRITE,MAP_SHARED,fd,0)) == MAP_FAILED){
		perror("MMAP");
		exit(-1);
	}
	if ((buf2 = (unsigned char*)mmap(0, vbuf.size, PROT_READ
				   	| PROT_WRITE,MAP_SHARED,fd2,0)) == MAP_FAILED){
		perror("MMAP2");
		exit(-1);
	}

	/* Set of video_channel */
	vch.channel = 0;
	vch.norm = VIDEO_MODE_NTSC;
	if (ioctl(fd, VIDIOCSCHAN, &vch) < 0) {
		perror("VIDIOCSCHAN");
		exit(-1);
	}
	if (ioctl(fd2, VIDIOCSCHAN, &vch) < 0) {
		perror("VIDIOCSCHAN2");
		exit(-1);
	}

	/* Set of video_mmmap */
	vmm.width = V4L_WIDTH;
	vmm.height = V4L_HEIGHT;
	//vmm.format = VIDEO_PALETTE_RGB24;
	vmm.format = VIDEO_PALETTE_YUV420P;

	/* Buffer0  Capture */
	vmm.frame =0;
	if (ioctl(fd, VIDIOCMCAPTURE, &vmm) < 0){
		perror("VIDIOCMCAPTURE(0)");
		exit(-1);
	}
	if (ioctl(fd2, VIDIOCMCAPTURE, &vmm) < 0){
		perror("VIDIOCMCAPTURE2(0)");
		exit(-1);
	}

	/* Buffer1 Capture */
	vmm.frame =1;
	if (ioctl(fd, VIDIOCMCAPTURE, &vmm) < 0){
		perror("VIDIOCMCAPTURE(1)");
		exit(-1);
	}
	if (ioctl(fd2, VIDIOCMCAPTURE, &vmm) < 0){
		perror("VIDIOCMCAPTURE2(1)");
		exit(-1);
	}

	/* Set of Buufer0  */
	vmm.frame = 0;
	//perror("Video Device Init OK");
}

/*							*/
/*  next frame capture		*/
/*							*/
unsigned char* get_next_frame_yuv(int fdid){
	int fdt;
	unsigned char *yuvbuf;

	if (fdid == 0){
		fdt=fd;
	}else{
		fdt=fd2;
	}

	/* wait next frame */
	if (ioctl(fdt, VIDIOCSYNC, &vmm.frame) < 0) {
		perror("VIDIOCSYNC");
		exit(-1);
	}

	if (fdid == 0){
		yuvbuf = buf + vbuf.offsets[vmm.frame];
	}else{
		yuvbuf = buf2 + vbuf.offsets[vmm.frame];
	}

	/* Restart Capture for Nextframe  */
	if (ioctl(fdt, VIDIOCMCAPTURE, &vmm) < 0){
		perror("VIDIOCMCAPTURE");
		exit(-1);
	}

	/* Change of Buffer */
	vmm.frame = (vmm.frame == 0) ? 1 : 0;

	return(yuvbuf) ;
}
