//---------------------------------------------------------------------------
#ifndef FCLIBX_H
#define FCLIBX_H
//---------------------------------------------------------------------------

/*
#ifdef __cplusplus
extern "C" {
#endif
*/

#include <xtl.h>

int		FCp_Open(int is50Hz);
void	FCp_Close(void);
void	FCp_Start(void);
void	FCp_Stop(void);
void	FCp_Run(void);
int		FCp_InitModule(char *Module);
int		FCp_Update(void);
void	FCp_MasterVol(int volume);

/*
#ifdef __cplusplus
}
#endif
*/

#endif
