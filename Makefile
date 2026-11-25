ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

CXX  = g++
CXX += -I./
CXX += -I./obj/
CXX += -I./src/

CXXFLAGS  = -g -Wall -fPIC -Wno-deprecated
CXXFLAGS += $(ROOTCFLAGS)
CXXFLAGS += $(ROOTLIBS)
CXXFLAGS += $(ROOTGLIBS)
CXXFLAGS += -fconcepts

OUTLIB = ./obj/

#----------------------------------------------------#

all: makedir runsim

makedir:
	mkdir -p $(OUTLIB);

.PHONY: printmakehelp_and_reminder
printmakehelp_and_reminder: runsim.cpp Makefile
	$(info  /***********************************************************/)
	$(info  * task --> printmakehelp_and_reminder: runsim.cpp Makefile *)
	$(info  * $$@ ----> $@                      *)
	$(info  * $$< --------------------------------> $<          *)
	$(info  * $$^ --------------------------------> $^ *)
	$(info  /***********************************************************/)

obj/sim.o: src/sim.cpp src/sim.hh src/simConfStr.hh
	$(CXX) -c -o $@ $< $(CXXFLAGS) 

runsim: obj/sim.o runsim.cpp src/sierpinskiStr.hh src/simConfStr.hh
	$(CXX) -o $@ obj/sim.o runsim.cpp $(CXXFLAGS)

clean:
	rm -rf runsim
	rm -rf *~
	rm -rf src/*~
	rm -rf $(OUTLIB)*.o
	rm -rf $(OUTLIB)*.so
	rm -rf $(OUTLIB)
