#include "Windows.h"
#include "FastBMP.h"


#define P75(i) (((i) * 100) / 100)

/*-----------------------------------------------------------------*/
void	TFastBMP::Initialize(void)
/*-----------------------------------------------------------------*/
{
int		i;
BYTE	*x;

	Pixels = new TPLine[Height];
	Gap = Width % 4;
	RowInc = (Width * 4) + Gap;
	Size = RowInc * Height;

	x = (BYTE*)Bits;

	for (i = 0; i < Height; i++)
    {
		Pixels[i] = (TPLine)x;
        x += RowInc;
	}

	hDC = CreateCompatibleDC(0);
	SelectObject(hDC,Handle);


   	ScanBuff = new SWORD[65536];

	for (i = 0; i < 65536; i++)
		ScanBuff[i] = P75(i & 0xFF) | P75( (i >> 8) & 0xFF );

}

/*-----------------------------------------------------------------*/
TFastBMP::~TFastBMP()
/*-----------------------------------------------------------------*/
{
	delete ScanBuff;
	delete Pixels;
}

/*-----------------------------------------------------------------*/
TFastBMP::TFastBMP(int cx, int cy)
/*-----------------------------------------------------------------*/
{
	Width = cx;
	Height = cy;

    bmInfo.bmiHeader.biSize = sizeof(bmInfo.bmiHeader);
    bmInfo.bmiHeader.biWidth = Width;
    bmInfo.bmiHeader.biHeight = -Height;
    bmInfo.bmiHeader.biPlanes = 1;
    bmInfo.bmiHeader.biBitCount = 32;
    bmInfo.bmiHeader.biCompression = BI_RGB;

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
TFastBMP::TFastBMP(char *lpstrFileName)
/*-----------------------------------------------------------------*/
{
BITMAP	Bmp;
HDC		memDC;
HANDLE	hBmp;

	hBmp = LoadImage(0,lpstrFileName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

	GetObject(hBmp, sizeof(Bmp), &Bmp);
	Width =  Bmp.bmWidth;
	Height = Bmp.bmHeight;

	Size = ((Width * 4)+(Width % 4)) * Height;

    bmInfo.bmiHeader.biSize = sizeof(bmInfo.bmiHeader);
    bmInfo.bmiHeader.biWidth = -Width;
    bmInfo.bmiHeader.biHeight = Height;
    bmInfo.bmiHeader.biPlanes = 1;
    bmInfo.bmiHeader.biBitCount = 32;
    bmInfo.bmiHeader.biCompression = BI_RGB;

	Handle = CreateDIBSection(0,
                   &bmInfo,
                   DIB_RGB_COLORS,
                   &Bits,
                   NULL,0);

	memDC = GetDC(0);
	GetDIBits(memDC, (HBITMAP)hBmp, 0, Height, Bits, &bmInfo, DIB_RGB_COLORS);
	ReleaseDC(0,memDC);
	DeleteObject(hBmp);
	Initialize();
}



/*-----------------------------------------------------------------*/
TFastBMP::TFastBMP(int ResID, HANDLE hInst)
/*-----------------------------------------------------------------*/
{
BITMAP	Bmp;
HDC		memDC;
HANDLE	hBmp;

	hBmp = LoadImage((HINSTANCE)hInst,(LPCTSTR)ResID,IMAGE_BITMAP,0,0,0);

	GetObject(hBmp, sizeof(Bmp), &Bmp);
	Width =  Bmp.bmWidth;
	Height = Bmp.bmHeight;

	Size = ((Width * 4)+(Width % 4)) * Height;

    bmInfo.bmiHeader.biSize = sizeof(bmInfo.bmiHeader);
    bmInfo.bmiHeader.biWidth = Width;
    bmInfo.bmiHeader.biHeight = -Height;
    bmInfo.bmiHeader.biPlanes = 1;
    bmInfo.bmiHeader.biBitCount = 32;
    bmInfo.bmiHeader.biCompression = BI_RGB;

	Handle = CreateDIBSection(0,
                   &bmInfo,
                   DIB_RGB_COLORS,
                   &Bits,
                   NULL,0);

	memDC = GetDC(0);
	GetDIBits(memDC, (HBITMAP)hBmp, 0, Height, Bits, &bmInfo, DIB_RGB_COLORS);
	ReleaseDC(0,memDC);
	DeleteObject(hBmp);
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

    Pix(x,y).r = r;
    Pix(x,y).g = g;
    Pix(x,y).b = b;
}


void	TFastBMP::SetPixel(int x, int y, TFColor c)
{
	if ((x < 0) || (x >= Width) ||
    	(y < 0) || (y >= Height) ) return;

    Pix(x,y).r = c.r;
    Pix(x,y).g = c.g;
    Pix(x,y).b = c.b;
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
	SetStretchBltMode(hDst,STRETCH_DELETESCANS);
	StretchBlt(hDst,x,y,cx,cy,hDC,0,0,Width,Height,SRCCOPY);
}


/*-----------------------------------------------------------------*/
void	TFastBMP::DrawDoubleTile(TFastBMP *Dst, int tsx, int tsy)
/*-----------------------------------------------------------------*/
{
int		i, gap1, gap2;
BYTE	*ls, *ld;
DWORD	*s1, *s2, *d1, *d2;

	ls = (BYTE*)Bits;
	ld = (BYTE*)Dst->Bits;
	ls += (  ((tsy * 8) * RowInc) + ((tsx * 8) * 4)  );
	ld += (  ((tsy * 16) * Dst->RowInc) + ((tsx * 16) * 4)  );


	s1 = (DWORD*)ls;
	s2 = (DWORD*)(ls + 4);
	d1 = (DWORD*)ld;
	d2 = (DWORD*)(ld + Dst->RowInc);

	gap1 = RowInc >> 2;
	gap2 = Dst->RowInc >> 1;

	for (i = 0; i < 8; i++)
	{
		__asm {

			mov	esi,s1
			mov	ecx,s2
			mov	edi,d1
			mov	edx,d2
//---
			mov	eax,[esi]
			mov	ebx,[ecx]
			mov	[edi],eax
			mov	[edi+8],ebx
			mov	[edi+4],eax
			mov	[edi+12],ebx
			and	eax,0FEFEFEFEh
			and	ebx,0FEFEFEFEh
			shr	eax,1
			shr	ebx,1
			mov	[edx],eax
			mov	[edx+8],ebx
			mov	[edx+4],eax
			mov	[edx+12],ebx

			mov	eax,[esi+8]
			mov	ebx,[ecx+8]
			mov	[edi+16],eax
			mov	[edi+24],ebx
			mov	[edi+20],eax
			mov	[edi+28],ebx
			and	eax,0FEFEFEFEh
			and	ebx,0FEFEFEFEh
			shr	eax,1
			shr	ebx,1
			mov	[edx+16],eax
			mov	[edx+24],ebx
			mov	[edx+20],eax
			mov	[edx+28],ebx

			add	esi,16
			add	ecx,16
			add	edi,32
			add	edx,32

			mov	eax,[esi]
			mov	ebx,[ecx]
			mov	[edi],eax
			mov	[edi+8],ebx
			mov	[edi+4],eax
			mov	[edi+12],ebx
			and	eax,0FEFEFEFEh
			and	ebx,0FEFEFEFEh
			shr	eax,1
			shr	ebx,1
			mov	[edx],eax
			mov	[edx+8],ebx
			mov	[edx+4],eax
			mov	[edx+12],ebx

			mov	eax,[esi+8]
			mov	ebx,[ecx+8]
			mov	[edi+16],eax
			mov	[edi+24],ebx
			mov	[edi+20],eax
			mov	[edi+28],ebx
			and	eax,0FEFEFEFEh
			and	ebx,0FEFEFEFEh
			shr	eax,1
			shr	ebx,1
			mov	[edx+16],eax
			mov	[edx+24],ebx
			mov	[edx+20],eax
			mov	[edx+28],ebx
		}


		s1 += gap1;
		s2 += gap1;
		d1 += gap2;
		d2 += gap2;
	}
}





/*-----------------------------------------------------------------*/
void	TFastBMP::DrawDoubleTile75(TFastBMP *Dst, int tsx, int tsy)
/*-----------------------------------------------------------------*/
{
int		i,j, gap1, gap2;
BYTE	*ls, *ld;
SWORD	*s1, *d1, *d2, p1;

	ls = (BYTE*)Bits;
	ls += (  ((tsy * 8) * RowInc) + ((tsx * 8) * 4)  );

	ld = (BYTE*)Dst->Bits;
	ld += (  ((tsy * 16) * Dst->RowInc) + ((tsx * 16) * 4)  );


	s1 = (SWORD*)ls;
	d1 = (SWORD*)ld;
	d2 = (SWORD*)(ld + Dst->RowInc);

	gap1 = RowInc >> 1;
	gap2 = Dst->RowInc;


	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 16; j++)
		{
			p1 = s1[j];

			d1[(j*2)+0] = p1;

//			d1[(j*2)+3] = p1;


//			p2 = ScanBuff[p1];

//			d2[(j*2)+0] = p2;

//			d2[(j*2)+1] = p2;
		}

		s1 += gap1;
		d1 += gap2;
		d2 += gap2;
	}
}

