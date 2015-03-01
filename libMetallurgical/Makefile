# $@ : cible
# $< : nom de la première dépendance
# $? : toutes les dépendance

              NAME = Metallurgical

               CXX = g++

          INCDIRS  = -I$(TOP_DIR) \
		     -I$(LIB_DIR)/SRC

SRC_FILE_LIST = Computation.cpp \
	        Hardening.cpp \
	        ThermalLoading.cpp

OBJ_FILE_LIST = $(SRC_FILE_LIST:%.cpp=$(OBJ_DIR)/%.o)

          SHELL  = /bin/sh
           HOST := $(shell uname -n)
            CPU := $(shell uname -m)
         SYSTEM := $(shell uname -o)

	ARCHI_FLAG = -m64

	  TOP_DIR = $(PWD)/..
          LIB_DIR = $(TOP_DIR)/lib$(NAME)
          SRC_DIR = $(LIB_DIR)/SRC
	  OBJ_DIR = $(LIB_DIR)/$(CPU)

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

.PHONY: all clean cleanAll purge info help obj_dir

# empties default rules:
.SUFFIXES: 

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp Makefile
	@echo "###################################################################################"
	@echo "# Compiling target    : $@ "
	@echo "###################################################################################"
	$(CXX) $(CXXFLAGS) $(EXTRACXXFLAGS) $(INCDIRS) -o $@ $<

all: obj_dir $(OBJ_DIR)/lib$(NAME).a


$(OBJ_DIR)/lib$(NAME).a: $(OBJ_FILE_LIST)
	ar -rs $@ $?

clean: purge
	-rm -f *.[oad] 
	-cd $(OBJ_DIR) && rm -f *.[oad]

cleanAll:
	find $(OBJ_DIR)/.. -name *.[oad] -exec rm -f {} \;  
	make clean

purge:
	-rm -f .*~ *~ #*
	-cd $(OBJ_DIR) && rm -f .*~ *~ #*

info:
	-@echo "NAME		: \"$(NAME)\"" 
	-@echo "MODE		: \"$(MODE)\"" 
	-@echo "mode		: \"$(mode)\"" 
	-@echo "TOP_DIR 	: \"$(TOP_DIR)\""
	-@echo "LIB_DIR 	: \"$(LIB_DIR)\""
	-@echo "SRC_DIR 	: \"$(SRC_DIR)\""
	-@echo "OBJ_DIR 	: \"$(OBJ_DIR)\""
	-@echo "OBJ_FILE_LIST 	: \"$(OBJ_FILE_LIST)\""

help:
	@echo "===================================================="
	@echo "Usage : make [MODE=RELEASE|DEBUG|PROFILE] target"
	@echo "  MODE default : RELEASE" 
	@echo "  ==> ARCHI  : $(ARCHI)"
	@echo "  ==> MODE   : $(mode)"
	@echo "  ==> CXX    : $(CXX)"
	@echo " Possible targets are :"
	@echo '       all   : compiles all what is needed.'
	@echo '       purge : removes .*~ *~ #* files       in current and OBJ directories'
	@echo '       clean : removes *.o *.a and *.d files in current and OBJ directories'
	@echo '       info  : echo value of importante Makefile variables.'

obj_dir:
	@if [ ! -d $(OBJ_DIR) ]; then \
	    mkdir -p $(OBJ_DIR); \
        fi; \

include ${wildcard $(OBJ_DIR)/*.d}

