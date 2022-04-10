//;------------------------------------------
//        Tiny Win32 CHP Playroutine
//    Version 1.00a - December 8th, 2002
//
//        from SiRioKD / Lockless 
//------------------------------------------

#ifndef _CHPLAY_H
#define _CHPLAY_H

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
