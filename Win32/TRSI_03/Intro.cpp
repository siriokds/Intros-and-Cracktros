#include <stdlib.h>
#include <stdio.h>
extern "C" const int _fltused = 0;
float		scaleval;

#include <windows.h>
#include <objbase.h>
#include "rtlib2.h"
//#include <math.h>
#include "timing.h"
#include "tristar.h"
#include "fc14p.h"
#include "tdx.h"
#include "Intro.h"
#include "Gfx.h"
#include "scroll.h"



#define	 FONTWIDTH	8
#define	 FONTHEIGHT	8
#define	 TXTCOLS	40
#define	 TXTROWS	10
#define	 CENTERX	0
#define	 CENTERY	82


UBYTE	Frame[320*240];
UBYTE	FramT[(TXTCOLS*FONTWIDTH)*(TXTROWS*(FONTHEIGHT+2))];

LRESULT CALLBACK MainWndProc( HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam );



char* szTitle = "TRSI";
char* szWindowClass = "TRSI";
HINSTANCE	ghInstance;
HWND		ghMainWnd;
int			g_bActive;

ticker_t	timer;









char	fntMtx[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890()+,!'@/><:.-";

struct	scrInfo		scroll_Info;

char	page1[] = 
// 0         1         2         3         4
// 01234567890123456789012345678901234567890
// -------------------------||--------------
  "                                        "
  "  TRISTAR AND RED SECTOR INC. PRESENTS  "
  "  ------------------------------------  "
  "                                        "
  "          > XMEN-2 +4 TRAINER <         "
  "            -----------------           "
  "                                        "
  "      TRAINED BY: SETH GECKO / TRSI     "
  "      CRACKED BY: FAIRLIGHT             "
  "                                       q";



char	page2[] = 
// 0         1         2         3         4
// 01234567890123456789012345678901234567890
// -------------------------||--------------
  "                                        "
  "     OUR GOLDEN REGARDS GO OUT TO:      "
  "                                        "
  "    FAIRLIGHT - QUARTEX - SECTION 8     "
  "   DANISH GOLD - KALISTO - RAZOR 1911   "
  "    IMMERSION - LOCKLESS - DEVIANCE     "
  "      MYTH - SCOOPEX - THE SILENTS      "
  "    GENESIS - SOUNDDRINKER - PARADOX    "
  "          HAUJOBB - FARBRAUSCH          "
  "                                       w";


char	page3[] = 
// 0         1         2         3         4
// 01234567890123456789012345678901234567890
// -------------------------||--------------
  "                                        "
  "             INTRO CREDITS:             "
  "                                        "
  " INTRO CODING ............ SIRIOKD/TRSI "
  " 2D FONT .................... ADEC/TRSI "
  " LOGO ..................... J.O.E./TRSI "
  " FC REPLAYER ............. SIRIOKD/TRSI "
  " MUZAK ........................... TRSI "
  "                                        "
  "                                       w";






char	*pages[] = { page1, page2, page3, 0 };





/*----------------------------------------------------------*/
void	DrawSirioKD(void)
/*----------------------------------------------------------*/
{
	UBYTE *src = skd+16;

	UBYTE *dst = Frame+(320*177)+ 320-35;


	for (int i = 0; i < 8; i++)
	{
		int offY = (i*320);

		for (int j = 0; j < 32; j++)
		{
			UBYTE v = src[(i<<5)+j];
			if (v!=0) dst[offY + j] = v;
		}
	}

}



#define STAR_NUM 128

int		star_pos[STAR_NUM];
int		star_spd[STAR_NUM];
UBYTE	rast[320];
int		rastI[320*3];
int		*rastIdx = rastI+320;



#define	SCRPITCH 392
UBYTE	scroll_buf[SCRPITCH*10];
int		scroll_shf = 0;
int		scroll_txtpos = 0;
char	scroll_txt[] = " TRISTAR AND RED SECTOR INC. "
				  "PROUDLY PRESENTS...  X-MEN2 +4 TRAINER.... "
				  "GAME CRACKED BY: - FAIRLIGHT -...   AND AS USUAL JUST RELEASED AND TRAINED BY SETH GECKO / TRSI...  "
				  "IF U WANNA TO CONTACT US... GO TO: WWW.TRSI.ORG      "
				  "  INTRO CODING BY SIRIOKD/TRSI...   "
				  "                                                     ";


/*----------------------------------------------------------*/
void	StarsInit(void)
/*----------------------------------------------------------*/
{
static	ULONG m_RND = 0x3B9AF812;

	for (int i = 0; i < STAR_NUM; i++)
	{
		m_RND = ((m_RND << 7) + (m_RND >> 25) + 0x1ECA256D) ^ 0x9E59A92B;
		star_pos[i] = (m_RND & 511) % 320;
		star_spd[i] = ((m_RND >> 24) & 3)+1;
	}

	for (i = 0; i < 160; i++) { rast[i] = 64 + ((i/5)&31); rast[i+160] = 64 + ((31-(i/5))&31); }
	for (i = 0; i < 320*3; i++) rastI[i] = i%320;

	for (i = 0; i < SCRPITCH*10; i++) scroll_buf[i] = 32;

}

/*----------------------------------------------------------*/
void	StarsDraw(void)
/*----------------------------------------------------------*/
{
UBYTE *dst;
int i;

int	cols[] = { 18, 20, 22, 24, 26 };


	dst = Frame;// + (320*82);

	for (i = 0; i < STAR_NUM; i++)
	{
		int pos = star_pos[i] + ((i<80)?(i*320):((i+107)*320));

		if (dst[pos] == 32) dst[pos] = cols[ star_spd[i] ];
		star_pos[i] += star_spd[i];
		if (star_pos[i] >= 320) star_pos[i] -= 320;
	}
}




/*----------------------------------------------------------*/
void	ScrollDraw(void)
/*----------------------------------------------------------*/
{
#define	M_PI 3.14159265358979323846264338328f

UBYTE *dst;
int i;
int	shf[16];

	dst = Frame+(320*200);

static float vY = 0.0f;
static float vX = 0.0f;


	int bounce = (int(sin(vX)*10)+4)*320;


	for (i = 0; i < 16; i++)
	{
		shf[i] = int(sin(vY)*32)+32;
		vY -= (M_PI / 700); 
		if (vY > M_PI) vY -= M_PI;
	}

	int l = 0;
	for (i = 0; i < 8; i++) 
	{ 
		fastCpy(dst+(640*i)+bounce,    scroll_buf+(SCRPITCH*i)+shf[l++], 320); 
		fastCpy(dst+(640*i)+320+bounce,scroll_buf+(SCRPITCH*i)+shf[l++], 320); 
	}

	vX -= (M_PI / 70); if (vX > M_PI) vX -= M_PI;



	for (int j = 0; j < 10; j++) { int off = j * SCRPITCH; for (i = 0; i < SCRPITCH-1; i++) scroll_buf[off+i] = scroll_buf[off+i+1]; }

	if (++scroll_shf > 8)
	{
		scroll_shf = 0;
		scrWriteChar(&scroll_Info, scroll_txt[scroll_txtpos], (char*)scroll_buf+SCRPITCH-8, SCRPITCH, 1);
		scroll_txtpos++; if (scroll_txt[scroll_txtpos] == 0) scroll_txtpos = 0;
	}




}



/*----------------------------------------------------------*/
void	DrawLogo(void)
/*----------------------------------------------------------*/
{
static int redline = 0;
static int movlogo = 1;

	UBYTE *src = logo + 778;
	UBYTE *dst = Frame + ((80-movlogo)*320);

	for (int i = 0; i < 320*movlogo; i++) *dst++ = *src++;

	if (movlogo < 80) movlogo++;

	dst += 320;		for (i = 0; i < 320; i++) *dst++ = rast[ rastIdx[i+redline] ];
	dst += 320*100;	for (i = 0; i < 320; i++) *dst++ = rast[ rastIdx[i-redline] ];

	redline += 8; if (redline > 320) redline -= 320;

}





/*----------------------------------------------------------*/
void	UpdateFrame(void)
/*----------------------------------------------------------*/
{
static	ULONG m_RND = 0x7BF89A12;

	scaleval = timer.Passed() * 0.1f;
	if (scaleval < 0) scaleval = 1.0f;

	fastSet(Frame,32,320*240);



	if (scroll_Info.Wait == 0)
	{
		for (int i = 0; i < (TXTROWS*(FONTHEIGHT+2)); i++)
		{
			int offDst = ((i+CENTERY)*320) + CENTERX;

			for (int j = 0; j < (TXTCOLS*FONTWIDTH); j+=4)
			{
					m_RND = ((m_RND << 7) + (m_RND >> 25) + 0x6ECA756D) ^ 0x9E59A92B;
					Frame[offDst+j+0] = (UBYTE)(((m_RND    ) & 15)+16);
					Frame[offDst+j+1] = (UBYTE)(((m_RND>> 8) & 15)+16);
					Frame[offDst+j+2] = (UBYTE)(((m_RND>>16) & 15)+16);
					Frame[offDst+j+3] = (UBYTE)(((m_RND>>24) & 15)+16);
			}
		}
	}
	else
	{
		fastCpy(Frame+(CENTERY*320),FramT, (TXTROWS*(FONTHEIGHT+2)) * (TXTCOLS*FONTWIDTH));
/*
		for (int i = 0; i < (TXTROWS*(FONTHEIGHT+2)); i++)
		{
			int offSrc = i * (TXTCOLS*FONTWIDTH);
			int offDst = ((i+CENTERY)*320) + CENTERX;

			for (int j = 0; j < (TXTCOLS*FONTWIDTH); j++)
			{
				Frame[offDst+j] = (FramT[offSrc+j]) ? FramT[offSrc+j] : 128;
			}
		}
*/

	}
			
			

	DrawLogo();
	ScrollDraw();
	StarsDraw();

	scrExecute(&scroll_Info);
	DrawSirioKD();


	UBYTE *dst = Frame + (320*226);
	for (int i = 0; i < 14; i++)
	{
		fastCpy(dst+(i*320),dst-((i+1)*640),320);
	}


	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);
	tdxUpdate(Frame,320);
	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_ABOVE_NORMAL);
}



