/* 
 * File:   GlobalDefs.h
 * Author: danni & yaniv
 *
 * Created on May 9, 2010, 5:58 PM
 */

#ifndef _GLOBALDEFS_H
#define	_GLOBALDEFS_H


typedef unsigned char bool;
typedef char* string;

#define NULL ((void *)0)
#define OUT
#define IN
#define FALSE 0
#define TRUE (!FALSE)



//=========================Debugging functions===============================
#define DEBUG

//DEBUGGING
#define __ASSERT_VOID_CAST (void)
#define ASSERT_ALWAYS(expr)							\
      ((expr)								\
       ? __ASSERT_VOID_CAST (0)						\
       : exit(printf("=========assertion failed on line:%d\t file:%s\n",__LINE__,__FILE__)))
#ifdef DEBUG
#define ASSERT(expr)							\
      ((expr)								\
       ? __ASSERT_VOID_CAST (0)						\
       : exit(printf("=========assertion failed on line:%d\t file:%s\n",__LINE__,__FILE__)))
#define ASSERT_RUN(expr) (expr)
#define ASSERT_PRINT(expr...)  (printf(expr))
#else
#define ASSERT(expr)        __ASSERT_VOID_CAST (0)
#define ASSERT_RUN(expr)    __ASSERT_VOID_CAST (0)
#define ASSERT_PRINT(expr...)  __ASSERT_VOID_CAST (0)
#endif

#endif	/* _GLOBALDEFS_H */

