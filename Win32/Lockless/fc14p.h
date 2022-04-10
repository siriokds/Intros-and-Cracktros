#ifndef _FC14P_H
#define _FC14P_H

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>

void	__stdcall FCp_Open(DWORD User);
void	__stdcall FCp_InitModule(char *Module);
void	__stdcall FCp_Start(void);
void	__stdcall FCp_Close(void);

#ifdef __cplusplus
}
#endif

#endif
