#ifndef RTX_H_
#define RTX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Types.h"

double	__stdcall _msin(double x);
double	__stdcall _mcos(double x);
int		__ftol(double x);
void 	copyROW16(char *src, char *dst);
void 	copyROW8(char *src, char *dst);
void 	fastSet(UBYTE *dst, int val, int todo);
void 	fastCpy(UBYTE *dst, UBYTE *src, int todo);



#ifdef __cplusplus
}
#endif

#endif
