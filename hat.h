/* 
 * File:   hat.h
 * Author: danni & yaniv
 *
 * Created on May 9, 2010, 7:01 PM
 */

#ifndef _HAT_H
#define	_HAT_H
#include "globaldefs.h"
typedef IPT_t_p* HAT_t; //HAT is just an array of IPT enteries
HAT_t HATArray;

bool InitHAT();
int Hash(PID proccessID,int pageNumber);
#endif	/* _HAT_H */

