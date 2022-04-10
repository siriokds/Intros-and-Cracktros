//---------------------------------------------------------------------------
#ifndef MtxH
#define MtxH
//---------------------------------------------------------------------------

#include "Windows.h"


class	TMtx 
{
private:
	int	SizeX, SizeY;
	int	*Data;

	int	Pos(int x, int y);
public:
	TMtx(int cx, int cy);
	~TMtx();
	void Fill(int v);
	void Set(int x, int y, int v);
	void SetRect(int px, int py, int cx, int cy, int v);
	int	 Get(int x, int y);
	int	 GetSizeX(void) { return SizeX; }
	int	 GetSizeY(void) { return SizeY; }
	BOOL Valid(void) { return (Data != NULL); }
};

#endif	//MtxH


