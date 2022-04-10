#include <stdlib.h>
#include <stdio.h>
extern "C" const int _fltused = 0;
float		scaleval;

#include <windows.h>
#include <objbase.h>
#include "rtlib2.h"
//#include <math.h>
#include "timing.h"
#include "pdx03.h"
//#include "mod.h"
#include "ch3play.h"
#include "tdx.h"
#include "Intro.h"
#include "TriFiller.h"
#include "Gfx.h"
#include "scroll.h"
#include "VLetters.h"

#include "Fntz.h"
#include "bck64c.h"
#include "degif.h"



#define	 TXTCOLS	52
#define	 TXTROWS	20
#define	 CENTERX	((640-(TXTCOLS*8))/2)
#define	 CENTERY	((480-(TXTROWS*16))/2)

UBYTE	Palette[768];
UBYTE	Background[640*480];
UBYTE	Palette2[768];
UBYTE	Fontground[664*16];

UBYTE	Frame[640*480];
UBYTE	FramT[(TXTCOLS*8)*(TXTROWS*16)];

LRESULT CALLBACK MainWndProc( HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam );



char* szTitle = "TRSI";
char* szWindowClass = "TRSI";
HINSTANCE	ghInstance;
HWND		ghMainWnd;
int			g_bActive;

ticker_t	timer;




int     pStar[2*12];
int     pStar2[2*12];

int		TriSetting[13] = 
{
//	0, 640, 0,480,
	16, 622, 18,461,
	(int)(Frame), 640, 3,
	0,0, 400,0, 400,400
};


/*
#define	COL1 22
#define	COL2 30


int		cols[] = 
{ 
	    0,COL1,COL2
	,   0,COL2,COL1 
	,   0,COL2,COL2 
	,COL2,COL1,COL1

	,COL2,COL2,COL1
	,COL2,COL1,COL2
	,COL1,COL2,COL1 
	,COL1,COL1,COL2
};

*/






