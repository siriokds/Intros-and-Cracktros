/*** Headers ****************************************************/

#include <windows.h>
#include "Resource.h"
#include "MOD.cpp"
#include "FastBMP.h"
#include "Texter.h"
#include "chplay.h"

#define	WINDOW_W	336
#define	WINDOW_H	208

#define LOGO_INIPOS	((100-64)+8)

#define	WINDOWB_W	((WINDOW_W*2)+6)
#define	WINDOWB_H	((WINDOW_H*2)+6)

#define	WINDOW_T	"L o c k l e s s   -   We fly high!"



char	Page1[] = 
	" LOCKLESS IS BACK TO HIT YOUR FACE WITH]"
	"      ANOTHER HOT MAJOR RELEASE...]]"
	"             THIS TIME IT IS]"
	"             ---------------]]"
	"          * SAMBA PARTIE  100% *        "
	"]]]"
	"SUPPLIED BY ................ LORD ANSHAR]]"
	"|";


char	Page2[] = 
	"          YOU CAN REACH US AT:]]"
	"            WWW.LOCKLESS.COM]"
	"            WWW.LOCKLESS.ORG]"
	"            WWW.LOCKLESS.NET]]"
	"        OR IN *LOCKLESS ON EFNET]]]"
	"      * LOCKLESS - WE FLY HIGH! *]"
	"|";

char	Page3[] = 
	"             LOCKLESS ARE...]]"	
	"   LORD ANSHAR - YADO - KROMA - NOIZE]"
	"  PHOTOPAUL - KARNAK - X-LOCK - NICKDH]"
	"      BISHOP - ECHELON - PINCOPALL ]"
	"     PARABYTES - MMASTER - SIRIOKD]"
	"    LITTLELUK - FREMEN - KH - SMURF1]"
	"         CAPATOSTA - ZEROSYNAPSE]]"
	"            GREETS FLY OUT TO...]]"
	"   CORE - DF - EMINENCE - EVC - DAMN]"
	"         TNT - TSRH - TCA - DBC]"
	"|";


char	Page4[] = 
	"            + INTRO CREDITS +]]"	
	" CODE .............. SIRIOKD / LOCKLESS]"
	" GFX ........... ZEROSYNAPSE / LOCKLESS]"
	" MUSIC .................. SMT / ARTWORK]]]"
	"   OKAY, THANX TO ALL OF YOU DUDIES..]]]"
	"        <C>OPYLEFT 2K3, LOCKLESS!"
	"|";
/*

char	Page4[] = 
  "THIS IS PAGE 4]"
  "PAGE4]"
  "|";
*/

char*	Pages[] = { Page1, Page2, Page3, Page4, NULL };

/*** Prototypes *************************************************/

LRESULT WINAPI MainWndProc( HWND, UINT, WPARAM, LPARAM );

void	StartAll(HWND);
void	CloseAll(void);

/*** Globals ****************************************************/
HANDLE		ghInstance;
HWND		ghMainWnd;
BOOL		g_bActive = FALSE;   // Is application active?
BOOL		ShowedFirst;
static		DWORD dwUpdateDelay = 10;

int			wposX, wposY;

/****************************************************************/
TFastBMP	*BackBuffer, *FrameBuffer, *FontImage, *LogoImage;
TTexter		*Texter1;
TMtx		*InfoMtx1;

char		Matrix[] = 
" ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789><*/\\,.'&-:+@!%";

/****************************************************************/


/*--------------------------------------------------------------*/
void	UpdateFrame(HWND hWnd)
/*--------------------------------------------------------------*/
{
HDC	hDC;
static DWORD	lastTickCount = 0;
static int		currentFrame = 0;
static BOOL		haveBackground = FALSE;
DWORD			thisTickCount;
int				x,y,cx,cy;

static int		pos = LOGO_INIPOS + 50;
int				ypos;



    thisTickCount = GetTickCount();
    if ((thisTickCount - lastTickCount) <= dwUpdateDelay)
        return;

    // Move to next frame;
    lastTickCount = thisTickCount;
    currentFrame++; if (currentFrame > 50) currentFrame = 0;


	if (pos>0)
	{
		pos -= 2;
		if (!pos) chpStart();
		if (pos <= LOGO_INIPOS)	ypos = 8 + pos;
		else					ypos = 8 + LOGO_INIPOS;

		LogoImage->DrawRect(FrameBuffer,8,ypos, 0,0, 320,64);
		InfoMtx1->SetRect(1,ypos/8, 320/8, (64/8)+1, 2);

	}
	else
	{
		if (Texter1) Texter1->RunOnce();
	}


	if (InfoMtx1->Valid() == TRUE)
	{
		cx = InfoMtx1->GetSizeX();
		cy = InfoMtx1->GetSizeY();

		for (y = 0; y < cy; y++)
		  for (x = 0; x < cx; x++)
			if (InfoMtx1->Get(x,y) == 2)
				FrameBuffer->DrawDoubleTile(BackBuffer,x,y);



		hDC = GetDC(ghMainWnd);
		
		for (y = 0; y < cy; y++)
		  for (x = 0; x < cx; x++)
			if (InfoMtx1->Get(x,y) == 2)
			{
				BackBuffer->DrawTile(hDC,x,y);
				InfoMtx1->Set(x,y,0);
			}

		DeleteDC(hDC);

		

	}
}