/*-----------------------------------------------------------------*/
void	TFastBMP::DrawDouble(TFastBMP *Dst)
/*-----------------------------------------------------------------*/
{
int		i,w;
DWORD	*s, *d1, *d2;
BYTE	*ls, *ld;

//	if ((Width != Dst->Width*2) || (Height != Height*2)) return;

	ls = (BYTE*)Bits;
	ld = (BYTE*)Dst->Bits;
	w = Width;

	for (i = 0; i < Height; i++)
	{
		s  = (DWORD*)ls;
		d1 = (DWORD*)ld;
		d2 = (DWORD*)(ld + Dst->RowInc);

		__asm {
			mov	esi,s
			mov	edi,d1
			mov	edx,d2
			mov	ecx,w
TVStretch0:
			mov	eax,[esi]
			add	esi,4

			mov	[edi],eax
			mov	[edi+4],eax

			shr	eax,1
			and	eax,07F7F7F7Fh

			mov	[edx],eax
			mov	[edx+4],eax


			add	edi,8
			add	edx,8
			dec	ecx
			jnz	TVStretch0
		}

		ls += RowInc;
		ld += Dst->RowInc<<1;
	}
}




/*-----------------------------------------------------------------*/
void	TFastBMP::Fill(BYTE r, BYTE g, BYTE b, int sY, int nY)
/*-----------------------------------------------------------------*/
{
int		j,c;
BYTE	*d;
DWORD	a;

	if (sY >= Height) return;

	if ((nY <= 0) || (sY <= 0))
	{
		sY = 0;
		nY = Height;
	}

	a = ((r << 16) | (g << 8) | b) & 0x00FFFFFF;
	c = Width;
	d = ((BYTE*)Bits) + (RowInc * sY);

	for (j = sY; j < Height; j++)
	{
		__asm {
			mov	ecx,c
			mov	edi,d
			mov	eax,a
			cld
			rep stosd
		}	
		d += RowInc;
	}
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

