/* 
 * File:   ui.h
 * Author: danni
 *
 * Created on May 9, 2010, 9:05 PM
 */

#ifndef _UI_H
#define	_UI_H
#include "globaldefs.h"
#include "datatypes.h"
#include "freelist.h"
#include "pcb.h"
#include "process.h"
#include "messagequeues.h"

#define MAX_INPUT_LENGTH 50



pthread_t UI_Thread;
static volatile bool UI_ShouldUIThreadClose = FALSE;
void* UI_Main();
bool UI_CreateUIThread();
void UI_SignalUIThreadToStop();

void UI_ParseCommand(const string* const comm);

/*creates a new process and prints out the new process ID
 * (or description of error if something like "insufficient space" occurred).
 */
void UI_HandleCreateProcess();
/*passes a message to the requested process, causing the process to update all
 * data structures (maps are updated, IPT,…), after it has finished running all
 * its existing jobs.  Note that there is no need to actually change the
 * content of the Disk pages or the MM pages, the MM pages will just be
 * marked free and contain 'trash' and the same goes for the Disk pages.
 */
void UI_HandleDelProcess(PID processID);
/*reads and prints amount bytes from vAddr of process id.
 * Prints each byte as a char. (or description of error if it occurred).
 * $
 */
void UI_HandleRead(int vAddr,PID processID,unsigned int amount,FILE* file);
/*reads and prints the chars found at
 * vAddr, vAddr+off, vAddr+2*off, …, vAddr+(amount-1)*off of process id.
 * Assume these memory addresses are within the bounds of the virtual memory
 * (not below 0 or too big). Assume amount > 0.
 * $
 */
void UI_HandleLoopRead(int vAddr,PID processID,int offset,unsigned int amount,FILE* file);
/*
 * identical in definition to “read vAddr id amount”,
 * with the sole difference that output is written to the specified file
 * filename, instead of to the screen.
 * $
 */
void UI_HandleReadToFile(int vAddr,PID processID,unsigned int amount,string filename);
/*
 * identical in definition to “loopRead vAddr id off amount”,
 * except that output is to the file filename and not to the screen.
 * $
 */
void UI_HandleLoopReadToFile(int vAddr,PID processID,int off,unsigned int amount,string filename);
/*
 * writes the string s starting from location vAddr. Prints "ok"
 * (or description of error if it occurred).
 * $
 */
void UI_HandleWrite(int vAddr,PID processID,string s);
/*
 * writes the char c to the following virtual
 * addresses: vAddr, vAddr+off, vAddr+2*off, …, vAddr+(amount-1)*off.
 * Assume these memory addresses are within the bounds of the virtual memory
 * (not below 0 or too big). Assume amount > 0.
 * $
 */
void UI_HandleLoopWrite(int vAddr,PID processID,char c,int off,unsigned int amount);
/*
 * prints the total hit ratio (a real number between 0 and 1).
 * (Reminder – this is equal to "how many times pages were already in MM,
 * divided by the number of times we referred to pages").
 */
void UI_HandleHitRate();
//prints the MM.
void UI_HandlePrintMM();
/*
 * prints the IPT, entry by entry.
 * Each entry should be displayed in form
 * "(pid, pageNum, dirty bit, aging reference bit, next, prev)".
 * For free pages (unused) print "(free)".
 */
void UI_HandlePrintMMUTable();
//prints the registers of the aging algorithm, each register on a new line.
void UI_HandlePrintRegisters();
//For each entry in the HAT, prints the list that is pointed by this entry.
void UI_HandlePrintHat();
//switches the system to Monitor mode.
void UI_HandleMonitor();
//switches the system to No Monitor mode.
void UI_HandleNoMonitor();
/*
 * read commands from the file filename.
 * Each command will appear on a new line with the correct syntax. 
 * The UI must deliver all commands to the correct processes.
 * The UI must not hang until a command is completed unless this behavior
 * is explicitly specified for this command i.e. the del command.
 */
void UI_HandleBatchFile(string filename);
#endif	/* _UI_H */

