## -*- Makefile -*-
##
## User: yanivdu
## Time: May 22, 2010 8:19:43 PM
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CC = gcc
BASICOPTS = -g
CFLAGS = $(BASICOPTS)


# Define the target directories.
TARGETDIR_sim=dist/Debug/GNU-Linux-x86


all: $(TARGETDIR_sim)/assignment2

## Target: sim
CPPFLAGS_sim = \
	-I.
OBJS_sim =  \
	$(TARGETDIR_sim)/hat.o \
	$(TARGETDIR_sim)/disk.o \
	$(TARGETDIR_sim)/ui.o \
	$(TARGETDIR_sim)/tests.o \
	$(TARGETDIR_sim)/prm.o \
	$(TARGETDIR_sim)/pcb.o \
	$(TARGETDIR_sim)/freelist.o \
	$(TARGETDIR_sim)/main.o
SYSLIBS_sim = -lpthread 
USERLIBS_sim = $(SYSLIBS_sim) 
DEPLIBS_sim =  
LDLIBS_sim = $(USERLIBS_sim)


# Link or archive
$(TARGETDIR_sim)/assignment2: $(TARGETDIR_sim) $(OBJS_sim) $(DEPLIBS_sim)
	$(LINK.c) $(CFLAGS_sim) $(CPPFLAGS_sim) -o $@ $(OBJS_sim) $(LDLIBS_sim)


# Compile source files into .o files
$(TARGETDIR_sim)/hat.o: $(TARGETDIR_sim) hat.c
	$(COMPILE.c) $(CFLAGS_sim) $(CPPFLAGS_sim) -o $@ hat.c

$(TARGETDIR_sim)/disk.o: $(TARGETDIR_sim) disk.c
	$(COMPILE.c) $(CFLAGS_sim) $(CPPFLAGS_sim) -o $@ disk.c

$(TARGETDIR_sim)/ui.o: $(TARGETDIR_sim) ui.c
	$(COMPILE.c) $(CFLAGS_sim) $(CPPFLAGS_sim) -o $@ ui.c

$(TARGETDIR_sim)/tests.o: $(TARGETDIR_sim) tests.c
	$(COMPILE.c) $(CFLAGS_sim) $(CPPFLAGS_sim) -o $@ tests.c

$(TARGETDIR_sim)/prm.o: $(TARGETDIR_sim) prm.c
	$(COMPILE.c) $(CFLAGS_sim) $(CPPFLAGS_sim) -o $@ prm.c

$(TARGETDIR_sim)/main.o: $(TARGETDIR_sim) main.c
	$(COMPILE.c) $(CFLAGS_sim) $(CPPFLAGS_sim) -o $@ main.c

$(TARGETDIR_sim)/pcb.o: $(TARGETDIR_sim) pcb.c
	$(COMPILE.c) $(CFLAGS_sim) $(CPPFLAGS_sim) -o $@ pcb.c

$(TARGETDIR_sim)/freelist.o: $(TARGETDIR_sim) freelist.c
	$(COMPILE.c) $(CFLAGS_sim) $(CPPFLAGS_sim) -o $@ freelist.c

#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_sim)/sim \
		$(TARGETDIR_sim)/hat.o \
		$(TARGETDIR_sim)/disk.o \
		$(TARGETDIR_sim)/ui.o \
		$(TARGETDIR_sim)/tests.o \
		$(TARGETDIR_sim)/prm.o \
		$(TARGETDIR_sim)/pcb.o \
		$(TARGETDIR_sim)/freelist.o \
		$(TARGETDIR_sim)/main.o
	rm -f -r $(TARGETDIR_sim)


# Create the target directory (if needed)
$(TARGETDIR_sim):
	mkdir -p $(TARGETDIR_sim)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-i386-Linux

