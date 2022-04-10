#include <windows.h>
#include <math.h>
#include <commctrl.h>
#include <commdlg.h>


#define	WND_TITLE		"Defjam Amiga Intro - by SiRioKD 2oo1"
#define	WND_CLASSNAME	TEXT("SoundWindow")

#define	  RGB16(r,g,b) ((SWORD)((r<<12)|(g<<7)|(b<<1)))	// USES 4bit for R for G and B (0~15)

#define	WSIZE_W	((336*1)+6)
#define	WSIZE_H	((240*1)+6)

#include "cGif.h"
#include "fc14p.h"

#include "m_astaroth.h"
//#include "m_horizon.h"
//#include "m_classic.h"

#include "dfj_font.h"


BOOL	 g_bActive = FALSE;
HWND	 g_hwndMain;

HDC		pDC;               // Device context for framebuffer
HBITMAP old;               // Stored HBITMAP of framebuffer context
HBITMAP ourbitmap;         // Framebuffer bitmap
SWORD*	framebuf;          // Pointer to the framebuffer


SBYTE*	Modules[] = { (SBYTE*)astaroth };

cGifImage	gif;
	

SWORD	font16[256*32];
SWORD	backbuf[336*200];
int		wrptr = 24;
int		rdptr = 0;
int		scrcnt = 0;

char	    Matrix[] = 
" #ABCDEFGHIJKLMNOPQRSTUVWXYZ0123"
"456789:+-!";


void	PutCharXY(SWORD *d, int pitch, int x, int y, char a);
void	TextXYn(SWORD *d, int pitch, int x, int y, char *s, int n);

int		TextPtr = 0;
char	Text[] = 
"^"
"####   ##### ##### #####  ####  ##    ##"
"##  ## ##    ##       ## ##  ## ###  ###"
"##  ## ##    ##       ## ##  ## ########"
"##  ## ##### ####     ## ###### ## ## ##"
"##  ## ##    ##       ## ##  ## ##    ##"
"##  ## ##    ##    ## ## ##  ## ##    ##"
"####   ##### ##     ###  ##  ## ##    ##"
"^^^^^^^"
"                PRESENTS:               "
"^^^"
"      CONFLICT MIDDLE EAST FROM SSI     "
"^^^"
"            100  SALES VERSION          "
"^^^^^^^"
"            CRACKED BY MOGWAI           "
"^^^^"
"      ORIGINAL SUPPLIED BY TWISTER      "
"^^^^^^^^^^^^^"
"  CALL STREETS OF FIRE: +46-755-10498   "
"^^^^^^^"
"  SPECIAL GREETINGS TO ALL OUR FRIENDS  "
"         AND CONTACTS WORLDWIDE!        "
"^^^^^^^^^^^^^^^^^^^^^^|";

/*--------------------------------------------------------------*/
void	HLine(int line, SWORD color)
/*--------------------------------------------------------------*/
{
SWORD	*dst1 = framebuf + (line*336);

	for (int i = 0; i < 336; i++) *dst1++ = color;
}

/*--------------------------------------------------------------*/
void	ShiftLL(int ypos)
/*--------------------------------------------------------------*/
{

	if (scrcnt == 0)
	{
		scrcnt = 8;
		if (Text[TextPtr] == '|') TextPtr = 0;

		if (Text[TextPtr] == '^')
		{
			TextXYn(backbuf, 336, 8, wrptr*8, "                                        ", 40);
			TextPtr++;
		}
		else
		{
			TextXYn(backbuf, 336, 8, wrptr*8, Text+TextPtr, 40);
			TextPtr += 40;
		}
		wrptr++; if (wrptr >= 25) wrptr = 0;
	}

	scrcnt--;


SWORD	*s, *d;
int		off = rdptr;

	s = backbuf;
	d = framebuf+(336*12);
	
	for (int i = 0; i < 192; i++)
	{
		for (int x = 0; x < 336; x++) d[x] = s[x+off];
		off += 336; off %= 336*200;
		d += 336;
	}

	rdptr += 336;
	rdptr %= 336*200;

	HLine(12, RGB16(15,15,15));
	HLine(204, RGB16(15,15,15));
}