/*
void	_UpdateFrame(HWND hWnd)
{
HDC	hDC;
static DWORD	lastTickCount = 0;
static int		currentFrame = 0;
static BOOL		haveBackground = FALSE;
DWORD			thisTickCount;

    thisTickCount = GetTickCount();
    if ((thisTickCount - lastTickCount) <= dwUpdateDelay)
        return;

    // Move to next frame;
    lastTickCount = thisTickCount;
    currentFrame++;
    if (currentFrame > 75) currentFrame = 0;


	if (Texter1) 
	{
		Texter1->RunOnce();
		FrameBuffer->DrawDouble(BackBuffer);

		hDC = GetDC(ghMainWnd);
		BackBuffer->Draw(hDC,0,0);
		DeleteDC(hDC);
	}
}
*/

/*--------------------------------------------------------------*/
int WINAPI WinMain( HINSTANCE hInstance,
    HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int nCmdShow )
/*--------------------------------------------------------------*/
{
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
	
	
	wposX = (GetSystemMetrics(SM_CXSCREEN) - WINDOWB_W) / 2; 
	wposY = (GetSystemMetrics(SM_CYSCREEN) - WINDOWB_H) / 2;


//	hWnd = CreateWindowEx( WS_EX_TOOLWINDOW,
	hWnd = CreateWindow(
	  "WINtroAppClass",
      WINDOW_T,
	  WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 
      0,0,(WINDOW_W*2) + 6, (WINDOW_H*2) + 6,
      NULL,NULL,hInstance,NULL
	);

	ghMainWnd = hWnd;
	ShowWindow( hWnd, nCmdShow );
	ShowedFirst = FALSE;



    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!GetMessage(&msg, NULL, 0, 0))
                return msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else if (g_bActive)
        {
            UpdateFrame(hWnd);
        }
        else
        {
            // Make sure we go to sleep if we have nothing else to do
            WaitMessage();
        }
    }

	return msg.wParam;
}




/*--------------------------------------------------------------*/
LRESULT CALLBACK MainWndProc( HWND hWnd, UINT msg, 
							  WPARAM wParam,LPARAM lParam )
/*--------------------------------------------------------------*/
{
HDC	hDC;
PAINTSTRUCT ps;

	switch( msg ) 
	{
		case WM_ACTIVATEAPP:
            // Pause if minimized or not the top window
            g_bActive = (wParam == WA_ACTIVE) || 
						(wParam == WA_CLICKACTIVE);

//			if (g_bActive == TRUE)	DOut_SetVolumeLog(0);
//			else					DOut_SetVolumeLog(-1000);
            
			return 0L;

		case WM_SHOWWINDOW:
			if (ShowedFirst == FALSE)
			{
				ShowedFirst = TRUE;
				StartAll(hWnd);

				FrameBuffer->Fill(19,0,71, 0,0);
				InfoMtx1->Fill(2);

//				LogoImage->DrawRect(FrameBuffer,8,8, 0,0, 320,64);
//				InfoMtx1->SetRect(1,1, 320/8, 64/8, 2);

//				SetTimer(hWnd,1,20,NULL);
			}

			SetWindowPos( ghMainWnd, HWND_TOP, wposX, wposY, 0,0,SWP_NOSIZE);
 
			break;

		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);

//			BackBuffer->Draw(hDC,0,0);
			BackBuffer->DrawRect(hDC, 
				ps.rcPaint.left, ps.rcPaint.top,	// Dst X,Y
				ps.rcPaint.left, ps.rcPaint.top,	// Src X,Y
        		ps.rcPaint.right-ps.rcPaint.left,	// Size X
				ps.rcPaint.bottom-ps.rcPaint.top);	// Size Y


			EndPaint (hWnd, &ps);
		break;

//		case WM_CREATE:		ShowedFirst = FALSE;		break;

		case WM_DESTROY:
//			KillTimer(hWnd,1);
			CloseAll();
			PostQuitMessage(0);
		break;

        case WM_KEYDOWN:
            // Handle any non-accelerated key commands
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostMessage(hWnd, WM_CLOSE, 0, 0);
                    return 0L;
            }
            break;


//		case WM_TIMER:
//			if (wParam == 1) UpdateFrame(ghMainWnd);
//			break;

      default:
		return( DefWindowProc( hWnd, msg, wParam, lParam ));
	}
	return 0;
}




/*--------------------------------------------------------------*/
void	StartAll(HWND hWnd)
/*--------------------------------------------------------------*/
{

	FrameBuffer = NULL;
	FrameBuffer = new TFastBMP(WINDOW_W,WINDOW_H);

	BackBuffer = NULL;
	BackBuffer = new TFastBMP(WINDOW_W*2,WINDOW_H*2);

	FontImage = NULL;
	FontImage = new TFastBMP(IDB_BITMAP2,ghInstance);

	LogoImage = NULL;
	LogoImage = new TFastBMP((DWORD)IDB_BITMAP1,ghInstance);


	InfoMtx1 = new TMtx(WINDOW_W / 8, WINDOW_H / 8);

	Texter1 = new TTexter(FontImage, FrameBuffer, Matrix, Pages, InfoMtx1);

	chpOpen(Module, (DWORD)ghMainWnd);
}


/*--------------------------------------------------------------*/
void	CloseAll(void)
/*--------------------------------------------------------------*/
{
	chpClose();

	if (Texter1) delete Texter1;
	if (InfoMtx1) delete InfoMtx1;

	if (BackBuffer) delete BackBuffer;
	if (FrameBuffer) delete FrameBuffer;
	if (FontImage) delete FontImage;
	if (LogoImage) delete LogoImage;
}