char	fntMtx[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz!?.,'-+@/><:";

struct	scrInfo		scroll_Info;

char	page1[] = 
// 0         1         2         3         4
// 0123456789012345678901234567890123456789012345678901
// -------------------------||-------------------------
  "£"
  "£"
  "£"
  "£"
  "  TRISTAR AND RED SECTOR INC. PRESENTS:£"
  "£"
  "         XMEN-2 +4 TRAINER£"
  "£"
  "£"
  "£"
  "         TRAINED BY  > SETH GECKO / TRSI <£"
  "£"
  "           GAME CRACKED BY  > FAIRLIGHT <£"
  "£"
  "£"
  "q"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@";



char	page2[] = 
// 0         1         2         3         4
// 0123456789012345678901234567890123456789012345678901
// -------------------------||-------------------------
  "£"
  "£"
  "£"
  "      OUR GOLDEN REGARDS GO OUT TO:£"
  "£"
  "£"
  "£"
  "         FAIRLIGHT - QUARTEX - SECTION 8£"
  "£"
  "        DANISH GOLD - KALISTO - RAZOR 1911£"
  "£"
  "          IMMERSION - LOCKLESS - DEVIANCE£"
  "£"
  "           MYTH - SCOOPEX - THE SILENTS£"
  "£"
  "         GENESIS - SOUNDDRINKER - PARADOX£"
  "£"
  "              HAUJOBB - FARBRAUSCH£"
  "£"
  "£"
  "w"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";


char	page3[] = 
// 0         1         2         3         4
// 0123456789012345678901234567890123456789012345678901
// -------------------------||-------------------------
  "£"
  "£"
  "     INTRO CREDITS:£"
  "£"
  "£"
  "   INTRO CODING BY ......SIRIOKD/TRSI£"
  "£"
  "   2D FONT BY ...........ADEC/TRSI£"
  "£"
  "   3D FONT BY ...........SIRIOKD/TRSI£"
  "£"
  "    MUZAK REPLAYER BY ....SIRIOKD/TRSI£"
  "£"
  "     MUZAK BY .............MONTY£"
  "£"
  "w"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";



/*
char	page1[] = 
  "PAGE1@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "q";


char	page2[] = 
  "PAGE2@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "w";

char	page3[] = 
  "PAGE3@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "w";


char	page4[] = 
  "PAGE4@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
  "w";
*/





char	*pages[] = { page1, page2, page3, 0 };



/*
void	Build_SinTab(void)
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

*/






/*
void    DrawStar(int posX, int posY, int radius, int angleSta)
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


		TriSetting[6] = 4;
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



void    DrawStar2(int posX, int posY, int radius, int angleSta)
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


		TriSetting[6] = 4;
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
*/


/*----------------------------------------------------------*/
void	DrawSirioKD(void)
/*----------------------------------------------------------*/
{
	//static int posT = 128;
static int rota2 = 0;

	int posX = 570;
	
	UBYTE *src = Fontground + 600;

	static float vY = 0.0f;

	UBYTE *dst = Frame+(640*448)+posX+18 - (640 * int(sin(vY)*30) );
	vY += (3.14159265358979323846264338328f / 95) * scaleval;
	if (vY > 3.14159265358979323846264338328f) vY -= 3.14159265358979323846264338328f;


	for (int i = 0; i < 10; i++)
	{
		int offY = (i*640);

		for (int j = 0; j < 32; j++)
		{
			UBYTE v = src[(i*664)+j];
			if (v!=131) dst[ offY + j] = v;
		}
	}

}





/*----------------------------------------------------------*/
void	UpdateFrame(void)
/*----------------------------------------------------------*/
{
	scaleval = timer.Passed() * 0.1f;
	if (scaleval < 0) scaleval = 1.0f;

//	fastSet(Frame,63,640*480);
	fastCpy(Frame+640,Background+640,640*478);




	vfont_run(TriSetting);

	
	
	DrawSirioKD();

	scrExecute(&scroll_Info);


	for (int i = 0; i < (TXTROWS*16); i++)
	{
		int offSrc = i * (TXTCOLS*8);
		int offDst = ((i+CENTERY)*640) + CENTERX + ((i>>4)*16) - (8*16);

		for (int j = 0; j < (TXTCOLS*8); j++)
		{
			if (FramT[offSrc+j]) Frame[offDst+j] = FramT[offSrc+j];
		}
	}
			
			


/*
	for (int y = 0; y < 36; y++) 
	{
		fastSet(Frame+(640*y),6,640);
		fastSet(Frame+(640*444)+(640*y),6,640);
	}
	for (y = 0; y < 36; y++) 
	{
		fastSet(Frame+(640*y),6,640);
		fastSet(Frame+(640*444)+(640*y),6,640);
	}
	for (y = 0; y < 408; y++) 
	{
		fastSet(Frame+(640*36)+(640*y),6,48);
		fastSet(Frame+(640*36)+(640-48)+(640*y),6,48);
	}


	for (y = 36; y < 38; y++) 
	{
		fastSet(Frame+48+(640*y),5,640-96);
		fastSet(Frame+48+(640*406)+(640*y),5,640-96);
	}
	for (y = 36; y < 444; y++) 
	{
		int off = 640*y;
		Frame[off+48] = 5;	Frame[off+49] = 5;
		Frame[off+590] = 5;	Frame[off+591] = 5;
	}
*/


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
void	WinMainCRTEnd(void)
/*--------------------------------------------------------------*/
{
//		FCp_Close();
		chpClose();
		tdxClose();
		ShowCursor(TRUE);
//		CoUninitialize();
		ExitProcess(0);
}



//int WINAPI WinMain( HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int nCmdShow )
//{	ghInstance = hInstance;

int WINAPI WinMainCRTStartup(void)
{	ghInstance = GetModuleHandle(0);

	if (!degif(back, Background, Palette )) return -1;
	if (!degif(font, Fontground, Palette2)) return -1;

	timer.Init();
	Math3DInit();
	vfont_init();

	scroll_Info.scrFont = (char*)Fontground;
	scroll_Info.scrFontRow = 664;
	scroll_Info.scrDest = (char*)FramT;
	scroll_Info.scrDestRow = TXTCOLS*8;

	scroll_Info.scrPages = pages;
	scroll_Info.scrFontMtx = fntMtx;
	scroll_Info.scrFontW = 8;
	scroll_Info.scrFontH = 16;
	scroll_Info.col = TXTCOLS;

	scrInit(&scroll_Info);

//----------




WNDCLASS	wc;
MSG			msg;
HWND		hWnd;


    wc.lpszClassName = szWindowClass;
    wc.lpfnWndProc = MainWndProc;
    wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.hInstance = ghInstance;
    wc.hIcon = 0; //LoadIcon( NULL, IDI_APPLICATION );
    wc.hCursor = 0; //LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = 0; //(HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.lpszMenuName = NULL; //"WINtroAppMenu";
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;

    RegisterClass( &wc );

/*
	hWnd = CreateWindowEx(WS_EX_TOPMOST,
	  "WINtroAppClass",
      "WINtro",
	  WS_POPUP, 
      0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),
      NULL,NULL,ghInstance,NULL
	);
*/
	hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP | WS_VISIBLE, 0, 0, 640, 480, NULL, NULL, ghInstance, NULL);

	ghMainWnd = hWnd;

	ShowCursor(FALSE);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

//	CoInitialize(0);
	tdxOpen((DWORD)hWnd);

//	FCp_Open((DWORD)hWnd, 120);	// Mastervol: 128 = 100%
//	FCp_InitModule((char*)Module);
	chpOpen(Module, (DWORD)hWnd);

//	while (!scrExecute(&scroll_Info)) ;

/*
UBYTE	*myPal = imx+10;

int		time = timeGetTime();
int		*pCols = cols + (time & 7) * 3;

UBYTE	r = pCols[0];
UBYTE	g = pCols[1];
UBYTE	b = pCols[2];

	setPal(myPal,0x01, r,g,b, 2);
	setPal(myPal,0x02, 60,60,60, 0);
	setPal(myPal,0x03, r,g,b, 2);
	setPal(myPal,0x04, r,g,b, 4);

	setPal(myPal,0x01+8, r,g,b, -1);
	setPal(myPal,0x02+8, 50,50,50, 0);
	setPal(myPal,0x03+8, r,g,b, 2);
	setPal(myPal,0x04+8, r,g,b, 6);

	setPal(myPal,0x05, 0,0,0, 0);
	setPal(myPal,0x06, r,g,b, -4);
*/


	for (int i = 0; i < 64*3; i++) 
	{
		float col = Palette[i] * 0.75f;
		Palette[i+64*3] = (int)col;
	}

//	for (i = 0; i < 128*3; i++) Palette[128*3+i] = (UBYTE)imx[10+i]<<2;

	for (i = 0; i < 16*3; i++) Palette[i+(128*3)] = Palette2[i];
	Palette[0+(144*3)] = Palette[1+(144*3)] = Palette[2+(144*3)] = 224;

	for (i = 0; i < 664*16; i++) Fontground[i] += 128;

	tdxSetPal8(Palette);

	Sleep(300);

//	FCp_Start();
	chpStart();
//----------


//	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_ABOVE_NORMAL);

    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, 0))
        {
            GetMessage(&msg, NULL, 0, 0);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else if (g_bActive) UpdateFrame();
             else           WaitMessage();
    }


	return 0;
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


		case WM_PAINT:
			ValidateRect(hWnd, 0);
			return TRUE;


		case WM_KEYDOWN:
            switch (wParam)
            {
                case VK_ESCAPE:
					g_bActive = 0;
					WinMainCRTEnd();
                    PostMessage(hWnd, WM_CLOSE, 0, 0);
                    return 0L;
            }
            break;

      default:
		return( DefWindowProc( hWnd, msg, wParam, lParam ));
	}
	return 0;
}

