          SHELL  = /bin/sh
           HOST := $(shell uname -n)
            CPU := $(shell uname -m)
         SYSTEM := $(shell uname -o)


              CXX = g++

          INCDIRS  = -I .

# default MODE is now RELEASE:
ifeq ($(MODE), )
	mode = RELEASE
else 
   ifeq ($(MODE), RELEASE)
	mode = RELEASE
   endif
   ifeq ($(MODE), PROFILE)
	mode = PROFILE
   endif
   ifeq ($(MODE), DEBUG)
	mode = DEBUG
   endif	
endif

OBJ_DIR = OBJ_$(CPU)

ifeq ($(mode), RELEASE)
       OBJ_DIR := $(OBJ_DIR)/RELEASE
endif
ifeq ($(mode), DEBUG)
       OBJ_DIR := $(OBJ_DIR)/DEBUG
endif
ifeq ($(mode), PROFILE)
       OBJ_DIR := $(OBJ_DIR)/PROFILE
endif

ifeq ($(mode), RELEASE) 
        CXXFLAGS  = -c -O2 -Wall -MMD -frounding-math
endif
ifeq ($(mode), DEBUG) 
        CXXFLAGS  = -c -g -Wall -DVERIF_ASSERT -MMD -frounding-math
endif
ifeq ($(mode), PROFILE) 
        CXXFLAGS  = -c -O2 -pg -Wall -DVERIF_ASSERT -MMD -frounding-math
endif

CXXFLAGS := $(CXXFLAGS) $(ARCHI_FLAG) -pipe

              EXE = main
LIB_METALLURGICAL = libMetallurgical/$(OBJ_DIR)/libMetallurgical.a

main: main.o $(LIB_METALLURGICAL)
	g++ -o $@ $?

main.o: main.cpp Makefile
	g++ $(CXXFLAGS) -o $@ $<

