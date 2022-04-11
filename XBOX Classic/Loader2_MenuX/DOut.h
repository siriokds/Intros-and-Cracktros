#ifndef DOUT_H_
#define DOUT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Types.h"

int		DOut_Open(void* (*UserF)(int), int VMode50);
int     DOut_Start(void);
void	DOut_RunOnce(void);
void	DOut_Close(void);
void	DOut_SetVol(int vol);
void	DOut_SetVolPerc(int vol);


#ifdef __cplusplus
}
#endif

#endif

