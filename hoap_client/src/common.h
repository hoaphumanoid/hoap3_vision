//====================================================================
//
// Common.h
//
// This file contains all the data structures, constants, and macros that are
// shared by all the modules in the architecture
//
// Copyright (C) 1999-2002 Orebro Universtity and Team Sweden, Sweden
// Copyright (C) 2002-2008 Universtity of Murcia and Team Chaos, Spain
// Copyright (C) 2007-2008 Universtity Carlos III of Madrid and Team Chaos, Spain
// All Rights Reserved.
//====================================================================

//====================================================================
#ifndef COMMON_H
#define COMMON_H
//====================================================================

//====================================================================
// Global Includes
//====================================================================
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

//====================================================================
// Common Data type definitions
//====================================================================
//typedef unsigned short	uint8;
typedef unsigned int	uint16;
typedef unsigned long	uint32;
typedef unsigned char	byte;		// 8-bit quantity

//====================================================================
// Common Utilities
//====================================================================
#define PI			3.1415926536
#define PI2			6.2831853072
#define PIh			1.5707963267
#define PIq			0.7853981633
#define SQRT_2		1.4142565
#define SQRT_3		1.7320508076
#define DEG(x)		((x)*57.295779)       // convert radiants to degrees
#define RAD(x)		((x)*0.01745329252)   // convert degrees to radiants
#define NORM(x)		((x) > PI) ? ((x)-PI2) : (((x) < -PI) ? ((x)+PI2) : (x))
#define ROUNDF(x)	((x-(int)x) > 0.5 ? (int)(x+1) : (int)(x))  // rounds a float value
 
#define YES			1
#define NO			0
#define DONE		1
#define NOTYET		0

#ifndef MIN
#define MIN(x,y)	(((x) > (y)) ? (y) : (x))
#endif

#ifndef MAX
#define MAX(x,y)	(((x) < (y)) ? (y) : (x))
#endif

#define ABSF(x)		(((x) > 0.0) ? (x) : -(x))
#define ABSI(x)		(((x) > 0) ? (x) : -(x))

inline double Normalise_PI(double angle)
{
	while(angle > PI)	angle -= PI2;
	while(angle <= -PI)	angle += PI2;
	
	return angle;
}

//====================================================================
// Global Customizations
//====================================================================
// robot model
#define NAO_MODEL		0
#define HOAP_3_MODEL	1

#define ModelName(x) \
	(((x)==NAO_MODEL)   ? "NAO MODEL" :\
	((x)==HOAP_3_MODEL) ? "HOAP 3 MODEL" :\
	"Unknown model")

//====================================================================
// Team and player information
//====================================================================
// team numbers
#define TEAM_COLOR_BLUE			0
#define TEAM_COLOR_RED			1

// PlayerInfo struct
typedef struct
{
        int             robot_team_number;
        int             robot_model;
        int             team_color;
        bool    isTeleoperated;
} PlayerInfo;

//====================================================================
// Concerning the Pam
//====================================================================
// Stuctures which is shared by the Pam
typedef struct		// used to send the ball prediction
{
	int ex, ey;				// expected Cartesian position (mm)
	int vx, vy;				// velocity components (mm/s)
	unsigned int timestamp;	// when last perceived
} BallP;

//====================================================================
// Concerning the Gm
//====================================================================
// Position struct
typedef struct
{
	int x;
	int y;
	float theta;
	int dx;
	int dy;
	float dtheta;
} Position;

//====================================================================
// Hardware ID
//====================================================================

// Types of interpolation
#define LinearInter		0	// Linear interpolation
#define SmoothInter		1	// 3th order polynomial interpolation

//====================================================================
// Head Commands
//====================================================================
// Operator codes for Head Command:
#define NO_SCAN		-1
#define LOOK_TO		0
#define SCAN		1
#define STOP_HEAD	2

// The HeadCmd data type
typedef struct
{
	int op;			// see codes above
	int interType;
	float pan;		// degrees
	float tilt;		// degrees
	float int_time;	// interpolation (seconds)
} Hc;

// Macros to return the HC names, mainly for printing
#define HCName(x) \
	(((x)==NO_SCAN)  ? "None" :\
	((x)==SCAN_HIGH) ? "ScanHi" :\
	((x)==SCAN_MID)  ? "ScanMi" :\
	((x)==SCAN_LOW)  ? "ScanLo" :\
	((x)==SCAN_LFL)  ? "ScanLFL" :\
	((x)==SCAN_LFR)  ? "ScanLFR" :\
	((x)==SCAN_DOG)  ? "ScanDo" :\
	((x)==SCAN_YES)  ? "ScanYe" :\
	((x)==SCAN_ALL)  ? "ScanAl" :\
	((x)==LOOK_TO)   ? "LookTo" :\
	((x)==STOP_HEAD) ? "StopHd" :\
	"Unknown")

// HC Completion notification
typedef struct
{
	int done;
} Notify;

// HC Completion codes
#define COMPLETED	1
#define ABORTED		2
#define INTERRUPTED	3

// HC completion code names, mainly for printing
#define DoneHCName(x) \
	(((x)==COMPLETED)  ? "Completed" :\
	((x)==ABORTED)     ? "Aborted" :\
	((x)==INTERRUPTED) ? "Interrupted" :\
	"Unknown")

//====================================================================
// Key Press
//====================================================================
// Struct sent by Kbd module, containing keyboard input
// Also used to receive raw bytes (non-ascii) for graphical interfaces
typedef struct
{
  unsigned char key;
  int data;
} KeyPress;

//====================================================================
#endif // COMMON_H
//====================================================================
