#ifndef _MODPL_H
#define _MODPL_H

#include <xtl.h>
#include "Types.h"


int		MOD_Open(SBYTE *ModAddr, int is50hz);
void	MOD_Start(void);
void	MOD_End(void);
void	MOD_Run(void);


#endif //_MODPL_H
