#include <stdlib.h>
#include <stdio.h>

#include <windows.h>
#include "tdx.h"
#include "Muzak.h"
#include "fc14p.h"
#include "Intro.h"
#include "Gfx.h"


UBYTE	Frame[336*256];

LRESULT CALLBACK MainWndProc( HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam );


#include "Stars.h"
#include "Scroll.h"



HINSTANCE	ghInstance;
HWND		ghMainWnd;
int			g_bActive;

char	fntMTX[] = 
"ABCDEFGHIJKLMNOP"
"QRSTUVWXYZ012345"
"6789:*?!()+-,./#"
"[] ø";


struct	scrInfo scroll_Info;


#include "Kern.h"







/*----------------------------------------------------------*/
void	UpdateFrame(void)
/*----------------------------------------------------------*/
{
	fastSet(Frame,0,336*240);

	copyTile(Frame,336);
	
	scroller((char*)Frame, 336);

	star_draw((char*)Frame, (320+16));


	static int ofstabY = 128;
	ofstabY = (ofstabY + 9) & 511;

	UBYTE *src = imx + 778 + (256*48);
	UBYTE *dst = Frame+(336*230)+280+28 - (336*((sinetab[ofstabY]/10)-2));

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 31; j++)
		{
			UBYTE v = src[(i<<8)+j];
			if (v!=0) dst[ (i*336) + j] = v;
		}
	}



	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);
	tdxUpdate(Frame+16,336);
	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_ABOVE_NORMAL);


/*
	if (GetAsyncKeyState(VK_F4) & 0x8000)
	{

		FILE *fp = fopen("c:\\1shot.scx","wb");

		fputc(0x52,fp);
		fputc(0x49,fp);
		fputc(0x58,fp);
		fputc(0x33,fp);
		fputc(0x40,fp);
		fputc(0x01,fp);
		fputc(0xF0,fp);
		fputc(0x00,fp);
		fputc(0xAF,fp);
		fputc(0x00,fp);

		fwrite(imx+10,1,768,fp);

		for (int i = 0; i < 240; i++)
		{
			fwrite(Frame+16+(i*336),1,320,fp);
		}


		fclose(fp);
	}
*/
}



//int WINAPI WinMain( HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int nCmdShow )
//{	ghInstance = hInstance;

int WINAPI WinMainCRTStartup(void)
{	ghInstance = GetModuleHandle(0);

WNDCLASS	wc;
MSG			msg;
HWND		hWnd;

      wc.lpszClassName = "WINtroAppClass";
      wc.lpfnWndProc = MainWndProc;
      wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
      wc.hInstance = ghInstance;
      wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
      wc.hCursor = LoadCursor( NULL, IDC_ARROW );
      wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
      wc.lpszMenuName = "WINtroAppMenu";
      wc.cbClsExtra = 0;
      wc.cbWndExtra = 0;

      RegisterClass( &wc );


	hWnd = CreateWindowEx(WS_EX_TOPMOST,
	  "WINtroAppClass",
      "WINtro",
	  WS_POPUP, 
      0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),
      NULL,NULL,ghInstance,NULL
	);


	ghMainWnd = hWnd;

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	ShowCursor(FALSE);

	tdxOpen((DWORD)hWnd);
	tdxSetPal(imx+10);

//----------


	fastCopy((UBYTE*)FNTBUF,imx+10+768,256*(4*16));

	scroll_Info.scrFont = FNTBUF;
	scroll_Info.scrFontRow = 256;
	scroll_Info.scrDest = SCRBUF;
	scroll_Info.scrDestRow = RESX+16;

	scroll_Info.scrText = text;
	scroll_Info.scrFontMtx = fntMTX;
	scroll_Info.scrFontW = 16;
	scroll_Info.scrFontH = 16;

	scrInit(&scroll_Info);


	star_init();

//----------
	FCp_Open((DWORD)hWnd);
	FCp_InitModule((char*)Muzak);
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
					ShowCursor(TRUE);
                    PostMessage(hWnd, WM_CLOSE, 0, 0);
					ExitProcess(0);
                    return 0L;
            }
            break;

      default:
		return( DefWindowProc( hWnd, msg, wParam, lParam ));
	}
	return 0;
}



