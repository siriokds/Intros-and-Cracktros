//;------------------------------------------
//        Tiny Win32 CHP3 PlayRoutine
//    Version 2.01 - December 17th, 2002
//
//        from SiRioKD / Lockless 
//------------------------------------------

#ifndef _CH3PLAYX_H
#define _CH3PLAYX_H
/*
#ifdef __cplusplus
extern "C" {
#endif
*/
#include <xtl.h>

void	__stdcall chpOpen(unsigned char *Module, int is50Hz);
void	__stdcall chpStart(void);
void	__stdcall chpClose(void);
void	__stdcall chpRun(void);

/*
#ifdef __cplusplus
}
#endif
*/
#endif
