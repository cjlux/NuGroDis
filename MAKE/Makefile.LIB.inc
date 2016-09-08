#
# $Id$
#

            SHELL = /bin/sh

          TOP_DIR = .
          SRC_DIR = $(TOP_DIR)/SRC

.PHONY: all purge clean cleanAll info

.SUFFIXES:

all: 
	cd $(SRC_DIR); $(MAKE) $@

purge:
	-@rm -f .*~ *~ #*
	cd $(SRC_DIR); $(MAKE) $@

clean: purge
	cd $(SRC_DIR); $(MAKE) $@

cleanAll:
	cd $(SRC_DIR); $(MAKE) $@

info:
	cd $(SRC_DIR); $(MAKE) $@
