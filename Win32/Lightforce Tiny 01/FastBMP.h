//---------------------------------------------------------------------------
#ifndef FastBMPH
#define FastBMPH
//---------------------------------------------------------------------------

#include "Types.h"

#define	Pix(x,y) (*(Pixels[y]+x))

typedef struct { BYTE b,g,r,a; } TFColor, *PFColor;
typedef	PFColor	TPLine;

class	TFastRGB
{
public:

	int Gap,    // space between scanlines
  		RowInc, // distance to next scanline
  		Size,   // size of Bits
  		Width,
  		Height;

	TPLine*	Pixels;
	void*	Bits;
	SWORD*	ScanBuff;
};



class	TFastBMP : public TFastRGB
{
private:
		void	Initialize(void);
public:
		HANDLE				Handle;
        HDC					hDC;
        BITMAPINFO			bmInfo;

        ~TFastBMP();
        TFastBMP(int cx = 256, int cy = 256);
        TFastBMP(char *lpstrFileName);
        TFastBMP(int ResID, HANDLE hInst);
        TFastBMP(TFastBMP *Src);

        TFColor* GetPixel(int x, int y);
        void	SetPixel(int x, int y, TFColor c);
        void	SetPixel(int x, int y, BYTE r, BYTE g, BYTE b);

        void	Draw(HDC hDst, int x, int y);
		void	Draw(HDC hDst, int x, int y, int cx, int cy);

        void	DrawRect(HDC hDst, int x, int y, int sx, int sy,
        				int cx, int cy);

        void	DrawRect(TFastBMP *hDst, int x, int y, int sx, int sy,
        				int cx, int cy);

		void	DrawDouble(TFastBMP *Dst);
		void	DrawDoubleTile(TFastBMP *Dst, int tsx, int tsy);
		void	DrawDoubleTile75(TFastBMP *Dst, int tsx, int tsy);
		void	DrawTile(HDC hDst, int tsx, int tsy);
		void	Fill(BYTE r, BYTE g, BYTE b, int sY, int nY);

};



#endif