/*--------------------------------------------------------------*/
void	ClearScreen(SWORD color)
/*--------------------------------------------------------------*/
{
	for (int i = 0; i < 336*240; i++) framebuf[i] = color;
}



/*--------------------------------------------------------------*/
void	CopyQuad(SWORD *d, int pitch, SWORD *s)
/*--------------------------------------------------------------*/
{

	for (int y = 0; y < 8; y++) 
	{
/*
		for (int x = 0; x < 8; x++) 
		{

SWORD	v = s[(y<<8)+x];
			if (v) d[(y*pitch)+x] = v;

			d[(y*pitch)+x] = s[(y<<8)+x];
		}
*/
		memcpy(d+(y*pitch),s+(y<<8),16);
	}
}


/*-----------------------------------------------------------------*/
int		GetMatrixPos(char c)
/*-----------------------------------------------------------------*/
{
int		i,pos = -1;
char	*p = Matrix;

	for (i = 0; p[i] != 0; i++)
	{
		if (p[i] == c) 
		{
			pos = i;
			break;
		}
	}

	int rig = (pos / 32);
	int col = (pos % 32);
	pos = (rig*256*8)+(col*8);

	return pos;
}


/*--------------------------------------------------------------*/
void	PutCharXY(SWORD *d, int pitch, int x, int y, char a)
/*--------------------------------------------------------------*/
{
	SWORD *s = font16 + GetMatrixPos(a);

	CopyQuad(d+(y*pitch)+x, pitch, s);

}


/*-----------------------------------------------------------------*/
void	TextXYn(SWORD *d, int pitch, int x, int y, char *s, int n)
/*-----------------------------------------------------------------*/
{
	while (n-- && *s) 
	{
		PutCharXY(d, pitch, x, y, *s);
		x += 8;
		s++;
	}
}



/*--------------------------------------------------------------*/
void	RunOnce(void)
/*--------------------------------------------------------------*/
{
	if (framebuf == 0) return;

	ClearScreen(0);

	ShiftLL(210);
	ShiftLL(210);
}



/*--------------------------------------------------------------*/
void	UpdateFrame(HWND hWnd)
/*--------------------------------------------------------------*/
{
HDC	hDC = GetDC(hWnd);

	BitBlt(hDC, 0, 0, 336, 240, pDC, 0, 0, SRCCOPY);

//	SetStretchBltMode(hDC,STRETCH_DELETESCANS);
//	StretchBlt(hDC,0,0,336*2,240*2,pDC,0,0,336,240,SRCCOPY);

	DeleteDC(hDC);

}





/*--------------------------------------------------------------*/
void init_framebuf(void)
/*--------------------------------------------------------------*/
{
  HDC hDC;
  BITMAPINFO *bitmapinfo;
  bitmapinfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO)+2*sizeof(int));
  hDC = CreateCompatibleDC(NULL);
  memset(bitmapinfo, 0, sizeof(BITMAPINFO)+2*sizeof(int));
  // These are bit masks for the R, G and B components:
  bitmapinfo->bmiColors[0].rgbGreen = 0xf8; 
  bitmapinfo->bmiColors[1].rgbGreen = 0x07;
  bitmapinfo->bmiColors[1].rgbBlue = 0xe0;
  bitmapinfo->bmiColors[2].rgbBlue = 0x1f;

  bitmapinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bitmapinfo->bmiHeader.biWidth = 336;
  bitmapinfo->bmiHeader.biHeight = -240; /* top-down */
  bitmapinfo->bmiHeader.biPlanes = 1;
  bitmapinfo->bmiHeader.biBitCount = 16; 
  bitmapinfo->bmiHeader.biCompression = BI_BITFIELDS; // BI_RGB doesn't work in ce.. 
  bitmapinfo->bmiHeader.biSizeImage = 336*240*2;
  bitmapinfo->bmiHeader.biClrUsed = 0;
  bitmapinfo->bmiHeader.biClrImportant = 0;
  ourbitmap = CreateDIBSection(hDC, bitmapinfo, DIB_RGB_COLORS, (void**)&framebuf, 0, 0);
  free(bitmapinfo);
  pDC = CreateCompatibleDC(NULL);
  old = (HBITMAP)SelectObject(pDC, ourbitmap);
  DeleteDC(hDC);
}

