#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/mmu.o \
	${OBJECTDIR}/prm.o \
	${OBJECTDIR}/hat.o \
	${OBJECTDIR}/readerswriters.o \
	${OBJECTDIR}/messagequeues.o \
	${OBJECTDIR}/ui.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/ipt.o \
	${OBJECTDIR}/tests.o \
	${OBJECTDIR}/freelist.o \
	${OBJECTDIR}/disk.o \
	${OBJECTDIR}/pcb.o \
	${OBJECTDIR}/process.o \
	${OBJECTDIR}/mm.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/assignment2

dist/Release/GNU-Linux-x86/assignment2: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/assignment2 ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/mmu.o: nbproject/Makefile-${CND_CONF}.mk mmu.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/mmu.o mmu.c

${OBJECTDIR}/prm.o: nbproject/Makefile-${CND_CONF}.mk prm.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/prm.o prm.c

${OBJECTDIR}/hat.o: nbproject/Makefile-${CND_CONF}.mk hat.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/hat.o hat.c

${OBJECTDIR}/readerswriters.o: nbproject/Makefile-${CND_CONF}.mk readerswriters.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/readerswriters.o readerswriters.c

${OBJECTDIR}/messagequeues.o: nbproject/Makefile-${CND_CONF}.mk messagequeues.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/messagequeues.o messagequeues.c

${OBJECTDIR}/ui.o: nbproject/Makefile-${CND_CONF}.mk ui.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui.o ui.c

${OBJECTDIR}/main.o: nbproject/Makefile-${CND_CONF}.mk main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/ipt.o: nbproject/Makefile-${CND_CONF}.mk ipt.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ipt.o ipt.c

${OBJECTDIR}/tests.o: nbproject/Makefile-${CND_CONF}.mk tests.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/tests.o tests.c

${OBJECTDIR}/freelist.o: nbproject/Makefile-${CND_CONF}.mk freelist.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/freelist.o freelist.c

${OBJECTDIR}/disk.o: nbproject/Makefile-${CND_CONF}.mk disk.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/disk.o disk.c

${OBJECTDIR}/pcb.o: nbproject/Makefile-${CND_CONF}.mk pcb.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/pcb.o pcb.c

${OBJECTDIR}/process.o: nbproject/Makefile-${CND_CONF}.mk process.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/process.o process.c

${OBJECTDIR}/mm.o: nbproject/Makefile-${CND_CONF}.mk mm.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/mm.o mm.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/assignment2

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
