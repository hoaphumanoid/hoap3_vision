#====================================================================
# Makefile for C or C++ to be used with GNU make
#====================================================================

#====================================================================
# Macros
#====================================================================

# default commands definitions
PWD=pwd
RM=rm -rf
CP=cp -af
CCC = gcc
CXX = g++

# default target and directories definitions
TARGET = hoap_stereo.bin
INSTALLDIR = ./

# filters to compile only source code with .c .cc and .cpp extensions 
OBJC = $(patsubst %.c,%.o,$(filter %.c,$(SOURCE)))
OBJCC = $(patsubst %.cc,%.o,$(filter %.cc,$(SOURCE)))
OBJCPP = $(patsubst %.cpp,%.o,$(filter %.cpp,$(SOURCE)))

# source code and definitions in other files
COMMONSOURCEDIR = ../modules/common
include ../common.path
UTILSSOURCEDIR = ../modules/utils
include ../utils.path
PAMSOURCEDIR = ../modules/pam
include ../pam.path

# flags, libraries, paths and source code used to compile 
LIBS = \
	-L/usr/lib \
	-pthread

CXXFLAGS = \
	-O2 \
	-Wall \
	-pthread \
	-D__LINUX__ \
	$(SOURCEPATH)

SOURCEPATH = \
	-I. \
	-I../modules/include/ \
	-I../../../include \
	-I../../../include/rtlinux-3.2-pre3/include \
	-I../../../include/rtlinux-3.2-pre3/include/compat \
	$(COMMONSOURCEPATH) \
	$(UTILSSOURCEPATH) \
	$(PAMSOURCEPATH)
	

SOURCE = \
	main.c \
	command.c \
	vdevsrv.c \
	socksrv.c \
	sm_access.c \
	$(COMMONSOURCE) \
	$(UTILSSOURCE) \
	$(PAMSOURCE)

#====================================================================
# Rules
#====================================================================

%.o: %.c
	$(CXX) $(CXXFLAGS) -o $@ -c $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $^

$(TARGET) : $(OBJC) $(OBJCC) $(OBJCPP)
	$(CXX) -o $@ $^ $(LIBS)

all : $(TARGET)

install: $(TARGET)
	$(CP) $^ $(INSTALLDIR)

clean :
	$(RM) $(TARGET) $(OBJC) $(OBJCC) $(OBJCPP)
	$(RM) $(INSTALLDIR)/$(TARGET)
	