/*--------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
/*--------------------------------------------------------------------------------*/
{

	switch (message) 
	{

	    case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;



		case WM_ACTIVATE:
            g_bActive = (LOWORD(wParam) == WA_ACTIVE) || 
						(LOWORD(wParam) == WA_CLICKACTIVE);

			break;


		case WM_DESTROY:
			PostQuitMessage(0);
			break;


		case WM_CREATE:
		  break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}





/*--------------------------------------------------------------------------------*/
HWND	windowCreate(HINSTANCE hInstance)
/*--------------------------------------------------------------------------------*/
{
WNDCLASS wcex;


		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= (WNDPROC)WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;
		wcex.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= WND_CLASSNAME;
		RegisterClass(&wcex);

		HWND hWnd = CreateWindow(	WND_CLASSNAME,TEXT(WND_TITLE),
									WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 
				(GetSystemMetrics(SM_CXSCREEN)-WSIZE_W)/2, (GetSystemMetrics(SM_CYSCREEN)-WSIZE_H) / 2,
									WSIZE_W, WSIZE_H,
									NULL, NULL, hInstance, NULL);

		if (!hWnd) return NULL;

		/*
		RECT rc;
		GetWindowRect(hWnd, &rc);
		rc.top -= 26;
		MoveWindow(hWnd, rc.left, rc.top, rc.right, rc.bottom, FALSE);
*/
		ShowWindow(hWnd, SW_SHOWNORMAL);
		UpdateWindow(hWnd);

		g_hwndMain = hWnd;

		return hWnd;

}


/*--------------------------------------------------------------*/
void	StartAll(void)
/*--------------------------------------------------------------*/
{
	init_framebuf();
	UpdateFrame(g_hwndMain);

	
	gif.LoadGifM((UBYTE*)font,(UBYTE*)font16);


	FCp_Open((DWORD)g_hwndMain);
	FCp_InitModule((char*)Modules[0]);
	FCp_Start();


	Sleep(150);
}


/*--------------------------------------------------------------*/
void	CloseAll(void)
/*--------------------------------------------------------------*/
{
	FCp_Close();
}



/*--------------------------------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hP, LPSTR cl, int nCmdShow)
/*--------------------------------------------------------------------------------*/
{
DWORD	dwUpdateDelay = 20;	// milliseconds

		MSG	msg;
		HWND hWnd = windowCreate(hInstance);
		if (hWnd)
		{
//			SHFullScreen(hWnd, SHFS_HIDETASKBAR|SHFS_HIDESIPBUTTON|SHFS_HIDESTARTICON);

			StartAll();

			while (TRUE)
			{
		        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
				{
					if (!GetMessage(&msg, NULL, 0, 0)) return msg.wParam;
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else if (g_bActive)
				{
					static DWORD	lastTickCount = 0;
					static int		currentFrame = 0;
					DWORD			thisTickCount;

//				    thisTickCount = GetTickCount();
				    thisTickCount = timeGetTime();
					
					if ((thisTickCount - lastTickCount) > dwUpdateDelay)
					{
					    // Move to next frame;
						lastTickCount = thisTickCount;
						if (++currentFrame > 50) currentFrame = 0;

						RunOnce();
						UpdateFrame(hWnd);
					}
				}
				else
				{
					WaitMessage();
				}
			}



			CloseAll();
		}

		return 0;
}



