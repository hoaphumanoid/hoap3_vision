#ifndef VGET_H
#define VGET_H

/* Hoap-3 Video Capture Client Program            */
/*                                                */
/*   vget.h  2004.12.14  takaishi                 */
/*                                                */
/* Copyright(C) FUJITU AUTOMATION LIMITED  2004   */


#include <cv.h>
#include <highgui.h>

#include "sock.h"

#define V4L_WIDTH  320
#define V4L_HEIGHT 240
#define BUF_SIZE (512)


class vget{

private:
    char *yuvbuf;
    char *rgbbuf;
    char *buf;
    char *rbuf;
    int framecnt;
    IplImage* Image;
    int cameraID;
    Sock* vSocket;


public:

    vget(int fid, Sock* s);
     ~vget();


     ///* Next Fram Capture */
     int RecvFrame();
     IplImage* get_next_frame();
     IplImage* getImage(){return Image;};
     /* YUV240P->RGB Convert */
     void yuv240p_rgb();
     /* time check for debug */
     void time_check(int);


};







#endif // VGET_H