/*
void	setPal(UBYTE *pal, int idx, int r, int g, int b, int delt)
{
	pal += (idx*3);


	r += delt; if (r < 0) r = 0; if (r > 63) r = 63;
	g += delt; if (g < 0) g = 0; if (g > 63) g = 63;
	b += delt; if (b < 0) b = 0; if (b > 63) b = 63;

	pal[0] = r; pal[1] = g; pal[2] = b;
}
*/


/*--------------------------------------------------------------*/
void	WinMainCRTEnd(void)
/*--------------------------------------------------------------*/
{
		FCp_Close();
		tdxClose();
		ShowCursor(TRUE);
//		CoUninitialize();
		ExitProcess(0);
}



//int WINAPI WinMain( HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int nCmdShow )
//{	ghInstance = hInstance;

int WINAPI WinMainCRTStartup(void)
{	ghInstance = GetModuleHandle(0);

	timer.Init();

	scroll_Info.scrFont = (char*)imx+778;
	scroll_Info.scrFontRow = 416;
	scroll_Info.scrDest = (char*)FramT;
	scroll_Info.scrDestRow = TXTCOLS*FONTWIDTH;

	scroll_Info.scrPages = pages;
	scroll_Info.scrFontMtx = fntMtx;
	scroll_Info.scrFontW = FONTWIDTH;
	scroll_Info.scrFontH = FONTHEIGHT;
	scroll_Info.col = TXTCOLS;

	scrInit(&scroll_Info);
	scroll_Info.Wait = 40;
	scroll_Info.scrPage = 3;

	StarsInit();


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

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	
	if (tdxOpen((DWORD)hWnd) != 0) ExitProcess(0);

	ShowCursor(FALSE);

	FCp_Open((DWORD)hWnd, 118);
	FCp_InitModule(Module);


UBYTE	myPal[768];

	for (int i = 0; i < 16*3; i++) myPal[i] =  imx[10+i];

	for (i = 0; i < 16; i++)
	{
		myPal[48+(i*3)+0] = i*4;
		myPal[48+(i*3)+1] = i*4;
		myPal[48+(i*3)+2] = i*4;
	}

	for (i = 0; i < 32; i++)
	{
		myPal[192+(i*3)+0] = i*2;
		myPal[192+(i*3)+1] = (i*5)>>2;
		myPal[192+(i*3)+2] = 0;
	}


	for (i = 0; i < 32*3;i++) myPal[(32*3)+i] = logo[10+i];
	for (i = 0; i < 320*80; i++) logo[778+i] += 32;

	skd[3] = 4;
	skd[4] = 4;
	skd[5] = 8;
	for (i = 0; i < 4*3; i++) myPal[(96*3)+i] = skd[i];
	for (i = 0; i < 32*8; i++) if (skd[16+i]) skd[16+i] += 96;


	myPal[(128*3)+0] = 0;
	myPal[(128*3)+1] = 8;
	myPal[(128*3)+2] = 18;
	myPal[0] = 0;
	myPal[1] = 8;
	myPal[2] = 18;


	tdxSetPal(myPal);


	Sleep(300);

	FCp_Start();

	Sleep(100);
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
            g_bActive = (wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE);
			return 0L;

		case WM_DESTROY:
			PostQuitMessage(0);
		break;


		case WM_PAINT:
			ValidateRect(hWnd, 0);
			return TRUE;


		case WM_LBUTTONDOWN:
			g_bActive = 0;
			WinMainCRTEnd();
            PostMessage(hWnd, WM_CLOSE, 0, 0);
            return 0L;

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

