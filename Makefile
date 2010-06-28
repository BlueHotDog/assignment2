## -*- Makefile -*-
##
## User: danni
## Time: May 23, 2010 10:40:20 PM
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CC = gcc
BASICOPTS = -g
CFLAGS = $(BASICOPTS)


# Define the target directories.
TARGETDIR_Assignment2=bin


all: $(TARGETDIR_Assignment2)/sim
	mv $(TARGETDIR_Assignment2)/sim .

## Target: Assignment2
CPPFLAGS_Assignment2 = \
	-I.
OBJS_Assignment2 =  \
	$(TARGETDIR_Assignment2)/main.o \
	$(TARGETDIR_Assignment2)/ui.o \
	$(TARGETDIR_Assignment2)/prm.o \
	$(TARGETDIR_Assignment2)/freelist.o \
	$(TARGETDIR_Assignment2)/disk.o \
	$(TARGETDIR_Assignment2)/tests.o \
	$(TARGETDIR_Assignment2)/process.o \
	$(TARGETDIR_Assignment2)/pcb.o \
	$(TARGETDIR_Assignment2)/mm.o \
	$(TARGETDIR_Assignment2)/mmu.o \
	$(TARGETDIR_Assignment2)/messagequeues.o \
	$(TARGETDIR_Assignment2)/hat.o \
	$(TARGETDIR_Assignment2)/readerswriters.o \
	$(TARGETDIR_Assignment2)/aging.o \
	$(TARGETDIR_Assignment2)/ipt.o
SYSLIBS_Assignment2 = -lpthread -lrt
USERLIBS_Assignment2 = $(SYSLIBS_Assignment2) 
DEPLIBS_Assignment2 =  
LDLIBS_Assignment2 = $(USERLIBS_Assignment2)


# Link or archive
$(TARGETDIR_Assignment2)/sim: $(TARGETDIR_Assignment2) $(OBJS_Assignment2) $(DEPLIBS_Assignment2)
	$(LINK.c) $(CFLAGS_Assignment2) $(CPPFLAGS_Assignment2) -o $@ $(OBJS_Assignment2) $(LDLIBS_Assignment2)


# Compile source files into .o files
$(TARGETDIR_Assignment2)/main.o: $(TARGETDIR_Assignment2) main.c
	$(COMPILE.c) $(CFLAGS_Assignment2) $(CPPFLAGS_Assignment2) -o $@ main.c
	
$(TARGETDIR_Assignment2)/readerswriters.o: $(TARGETDIR_Assignment2) readerswriters.c
	$(COMPILE.c) $(CFLAGS_Assignment2) $(CPPFLAGS_Assignment2) -o $@ readerswriters.c

$(TARGETDIR_Assignment2)/ui.o: $(TARGETDIR_Assignment2) ui.c
	$(COMPILE.c) $(CFLAGS_Assignment2) $(CPPFLAGS_Assignment2) -o $@ ui.c

$(TARGETDIR_Assignment2)/aging.o: $(TARGETDIR_Assignment2) aging.c
	$(COMPILE.c) $(CFLAGS_Assignment2) $(CPPFLAGS_Assignment2) -o $@ aging.c

$(TARGETDIR_Assignment2)/prm.o: $(TARGETDIR_Assignment2) prm.c
	$(COMPILE.c) $(CFLAGS_Assignment2) $(CPPFLAGS_Assignment2) -o $@ prm.c

$(TARGETDIR_Assignment2)/freelist.o: $(TARGETDIR_Assignment2) freelist.c
	$(COMPILE.c) $(CFLAGS_Assignment2) $(CPPFLAGS_Assignment2) -o $@ freelist.c

$(TARGETDIR_Assignment2)/disk.o: $(TARGETDIR_Assignment2) disk.c
	$(COMPILE.c) $(CFLAGS_Assignment2) $(CPPFLAGS_Assignment2) -o $@ disk.c

$(TARGETDIR_Assignment2)/tests.o: $(TARGETDIR_Assignment2) tests.c
	$(COMPILE.c) $(CFLAGS_Assignment2) $(CPPFLAGS_Assignment2) -o $@ tests.c

$(TARGETDIR_Assignment2)/process.o: $(TARGETDIR_Assignment2) process.c
	$(COMPILE.c) $(CFLAGS_Assignment2) $(CPPFLAGS_Assignment2) -o $@ process.c

$(TARGETDIR_Assignment2)/pcb.o: $(TARGETDIR_Assignment2) pcb.c
	$(COMPILE.c) $(CFLAGS_Assignment2) $(CPPFLAGS_Assignment2) -o $@ pcb.c

$(TARGETDIR_Assignment2)/hat.o: $(TARGETDIR_Assignment2) hat.c
	$(COMPILE.c) $(CFLAGS_Assignment2) $(CPPFLAGS_Assignment2) -o $@ hat.c

$(TARGETDIR_Assignment2)/ipt.o: $(TARGETDIR_Assignment2) ipt.c
	$(COMPILE.c) $(CFLAGS_Assignment2) $(CPPFLAGS_Assignment2) -o $@ ipt.c

$(TARGETDIR_Assignment2)/mm.o: $(TARGETDIR_Assignment2) mm.c
	$(COMPILE.c) $(CFLAGS_Assignment2) $(CPPFLAGS_Assignment2) -o $@ mm.c

$(TARGETDIR_Assignment2)/mmu.o: $(TARGETDIR_Assignment2) mmu.c
	$(COMPILE.c) $(CFLAGS_Assignment2) $(CPPFLAGS_Assignment2) -o $@ mmu.c

$(TARGETDIR_Assignment2)/messagequeues.o: $(TARGETDIR_Assignment2) messagequeues.c
	$(COMPILE.c) $(CFLAGS_Assignment2) $(CPPFLAGS_Assignment2) -o $@ messagequeues.c

#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_Assignment2)/Assignment2 \
		$(TARGETDIR_Assignment2)/main.o \
		$(TARGETDIR_Assignment2)/ui.o \
		$(TARGETDIR_Assignment2)/prm.o \
		$(TARGETDIR_Assignment2)/freelist.o \
		$(TARGETDIR_Assignment2)/disk.o \
		$(TARGETDIR_Assignment2)/tests.o \
		$(TARGETDIR_Assignment2)/process.o \
		$(TARGETDIR_Assignment2)/pcb.o \
		$(TARGETDIR_Assignment2)/mm.o \
		$(TARGETDIR_Assignment2)/mmu.o \
		$(TARGETDIR_Assignment2)/messagequeues.o \
		$(TARGETDIR_Assignment2)/hat.o \
		$(TARGETDIR_Assignment2)/readerswriters.o \
		$(TARGETDIR_Assignment2)/aging.o \
		$(TARGETDIR_Assignment2)/ipt.o
	rm -f -r $(TARGETDIR_Assignment2)


# Create the target directory (if needed)
$(TARGETDIR_Assignment2):
	mkdir -p $(TARGETDIR_Assignment2)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

