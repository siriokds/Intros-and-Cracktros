#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <windows.h>
#include "Muzak.h"
#include "fc14p.h"
#include "tdx.h"
#include "Intro.h"
#include "Gfx.h"
#include "TriFiller.h"
#include "scroll.h"



#define	 TXTCOLS	52
#define	 TXTROWS	20
#define	 CENTERX	((640-(TXTCOLS*8))/2)
#define	 CENTERY	((480-(TXTROWS*12))/2)


UBYTE	Frame[640*480];
UBYTE	FramT[(TXTCOLS*8)*(TXTROWS*12)];

LRESULT CALLBACK MainWndProc( HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam );



HINSTANCE	ghInstance;
HWND		ghMainWnd;
int			g_bActive;




int     pStar[2*12];
int     pStar2[2*12];

int		TriSetting[13] = 
{
	0,640-1, 0,480-1,
	(int)(Frame), 640, 3,
	0,0, 400,0, 400,400
};



int		cols[] = { 

	  0,18,26
	, 0,26,18 
	, 0,26,26 
	,26,18,18

	,26,26,18
	,26,18,26
	,18,26,18 
	,18,18,26

};

short	SinTab[256];



char	fntMtx[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-()><:.,!?^_é";

struct	scrInfo		scroll_Info;

char	xt[] = "        ";

char	mytext[] = 

// 0         1         2         3         4
// 0123456789012345678901234567890123456789012345678901
// -------------------------||-------------------------
  "____________________________________________________"
  "                                                    "
  "               >  L O C K L E S S  <                "
  "                                                    "
  "____________________________________________________"
  "                                                    "
  "                                                    "
  "                SIRIOKD OF LOCKLESS                 "
  "                                                    "
  "                  PROUDLY PRESENTS                  "
  "                                                    "
  "                                                    "
  "                 > DELUXE PAINT 4 <                 "
  "                                                    "
  "                FROM ELECTRONIC ARTS                "
  "                                                    "
  "                                                    "
  "       VISIT OUR WEBSITE AT: WWW.LOCKLESS.COM       "
  "                                                    "
  "____________________________________________________"
  "s";

/*----------------------------------------------------------*/
void	Build_SinTab(void)
/*----------------------------------------------------------*/
{
	SinTab[0] = 256;

	__asm
	{
		pushad

		finit
		lea		edi,SinTab
		xor		ecx,ecx
		inc		ch

		fild	[edi]				// (256)

		fldpi						// (PI )(256)
		fadd	st, st				// (PI2)(256)
		fdiv	st, st(1)			// (PIi)(256)
		fldz						// (ang)(PIi)(256)
		fldz						// (ang)(ang)(PIi)(256)
calcTab:
		fsin						// (anS)(ang)(PIi)(256)
		fmul	st, st(3)			// (snM)(ang)(PIi)(256)
		fistp	[edi]				// (anO)(PIi)(256)
		add		edi,2

		fadd	st,st(1)			// (anN)(PIi)(256)
		fld		st(0)				// (anN)(anN)(PIi)(256)

		loop	calcTab

		finit

		popad
	}

}





/*----------------------------------------------------------*/
void    DrawStar(int posX, int posY, int radius, int angleSta)
/*----------------------------------------------------------*/
{
int i;

int		incr = (256 * 256) / 10;
int		angle = ((angleSta+128)&255) * 256;
int     radi = radius;

        pStar[0] = posX;
        pStar[1] = posY;

        for (i = 1; i < 11; i++)
        {
			int	rA = angle / 256;
			pStar[(i*2)+0] = posX + ((SinTab[ rA & 255 ] * radi) / 256);
			pStar[(i*2)+1] = posY + ((SinTab[ (rA+64)&255 ] * radi) / 256);

            angle = (angle + incr) & 0xFFFF;

            if (radi == radius) radi = radius>>1; else radi = radius;
        }


        for (i = 1; i < 11; i++)
        {

			TriSetting[7] = pStar[0];
			TriSetting[8] = pStar[1];

			int idx = i << 1;
			TriSetting[9] = pStar[idx+0];
			TriSetting[10] = pStar[idx+1];

			idx = ((i+1) < 11 ? (i+1) : 1) << 1;
			TriSetting[11] = pStar[idx+0];
			TriSetting[12] = pStar[idx+1];

			TriDraw(TriSetting);
		}
}



/*----------------------------------------------------------*/
void    DrawStar2(int posX, int posY, int radius, int angleSta)
/*----------------------------------------------------------*/
{
int i;

int		incr = (256 * 256) / 10;
int		angle = ((angleSta+128)&255) * 256;
int     radi = radius;

        pStar2[0] = posX;
        pStar2[1] = posY;

        for (i = 1; i < 11; i++)
        {
			int	rA = angle / 256;
			pStar2[(i*2)+0] = posX + ((SinTab[ rA & 255 ] * radi) / 256);
			pStar2[(i*2)+1] = posY + ((SinTab[ (rA+64)&255 ] * radi) / 256);

            angle = (angle + incr) & 0xFFFF;

            if (radi == radius) radi = radius>>1; else radi = radius;
        }


        for (i = 1; i < 11; i++)
        {

			TriSetting[7] = pStar2[0];
			TriSetting[8] = pStar2[1];

			int idx = i << 1;
			TriSetting[9] = pStar2[idx+0];
			TriSetting[10] = pStar2[idx+1];

			idx = ((i+1) < 11 ? (i+1) : 1) << 1;
			TriSetting[11] = pStar2[idx+0];
			TriSetting[12] = pStar2[idx+1];

			TriDraw(TriSetting);
		}
}



/*----------------------------------------------------------*/
void	DrawSirioKD(void)
/*----------------------------------------------------------*/
{
//static int posT = 128;
static int rota2 = 0;

	int posX = 604;
	

//int	posX = (int)(float(sinetab[posT]) * 3.23f) + 36;
//	posT = (posT + 5) & 511;
	
	DrawStar2(posX+10,465, 60, rota2-=2);

	static int ofstabY = 128;
	ofstabY = (ofstabY + 9) & 511;

	UBYTE *src = imx + 778 + 400;
	UBYTE *dst = Frame+(640*460)+posX - (640*((sinetab[ofstabY]/10)-2));

	for (int i = 0; i < 9; i++)
	{
		int offY = (i*640*2);

		for (int j = 0; j < 31; j++)
		{
			UBYTE v = src[(i<<9)+j];
			if (v!=0) 
			{

				dst[ offY + j] = v;
				dst[ offY + j + 640] = v;
			}
		}
	}

}


/*----------------------------------------------------------*/
void	UpdateFrame(void)
/*----------------------------------------------------------*/
{
	fastSet(Frame,1,640*480);


static int rota = 0;
static int radi = 800;
	DrawStar(320,240, radi, rota++); if (radi>240) radi-=2;


	for (int i = 0; i < (TXTROWS*12); i++)
	{
		int offSrc = i * (TXTCOLS*8);

		int offDst = ((i+CENTERY)*640) + CENTERX;

		for (int j = 0; j < (TXTCOLS*8); j++)
		{
			if (FramT[offSrc+j]) 
			{
				Frame[offDst+j+(1924)] |= 0x10;
				Frame[offDst+j] = FramT[offSrc+j];
			}
		}
	}
			


	DrawSirioKD();


/*
	if (GetAsyncKeyState(VK_F4) & 0x8000)
	{

		FILE *fp = fopen("c:\\1shot.scx","wb");

		fputc(0x52,fp);	fputc(0x49,fp);	fputc(0x58,fp);	fputc(0x33,fp);

		fputc(0x80,fp);	fputc(0x02,fp);
		fputc(0xE0,fp);	fputc(0x01,fp);

		fputc(0xAF,fp);	fputc(0x00,fp);

		fwrite(imx+10,1,768,fp);

		for (int i = 0; i < 480; i++)
		{
			fwrite(Frame+(i*640),1,640,fp);
		}


		fclose(fp);
	}
*/


	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);
	tdxUpdate(Frame,640);
	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_ABOVE_NORMAL);
}


