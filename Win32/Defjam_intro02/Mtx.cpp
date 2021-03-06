#include "Mtx.h"

/*-------------------------------------------------------------------*/
		TMtx::~TMtx()
/*-------------------------------------------------------------------*/
{
	if (Data != NULL) delete Data;
	SizeX = 0;
	SizeY = 0;
}

/*-------------------------------------------------------------------*/
		TMtx::TMtx(int cx, int cy)
/*-------------------------------------------------------------------*/
{
	SizeX = cx;
	SizeY = cy;
	Data = new int[SizeX * SizeY];
	Fill(0);
}

/*-------------------------------------------------------------------*/
void	TMtx::Fill(int v)
/*-------------------------------------------------------------------*/
{
int	x,y;

	if (Data == NULL) return;

	for (y = 0; y < SizeY; y++)
		for (x = 0; x < SizeX; x++)
			Set(x, y, v);
}

/*-------------------------------------------------------------------*/
int		TMtx::Pos(int x, int y)
/*-------------------------------------------------------------------*/
{
	if (x < 0) x = 0;
	if (x >= SizeX) x = SizeX-1;

	if (y < 0) y = 0;
	if (y >= SizeY) y = SizeY-1;

	return ((y * SizeX) + x);
}

/*-------------------------------------------------------------------*/
void	TMtx::Set(int x, int y, int v)
/*-------------------------------------------------------------------*/
{
	if (Data == NULL) return;

	Data[Pos(x,y)] = v;
}



/*-------------------------------------------------------------------*/
int		TMtx::Get(int x, int y)
/*-------------------------------------------------------------------*/
{
	if (Data == NULL) return -1;

	return Data[Pos(x,y)];
}



/*-------------------------------------------------------------------*/
void	TMtx::SetRect(int px, int py, int cx, int cy, int v)
/*-------------------------------------------------------------------*/
{
int	x,y;

	if (Data == NULL) return;

	for (y = py; y < py+cy; y++)
		for (x = px; x < px+cx; x++)
			Set(x, y, v);
}


