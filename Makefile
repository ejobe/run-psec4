#############################################################################
# Makefile for PSEC-4 EVAL exes 
# 2/2012
#############################################################################
#Generic and Site Specific Flags
CC=g++ -g
LDFLAGS=$(LIBS) -lusb
CXXFLAGS=-Wall -O2

INC= -I./include/
#-I/usr/src/linux-headers-2.6.38-12/include/ -I/usr/src/linux-headers-2.6.38-12/include/linux

EXE=	bin/RawRead bin/LinScan bin/LogData bin/TakePed \
	bin/SetPed bin/ScopeData bin/refresh \
	bin/SetInternalTrig bin/EnableTrig 

#############################################################################
OBJS= 	obj/PSEC4_EVAL.o   \
	obj/stdUSBl.o \
	obj/ScopePipe.o
#############################################################################
default:
	if [ ! -e bin	]; then mkdir	bin; fi
	if [ ! -e obj	]; then mkdir   obj; fi
	$(MAKE) all

all : $(EXE)


obj/%.o : src/%.cpp
	$(CC) $(INC) -c $< -o $@
obj/%.o : src/%.cxx
	$(CC) $(INC) -c $< -o $@

#############################################################################
bin/RawRead : obj/RawRead.o $(OBJS); $(CC) $^ $(LDFLAGS) -o $@

bin/LinScan : obj/LinScan.o $(OBJS); $(CC) $^ $(LDFLAGS) -o $@

bin/LogData : obj/LogData.o $(OBJS); $(CC) $^ $(LDFLAGS) -o $@

bin/TakePed : obj/TakePed.o $(OBJS); $(CC) $^ $(LDFLAGS) -o $@

bin/SetPed  : obj/SetPed.o $(OBJS); $(CC) $^ $(LDFLAGS) -o $@

bin/ScopeData : obj/ScopeData.o $(OBJS); $(CC) $^ $(LDFLAGS) -o $@

bin/refresh : obj/refresh.o $(OBJS); $(CC) $^ $(LDFLAGS) -o $@

#bin/SetSampleRate : obj/SetSampleRate.o $(OBJS); $(CC) $^ $(LDFLAGS) -o $@

bin/SetInternalTrig : obj/SetInternalTrig.o $(OBJS); $(CC) $^ $(LDFLAGS) -o $@

bin/EnableTrig : obj/EnableTrig.o $(OBJS); $(CC) $^ $(LDFLAGS) -o $@

#############################################################################
clean:
	@ rm -f $(OBJS) *~ *.o src/*~ -rf obj/

clean_all:
	@ rm -f $(OBJS) *~ *.o src/*~ -rf bin/ obj/
#############################################################################
