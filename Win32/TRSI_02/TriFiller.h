#ifndef TRIFILLER_H_
#define TRIFILLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Types.h"

void	__stdcall TriDraw(int *setting);
void	__stdcall TriGet(int *global);


void	PutTriangle(int *set, 
					int p1_x, int p1_y, 
					int p2_x, int p2_y, 
					int p3_x, int p3_y, int col256)
{
	set[ 6] = col256;
	
	set[ 7] = p1_x;	set[ 8] = p1_y;
	set[ 9] = p2_x;	set[10] = p2_y;
	set[11] = p3_x;	set[12] = p3_y;
	TriDraw(set);
	
}

#ifdef __cplusplus
}
#endif

#endif
