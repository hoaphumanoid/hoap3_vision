/* Hoap-3 Video Capture Client Program            */
/*                                                */
/*   vget.c  2004.12.17  takaishi                 */
/*                                                */
/* Copyright(C) FUJITU AUTOMATION LIMITED  2004   */
/* Hoap-3 Video Capture Client Program            */
/*                                                */

/*   vget.cpp  2010.09.14  Hernandez Garcia       */
/* Copyright(C) UNIVERSIDAD CARLOS III DE MADRID  2010   */
/* MODIFIED                                       */
/*      vget.cpp                                  */
/*   Hoap-3 communication client program          */
/*     Video Capture Class                              */
/*   Daniel Hernández García 2010.11              */
/*   Universidad Carlos III de Madrid             */



#include <cstdlib>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <sys/timeb.h>

#include "vget.h"
#include "sock.h"

#include <cv.h>
#include <highgui.h>

#include <iostream>

#define DB(x)


vget::vget(int fid, Sock* s){
    yuvbuf = (char*)malloc(sizeof(char) * V4L_WIDTH * V4L_HEIGHT *3/2); //yuvbuf = new uchar[sizeof(uchar) *V4L_WIDTH * V4L_HEIGHT *3/2]; //
    rgbbuf = (char*)malloc(sizeof(char) * V4L_WIDTH * V4L_HEIGHT *3); //rgbbuf = new uchar[sizeof(uchar) *V4L_WIDTH * V4L_HEIGHT *3]; //
    framecnt=0;
    cameraID = fid;
    vSocket = s;
    Image = cvCreateImageHeader(cvSize(V4L_WIDTH, V4L_HEIGHT), 8, 3);
    buf = yuvbuf;
    rbuf = rgbbuf;
}
vget::~vget(){
    free(rgbbuf); //delete yuvbuf;
    free(yuvbuf); //delete rgbbuf;
}

IplImage* vget::get_next_frame(){

    DB(time_check(100));

        /* get frame */
                RecvFrame();
                //std::cout << "YUV " << (int)yuvbuf[50] << std::endl;
                //yuvbuf= &yuvbuf[110];
                //std::cout << "YUV " << (int)*yuvbuf << std::endl;
                yuv240p_rgb();
                yuvbuf = buf;
                rgbbuf = rbuf;
                cvSetData(Image, rgbbuf, V4L_WIDTH*3);
                //mImageL = cvLoadImage("todiz.jpeg", CV_LOAD_IMAGE_COLOR );
                return Image;
}

int vget::RecvFrame()
{
        char buf[BUF_SIZE];
        int len;

        DB(fprintf(stderr,"MainLoop()\n"));

        /* send getcap command */
        if(cameraID == 0){
                strcpy(buf,"getcap0\n\0");
        }else{
                strcpy(buf,"getcap1\n\0");
        }

        DB(fprintf(stderr,"CMD:%s\n",buf));
        len=send(vSocket->getSoc(),buf,strlen(buf),0);
        if(len<0){
                /* error */
                perror("send");
        }



        /* capture recv start */
        int i=0;
        int j=0;
        int loop;
        div_t div_result;
        double frame_size=V4L_WIDTH*V4L_HEIGHT*3/2;

        /* set loop count */
        div_result=div(frame_size,BUF_SIZE);
        if(div_result.rem==0){
                loop=div_result.quot;
        }else{
                loop=div_result.quot+1;
        }

        DB(fprintf(stderr,"loop=%d\n",loop));
        while(i<loop){

                /* recv capture data */
                len=recv(vSocket->getSoc(),buf,sizeof(buf),MSG_WAITALL);
                if(len<0){
                        /* error */
                        perror("recv");
                }
                DB(fprintf(stderr,"RECVOK len=%d,cnt=%d\n",len,i));


                //copy from buf to yuvbuf
                for(j=0;j<len;j++){
                        *yuvbuf=buf[j];
                        //printf("%f(%d)::::%f(%d)\n", (float)buf[j], j, (float)*yuvbuf, (int)yuvbuf);
                        yuvbuf++;

                }
                //printf("\n\n::%d::%d::\n", j, i);
                i++;
        }

        return(0);
}



