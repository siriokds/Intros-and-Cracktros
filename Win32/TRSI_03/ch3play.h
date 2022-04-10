//;------------------------------------------
//        Tiny Win32 CHP3 PlayRoutine
//    Version 2.01 - December 17th, 2002
//
//        from SiRioKD / Lockless 
//------------------------------------------

#ifndef _CH3PLAY_H
#define _CH3PLAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>

void	__stdcall chpOpen(unsigned char *Module, DWORD User);
void	__stdcall chpStart(void);
void	__stdcall chpClose(void);

#ifdef __cplusplus
}
#endif

#endif
