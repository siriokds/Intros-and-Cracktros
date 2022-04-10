#include "Windows.h"
#include "FastBMP.h"


/*-----------------------------------------------------------------*/
void	TFastBMP::Initialize(void)
/*-----------------------------------------------------------------*/
{
int		i;
BYTE	*x;

	Pixels = new TPLine[Height];
	Gap = Width % 2;
	RowInc = (Width * 2) + Gap;
	Size = RowInc * Height;

	x = (BYTE*)Bits;

	for (i = 0; i < Height; i++)
   	{
		Pixels[i] = (TPLine)x;
       	x += RowInc;
	}

	hDC = CreateCompatibleDC(0);
	SelectObject(hDC,Handle);

}

/*-----------------------------------------------------------------*/
TFastBMP::~TFastBMP()
/*-----------------------------------------------------------------*/
{
	delete Pixels;
}

/*-----------------------------------------------------------------*/
TFastBMP::TFastBMP(int cx, int cy)
/*-----------------------------------------------------------------*/
{
	Width = cx;
	Height = cy;

/*	
DWORD* pColors = (DWORD*)bmInfo.bmiColors;

    pColors[0] = 0x1F << 11;
    pColors[1] = 0x3F << 5;
    pColors[2] = 0x1F;
*/

    bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmInfo.bmiHeader.biWidth = Width;
    bmInfo.bmiHeader.biHeight = -Height;
    bmInfo.bmiHeader.biPlanes = 1;
    bmInfo.bmiHeader.biBitCount = 16;
    bmInfo.bmiHeader.biSizeImage = Width*Height*2;
    bmInfo.bmiHeader.biCompression = BI_BITFIELDS;
    bmInfo.bmiHeader.biClrUsed = 0;
    bmInfo.bmiHeader.biClrImportant = 0;

	Handle = CreateDIBSection(0,
                   &bmInfo,
                   DIB_RGB_COLORS,
                   &Bits,
                   NULL,0);
	Initialize();
}




/*-----------------------------------------------------------------*/
TFastBMP::TFastBMP(TFastBMP *Src)
/*-----------------------------------------------------------------*/
{
	CopyMemory(&bmInfo, &Src->bmInfo, sizeof(BITMAPINFO));

	Width = Src->Width;
	Height = Src->Height;
	Size = Src->Size;

   	Handle = CreateDIBSection(0,
				&bmInfo,
				DIB_RGB_COLORS,
                &Bits,
                NULL,0);

	CopyMemory(Bits, Src->Bits, Size);
	Initialize();
}






/*-----------------------------------------------------------------*/
TFColor* TFastBMP::GetPixel(int x, int y)
/*-----------------------------------------------------------------*/
{
	return &Pix(x,y);	
}

/*-----------------------------------------------------------------*/
void	TFastBMP::SetPixel(int x, int y, BYTE r, BYTE g, BYTE b)
/*-----------------------------------------------------------------*/
{
	if ((x < 0) || (x >= Width) ||
    	(y < 0) || (y >= Height)) return;

	r >>= 3;
	g >>= 2;
	b >>= 3;

    Pix(x,y) = ((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F);
}



/*-----------------------------------------------------------------*/
void	TFastBMP::Draw(HDC hDst, int x, int y)
/*-----------------------------------------------------------------*/
{
	BitBlt(hDst,x,y,Width,Height,hDC,0,0,SRCCOPY);
}

/*-----------------------------------------------------------------*/
void	TFastBMP::Draw(HDC hDst, int x, int y, int cx, int cy)
/*-----------------------------------------------------------------*/
{
	StretchBlt(hDst,x,y,cx,cy,hDC,0,0,Width,Height,SRCCOPY);
}






/*-----------------------------------------------------------------*/
void	TFastBMP::DrawRect(HDC hDst, int x, int y, int sx, int sy,
   							int cx, int cy)
/*-----------------------------------------------------------------*/
{
		BitBlt(hDst,x,y,cx,cy,hDC,sx,sy,SRCCOPY);
}

/*-----------------------------------------------------------------*/
void	TFastBMP::DrawTile(HDC hDst, int tsx, int tsy)
/*-----------------------------------------------------------------*/
{
		BitBlt(
			hDst,
			tsx*16,tsy*16,
			16,16,
			hDC,
			tsx*16,tsy*16,
			SRCCOPY);
}

/*-----------------------------------------------------------------*/
void	TFastBMP::DrawRect(TFastBMP *Dst, int x, int y, int sx, int sy,
   							int cx, int cy)
/*-----------------------------------------------------------------*/
{
BYTE	*d,*s;
int		i;

	if ((cy + sy) > Height)		cy -= ((cy + sy) - Height);
	if ((cx + sx) > Width)		cx -= ((cx + sx) - Width);
	if ((cy + y) > Dst->Height) cy -= ((cy + y) - Dst->Height);
	if ((cx + x) > Dst->Width)  cx -= ((cx + x) - Dst->Width);

	d = (BYTE*)Dst->GetPixel(x,y);
	s = (BYTE*)GetPixel(sx,sy);

	for (i = 0; i < cy; i++)
    {
		CopyMemory((void*)d, (void*)s, cx * 4);
        d += Dst->RowInc;
        s += RowInc;
    }

}