/*--------------------------------------------------------------*/
void	setPal(UBYTE *pal, int idx, int r, int g, int b, int delt)
/*--------------------------------------------------------------*/
{
	pal += (idx*3);


	r += delt; if (r < 0) r = 0; if (r > 63) r = 63;
	g += delt; if (g < 0) g = 0; if (g > 63) g = 63;
	b += delt; if (b < 0) b = 0; if (b > 63) b = 63;

	pal[0] = r; pal[1] = g; pal[2] = b;
}





/*--------------------------------------------------------------*/
int WINAPI WinMain( HINSTANCE hInstance,
    HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int nCmdShow )
/*--------------------------------------------------------------*/
{
	Build_SinTab();


//----------

	scroll_Info.scrFont = (char*)imx+778;
	scroll_Info.scrFontRow = 512;
	scroll_Info.scrDest = (char*)FramT;
	scroll_Info.scrDestRow = TXTCOLS*8;

	scroll_Info.scrText = mytext;
	scroll_Info.scrFontMtx = fntMtx;
	scroll_Info.scrFontW = 8;
	scroll_Info.scrFontH = 10;
	scroll_Info.col = TXTCOLS;


	scrInit(&scroll_Info);
	



WNDCLASS	wc;
MSG			msg;
HWND		hWnd;

	if( !hPrevInstance )
	{
      wc.lpszClassName = "WINtroAppClass";
      wc.lpfnWndProc = MainWndProc;
      wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
      wc.hInstance = hInstance;
      wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
      wc.hCursor = LoadCursor( NULL, IDC_ARROW );
      wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
      wc.lpszMenuName = "WINtroAppMenu";
      wc.cbClsExtra = 0;
      wc.cbWndExtra = 0;

      RegisterClass( &wc );
	}

	ghInstance = hInstance;

	hWnd = CreateWindowEx(WS_EX_TOPMOST,
	  "WINtroAppClass",
      "WINtro",
	  WS_POPUP, 
      0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),
      NULL,NULL,hInstance,NULL
	);


	ghMainWnd = hWnd;

	ShowWindow(hWnd, SW_SHOWNORMAL);
	SetFocus(hWnd);

	tdxOpen((DWORD)hWnd);
	FCp_Open((DWORD)hWnd, 120);
	FCp_InitModule((char*)Muzak);
	while (!scrExecute(&scroll_Info)) ;