/*
*   YUV240P => RGB
*/
void vget::yuv240p_rgb(){
        int i;
        int j;

        int Y;
        int U;
        int V;
        int R;
        int G;
        int B;
        uint c_num;
        uint u_ofs;
        uint v_ofs;
        uint y_num;
        uint u_num;
        uint v_num;
        uint uv_cntup_w;
        uint uv_cntup_h;

        u_ofs=V4L_WIDTH*V4L_HEIGHT;
        v_ofs=(V4L_WIDTH*V4L_HEIGHT)+((V4L_WIDTH*V4L_HEIGHT)>>2);
        y_num=0;
        u_num=u_ofs;
        v_num=v_ofs;
        c_num=0;
        uv_cntup_w=0;
        uv_cntup_h=0;
        for (j=0; j< V4L_HEIGHT; j++){
                if(uv_cntup_h==0){
                        uv_cntup_h=1;
                }else{
                        u_num = u_num - (V4L_WIDTH>>1);
                        v_num = v_num - (V4L_WIDTH>>1);
                        uv_cntup_h=0;
                }
                for (i=0; i < V4L_WIDTH;i++){

                        yuvbuf = buf + y_num;
                        Y = (int)(uchar)*yuvbuf;
                        yuvbuf = buf + u_num;
                        U = (int)(uchar)*yuvbuf;
                        yuvbuf = buf + v_num;
                        V = (int)(uchar)*yuvbuf;

                        /* YUV to RGB Convert */
                        R=1.402*(V-128)+Y;
                        G=-0.344*(U-128)-0.714*(V-128)+Y;
                        B=1.722*(U-128)+Y;

                        //B = 1.164*(Y - 16) + 2.018*(U - 128);
                        //G = 1.164*(Y - 16) - 0.813*(V - 128) - 0.391*(U - 128);
                        //R = 1.164*(Y - 16) + 1.596*(V - 128);

                        /* Limit Check*/
                        if(R < 0) R=0;
                        if(R > 255) R=255;
                        if(G < 0) G=0;
                        if(G > 255) G=255;
                        if(B < 0) B=0;
                        if(B > 255) B=255;


                        *rgbbuf=(uchar)B; rgbbuf++;//rgbbuf[c_num]=(uchar)B;
                        *rgbbuf=(uchar)G; rgbbuf++;//rgbbuf[c_num+1]=(uchar)G;
                        *rgbbuf=(uchar)R; rgbbuf++;//rgbbuf[c_num+2]=(uchar)R;

                        //c_num=c_num+3;
                        y_num++;
                        if(uv_cntup_w==0){
                                uv_cntup_w=1;
                        }else{
                                u_num=u_num+1;
                                v_num=v_num+1;
                                uv_cntup_w=0;
                        }
                }
        }
}

void vget::time_check(int framenum){

        /* frame rate counter */
        static struct timeb start;
        static struct timeb finish;
        static int framecnt;
        double duration;
        double frame_rate;
        double bps;
        if(framecnt==0){
                ftime(&start);
                framecnt++;
        }else if(framecnt==framenum){
                ftime(&finish);
                duration = (difftime(finish.time,start.time)*1000)+((long)finish.millitm - (long)start.millitm);
                frame_rate=((framecnt/2)/duration)*1000;
                bps=(V4L_WIDTH*V4L_HEIGHT*12*framecnt/duration)/1000;
                fprintf(stderr,"FRAME_RATE=%3.2f[frm/sec]\n",frame_rate);
                fprintf(stderr,"BPS=%3.2f[Mbps]\n",bps);
                framecnt=0;
        }else{
                framecnt++;
        }
        return;
}
