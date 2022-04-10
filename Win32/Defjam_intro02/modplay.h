#ifndef _MODPL_H
#define _MODPL_H

/*
#ifdef __cplusplus
extern "C" {
#endif
*/

#include "Windows.h"
#include "Types.h"

void	MOD_InitMusic(SBYTE *ModAddr);
void	MOD_StartMusic(void);
void	MOD_EndMusic(void);

/*
#ifdef __cplusplus
}
#endif
*/

#endif //_MODPL_H
