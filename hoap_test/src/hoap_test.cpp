// Copyright  (C)  2010  Miguel González-Fierro

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <iostream>


#include "sock.h"
#include "vget.h"
#include "command.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {

    IplImage *frameL; IplImage *frameR; IplImage *pFrameL; IplImage *pFrameR;
    cvNamedWindow("Left Camera", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Right Camera", CV_WINDOW_AUTOSIZE);
    string regObj;
    int imN;


    //Calibration Params
    double mLeft[] = { 422.3578,  0,  159.3026,
                       0,  423.8557,  140.2050,
                      0, 0, 1 };
    double mRight[] = { 425.867,  0,  153.6501,
                       0,  425.0106,  120.708,
                      0, 0, 1 };
    CvMat cameraMatrixLeft;
    cvInitMatHeader(&cameraMatrixLeft, 3, 3, CV_64F, mLeft);
    CvMat cameraMatrixRight;
    cvInitMatHeader(&cameraMatrixRight, 3, 3, CV_64F, mRight);
    double ccLeft[] = { 0.0257,  -0.062,  0.0169, -0.0006,  0 };
    double ccRight[] = { -0.121,  0.9839,  -0.0115, -0.0041,  0 };
    CvMat distCoeffsLeft;
    cvInitMatHeader(&distCoeffsLeft, 1, 5, CV_64F, ccLeft);
    CvMat distCoeffsRight;
    cvInitMatHeader(&distCoeffsRight, 1, 5, CV_64F, ccRight);
    ///Cameras Parameterss


    Sock videoServer = Sock(argv[1], (char*)"hoapserver", (char*)"tcp");
    //Sock commandServer = Sock(argv[1], "hoapcommand", "tcp");

    vget leftEyeServ = vget(0, &videoServer);
    vget rightEyeServ = vget(1, &videoServer);
    //command commServ = command(&commandServer);

    leftEyeServ.get_next_frame();
    rightEyeServ.get_next_frame();
    frameL = leftEyeServ.getImage();
    frameR = rightEyeServ.getImage();

    CvVideoWriter* leftWriter = cvCreateVideoWriter( "../share/leftEye.avi", CV_FOURCC('D','I','V','X') , 5, cvGetSize(frameL));
    CvVideoWriter* rightWriter = cvCreateVideoWriter( "../share/rightEye.avi", CV_FOURCC('D','I','V','X') , 5, cvGetSize(frameR));
    char filenameL[50];
    char filenameR[50];
    int cont =0;
    while(1) {

        //pFrameL = frameL; pFrameR = frameR;

        leftEyeServ.get_next_frame();
        rightEyeServ.get_next_frame();

        frameL = leftEyeServ.getImage();
        frameR = rightEyeServ.getImage();

        //frameL = imagestabilization(frameL, pFrameL);
        //frameR = imagestabilization(frameR, pFrameR);

        //regObj = deteccionDistancia(frameL, frameR);

        char c = cvWaitKey(50);
        if(c == 27) break;
        //if(c == 120) { commServ.sendCommand("MOVE HEAD LEFT 10 DEGREES\n"); }
        //if(c == 118) { commServ.sendCommand("MOVE HEAD RIGHT 10 DEGREES\n"); }
        //if(c == 100) { commServ.sendCommand("MOVE HEAD UP 10 DEGREES\n"); }
        //if(c == 99) { commServ.sendCommand("MOVE HEAD DOWN 10 DEGREES\n"); }

        if(c == 'm') {
            cont++;
            sprintf(filenameL, "left%.2d.png",cont);
            sprintf(filenameR, "right%.2d.png",cont);

            cvSaveImage(filenameL,frameL);
            cvSaveImage(filenameR,frameR);

        }


        if(c == 'c') {
            cont++;
            sprintf(filenameL, "izquierda%.2d.png",cont);
            sprintf(filenameR, "derecha%.2d.png",cont);
            cvSaveImage(filenameL,frameL);
            cvSaveImage(filenameR,frameR);

            CvSize size = cvGetSize(frameL);
            pFrameL = cvCreateImage(size,frameL->depth,frameL->nChannels);
            pFrameR = cvCreateImage(size,frameR->depth,frameR->nChannels);

            cvUndistort2( frameL, pFrameL, &cameraMatrixLeft, &distCoeffsLeft);
            cvUndistort2( frameR, pFrameR, &cameraMatrixRight, &distCoeffsRight);

            cout << "Image undistorted" << endl;
            sprintf(filenameR, "derecha%.2d_undistorted.png",cont);
            sprintf(filenameL, "izquierda%.2d_undistorted.png",cont);

            cvSaveImage(filenameL,pFrameL);
            cvSaveImage(filenameR,pFrameR);

            CvMat *leftFrameG = cvCreateMat(size.height, size.width, CV_8UC1 );
            CvMat *rightFrameG= cvCreateMat(size.height, size.width, CV_8UC1 );
            cvCvtColor(pFrameL,leftFrameG,CV_BGR2GRAY);
            cvCvtColor(pFrameR,rightFrameG,CV_BGR2GRAY);

            cout << "Computing Disparity Maps..." << endl;

            // image_left and image_right are the input 8-bit single-channel images
            // from the left and the right cameras, respectively
            CvMat* disparity_left = cvCreateMat( size.height, size.width, CV_16S );
            CvMat* disparity_right = cvCreateMat( size.height, size.width, CV_16S );
            CvStereoGCState* state = cvCreateStereoGCState( 16, 5 );
            cvFindStereoCorrespondenceGC( leftFrameG, rightFrameG, disparity_left, disparity_right, state, 0 );
            cvReleaseStereoGCState( &state );
            CvMat* disparity_left_visual = cvCreateMat( size.height, size.width, CV_8U );
            cvConvertScale( disparity_left, disparity_left_visual, -32 );
            CvMat* disparity_right_visual = cvCreateMat( size.height, size.width, CV_8U );
            cvConvertScale( disparity_right, disparity_right_visual, 32 );

            cout << "Done! disparity maps" << endl;
            sprintf(filenameL, "izquierda%.2d_disparitymap.png",cont);
            sprintf(filenameR, "derecha%.2d_disparitymap.png",cont);
            cvSaveImage(filenameL,disparity_left_visual);
            cvSaveImage(filenameR,disparity_right_visual);

        }


        cvShowImage("Left Camera", frameL);
        cvShowImage("Right Camera", frameR);

        cvWriteFrame( leftWriter, frameL );
        cvWriteFrame( rightWriter, frameR );
    }

    cvReleaseImage(&frameL);
    cvReleaseImage(&frameR);
    cvReleaseVideoWriter(&leftWriter);
    cvReleaseVideoWriter(&rightWriter);

    return 0;
}