UBYTE	*myPal = imx+10;
int		palN = timeGetTime() & 7;

//		palN = 0;
int		*pCols = cols + (palN * 3);

UBYTE	r = pCols[0];
UBYTE	g = pCols[1];
UBYTE	b = pCols[2];

	setPal(myPal,0x01, r,g,b, 0);
	setPal(myPal,0x03, r,g,b, 2);
//	setPal(myPal,0x04, r,g,b, 4);

	setPal(myPal,0x11, r,g,b,-4);
	setPal(myPal,0x13, r,g,b,-2);
//	setPal(myPal,0x14, r,g,b, 0);


	tdxSetPal(myPal);


	Sleep(100);
//----------
	FCp_Start();


	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_ABOVE_NORMAL);
    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!GetMessage(&msg, NULL, 0, 0)) return msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else if (g_bActive) UpdateFrame();
             else           WaitMessage();
    }




	return msg.wParam;
}




/*--------------------------------------------------------------*/
LRESULT CALLBACK MainWndProc( HWND hWnd, UINT msg, 
							  WPARAM wParam,LPARAM lParam )
/*--------------------------------------------------------------*/
{

	switch( msg ) 
	{
		case WM_ACTIVATEAPP:
            g_bActive = (wParam == WA_ACTIVE) || 
						(wParam == WA_CLICKACTIVE);
			return 0L;

		case WM_DESTROY:
			PostQuitMessage(0);
		break;

        case WM_KEYDOWN:
            switch (wParam)
            {
                case VK_ESCAPE:
					g_bActive = 0;
					FCp_Close();
					tdxClose();

                    PostMessage(hWnd, WM_CLOSE, 0, 0);
                    return 0L;
            }
            break;

      default:
		return( DefWindowProc( hWnd, msg, wParam, lParam ));
	}
	return 0;
}



