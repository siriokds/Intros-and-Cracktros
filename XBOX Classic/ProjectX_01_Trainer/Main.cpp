//Main header file for the XDK


#define	 SCRWIDTH 720

#define	 RES_W 720
#define	 RES_H 240
#define	 BRD_W 24
#define	 BRD_H 18

#define	 FONTWIDTH	8
#define	 FONTHEIGHT	8
#define	 TXTCOLS	40
#define	 TXTROWS	8
#define	 CENTERX	0
#define	 CENTERY	98



#include <xtl.h>
#include <XBApp.h>
#include <XBInput.h>
#include "RemapDrives.h"
#include "Types.h"


char	TrainerTitle[] = 
">>  RALLYSPORT CHALLENGE +5  <<\0\0"
"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

int		TrainerDataSize = 5;
UBYTE	TrainerData[32];


char	Options[] = 
"UNLIMITED AMMO                  "
"NO SPRITE COLLISION             "
"OPTION02                        "
"OPTION03                        "
"OPTION04                        "
"OPTION05                        "
"OPTION06                        "
"OPTION07                        "
"OPTION08                        "
"OPTION09                        "
"OPTION10                        "
"OPTION11                        "
"OPTION12                        "
"OPTION13                        "
"OPTION14                        "
"OPTION15                        "
"OPTION16                        "
"OPTION17                        "
"OPTION18                        "
"OPTION19                        "
"OPTION20                        "
"OPTION21                        "
"OPTION22                        "
"OPTION23                        "
"OPTION24                        "
"OPTION25                        "
"OPTION26                        "
"OPTION27                        "
"OPTION28                        "
"OPTION29                        "
"OPTION30                        "
"OPTION31                        ";


char	credits1[] = "          >> INTRO CREDITS <<           ";
char	credits2[] = " INTRO CODING ............ SIRIOKD/TRSI ";
char	credits3[] = " 2D FONT .................... ADEC/TRSI ";
char	credits4[] = " LOGO ..................... J.O.E./TRSI ";
char	credits5[] = " FC REPLAYER ............. SIRIOKD/TRSI ";
char	credits6[] = " MUZAK ........................... TRSI ";
char	credits7[] = "              PRESS START               ";

int	OptionMaxChar;
int	OptionCursor = 0;
int	OptionPageCursor = 0;
int OptionSineCursor = 0;
int OptionCursorMoveWait = 0;
int OptionCursorMoveMax = 0;


#include "FC_Lib.h"
#include "pirate.h"

#include "Gfx.h"
#include "scroll.h"
#include "Fntz.h"
#include "Fntz2.h"
#include "sintab.h"
#include "logo.h"
#include "siriokd2.h"


int			resX, resY;

int			stretchX = 1;
int			stretchY = 0;
int			TrainerGo = 0;
int			TrainerFade = 0;
int			TrainerColor = 0xFFFFFFFF;





UBYTE	fill____01[1024];

DWORD	palette[256];

UBYTE	fill____02[1024];

UBYTE	Frame[RES_W*RES_H*2];

UBYTE	fill____03[1024];

UBYTE	FramT[RES_W*160];

UBYTE	fill____04[1024];



//char	fntMtx[]  = " ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890()+,!'@/><:.-";
char	fntMtx[]  = " ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.,'^+-?!()></";
char	fntMtx2[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890:*?!()+-,./#";

struct	scrInfo		scroll_Info;
struct	scrInfo		scroll_Info2;

UBYTE	fill____05[1024];

/*
char	page1[] = 
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
*/

//char	*pages[] = { page1, page2, page3, 0 };

char	*pages[] = { 0 };

// 0         1         2         3         4
// 01234567890123456789012345678901234567890
// -------------------------||--------------


/*----------------------------------------------------------*/
void	TrainerLaunch(void)
/*----------------------------------------------------------*/
{
	RemapDrive('T');

	HANDLE hTrn = CreateFile("T:\\pjx", 
							GENERIC_WRITE, 0, NULL, 
							CREATE_ALWAYS, 
							FILE_ATTRIBUTE_NORMAL, NULL);


	if (hTrn != INVALID_HANDLE_VALUE)
	{
		DWORD written = 0;
		WriteFile(hTrn, (LPCVOID)TrainerData, (DWORD)TrainerDataSize, &written, NULL);
		FlushFileBuffers(hTrn);
		CloseHandle(hTrn);

		LAUNCH_DATA LaunchData;	
		XLaunchNewImage( "d:\\pjx", &LaunchData );
	}
}





int	startPressed[2] = { 0, 0 };

/*----------------------------------------------------------*/
void	TrainerStart(void)
/*----------------------------------------------------------*/
{
		TrainerGo++;
}



/*----------------------------------------------------------*/
int		OptionStrLen(int item)
/*----------------------------------------------------------*/
{
int len = 32;
char *p = Options+(item*32)+31;

	while ((len > 0) && (*p == ' ')) { *p-- = 0; len--; }

//	Options[(item*32)+31] = 0;
	if (len > 0) Options[(item*32)+len] = 0;

	return len;
}


/*----------------------------------------------------------*/
void	OptionsInit(void)
/*----------------------------------------------------------*/
{
int i,k;

	OptionMaxChar = 0;
	TrainerDataSize &= 31;

	for (i = 0; i < 32; i++)
	{
		TrainerData[i] = 0;

		k = OptionStrLen(i); if (k > OptionMaxChar) OptionMaxChar = k;

	}

	OptionCursorMoveMax = 7;
	if (TrainerDataSize+1 < OptionCursorMoveMax) OptionCursorMoveMax = TrainerDataSize+1;
/*
	TrainerData[0] = 0;
	TrainerData[1] = 1;
	TrainerData[2] = 1;
	TrainerData[3] = 0;
	TrainerData[4] = 1;
*/
}



/*----------------------------------------------------------------------*/
void	OptionsWriteChar(int x, int y, int shiftX, int shiftY, char c)
/*----------------------------------------------------------------------*/
{
	scrWriteChar(&scroll_Info, c, (char*)FramT+shiftX
									+(((y*10)+shiftY)*RES_W)
									+(RES_W*4)
									+(x*8), RES_W, 1);
}

/*----------------------------------------------------------*/
void	OptionsWriteString(int x, int y, int shiftX, int shiftY, char *s, int len)
/*----------------------------------------------------------*/
{
	for (int i = 0; i < len; i++) 
		OptionsWriteChar(x+i,y,shiftX, shiftY, *s++);
}

/*----------------------------------------------------------*/
void	OptionsWriteStringZ(int x, int y, int shiftX, int shiftY, char *s)
/*----------------------------------------------------------*/
{
	for (int i = 0; *s; i++) 
		OptionsWriteChar(x+i,y, shiftX, shiftY, *s++);
}

/*----------------------------------------------------------*/
void	OptionsWriteStringZCentered(int y, char *s)
/*----------------------------------------------------------*/
{
int len = strlen(s);
int posx = ((RES_W>>3) - len) >> 1;


	for (int i = 0; *s; i++) 
		OptionsWriteChar(posx+i,y,0,0,*s++);
}


/*----------------------------------------------------------*/
void	OptionsWriteCharRepCentered(int y, char c, int len)
/*----------------------------------------------------------*/
{
int posx = ((RES_W>>3) - len) >> 1;


	for (int i = 0; i < len; i++) 
		OptionsWriteChar(posx+i,y,0,0,c);
}

/*-----------------------------------------------------------------------*/
void	OptionsWriteItem(int x, int y, int item, int onoff, int cursor)
/*-----------------------------------------------------------------------*/
{
int shift = 0;

	if (cursor)	shift += -16+(sinetab[OptionSineCursor]>>2);


	if (item >= TrainerDataSize)
	{
		char startString[] = "START GAME";
		int len = strlen(startString);
		OptionsWriteStringZ(x+OptionMaxChar-len+1, y, shift, 4, startString);
		return;
	}


char *s = Options+(item*32);
int  i,len = strlen(s);

	OptionsWriteStringZ(x, y, shift, 4, s);
	for (i = 0; i < OptionMaxChar-len+7; i++) OptionsWriteChar(x+len+1+i, y, shift, 4, '.');
	OptionsWriteStringZ(x+OptionMaxChar+9, y, shift, 4, (onoff)?"YES":"NO");
}


/*-----------------------------------------------------------------------*/
void	OptionsWriteStart(int x, int y, int cursor)
/*-----------------------------------------------------------------------*/
{
int shift = 0;
char startString[] = "START GAME";
int len = strlen(startString);


	if (cursor)	shift += -16+(sinetab[OptionSineCursor]>>2);

	OptionsWriteStringZ(x+OptionMaxChar-len+1, y, shift, 4, startString);
}





/*----------------------------------------------------------*/
void	DrawIntroInfo(void)
/*----------------------------------------------------------*/
{
	if (TrainerGo == 2) TrainerFade = 1;


static	ULONG m_RND = 0x7BF89A12;


	if (TrainerGo > 180)
	{
		UBYTE a,b,c,d;
		for (int i = 0; i < (TXTROWS*(FONTHEIGHT+2)); i++)
		{
			int offDst = ((i+CENTERY)*RES_W) + CENTERX;

			for (int j = 0; j < RES_W; j += 8)
			{
					m_RND = ((m_RND << 7) + (m_RND >> 25) + 0x6ECA756D) ^ 0x9E59A92B;

					a = (UBYTE)(((m_RND    ) & 15)+16);
					b = (UBYTE)(((m_RND>> 8) & 15)+16);
					c = (UBYTE)(((m_RND>>16) & 15)+16);
					d = (UBYTE)(((m_RND>>24) & 15)+16);
					
					Frame[offDst+j+0] = Frame[offDst+j+1] = a;
					Frame[offDst+j+2] = Frame[offDst+j+3] = b;
					Frame[offDst+j+4] = Frame[offDst+j+5] = c;
					Frame[offDst+j+6] = Frame[offDst+j+7] = d;
			}
		}
	}
	else
	{

		for (int i = 0; i < 80; i++) fastSet(FramT+(RES_W*i), 255, RES_W);

		OptionsWriteStringZ(25,0,0,0,credits1);
		OptionsWriteStringZ(25,1,0,4,credits2);
		OptionsWriteStringZ(25,2,0,4,credits3);
		OptionsWriteStringZ(25,3,0,4,credits4);
		OptionsWriteStringZ(25,4,0,4,credits5);
		OptionsWriteStringZ(25,5,0,4,credits6);

		static int k = 30;
		if (++k < 40) OptionsWriteStringZ(25,6,0,4,credits7);
		k &= 63;



		for (int i = 0; i < 80; i++)
			fastCpy(Frame+(CENTERY*RES_W)+(RES_W*i), FramT+(RES_W*i), RES_W);
	}

}



/*----------------------------------------------------------*/
void	DrawOptions(void)
/*----------------------------------------------------------*/
{
	for (int i = 0; i < 80; i++) fastSet(FramT+(RES_W*i), 255, RES_W);


int	len = strlen(TrainerTitle);

	OptionsWriteStringZCentered(0,TrainerTitle);
//	OptionsWriteCharRepCentered(1,'-', len);



int cursor = OptionCursor;
int centerx = ((RES_W>>3)-(OptionMaxChar+12))>>1;
int centery = 1;


int	sta = OptionPageCursor;

	for (i = 0; i < OptionCursorMoveMax; i++)
		OptionsWriteItem(centerx, centery+i, sta+i, TrainerData[sta+i], (cursor==i)?1:0);



	for (int i = 0; i < 80; i++)
		fastCpy(Frame+(CENTERY*RES_W)+(RES_W*i), FramT+(RES_W*i), RES_W);

	OptionSineCursor = (OptionSineCursor + 9) & 511;
}



/*----------------------------------------------------------*/
void	DrawSirioKD(void)
/*----------------------------------------------------------*/
{
	UBYTE *src = skd+16;

	UBYTE *dst = Frame+(RES_W*172)+ RES_W-120;


	for (int i = 0; i < 8; i++)
	{
		int offY = (i*RES_W);

		for (int j = 0; j < 32; j++)
		{
			UBYTE v = src[(i<<5)+j];
			if (v!=0) 
			{
				dst[offY + (j*2) + 0] = dst[offY + (j*2) + 1] = v;
			}
		}
	}

}



#define STAR_NUM 240

int		star_pos[STAR_NUM];
int		star_spd[STAR_NUM];
UBYTE	rast[RES_W];
int		rastI[RES_W*3];
int		*rastIdx = rastI+RES_W;



#define	SCRPITCH (720+16)
UBYTE	scroll_buf[SCRPITCH*32];
int		scroll_shf = 0;
int		scroll_txtpos = 0;

char	scroll_txt[] = 
 "SIRIOKD / TRSI PROUDLY PRESENTS...  RALLYSPORT CHALLENGE +16 MEGATRAINER.... "
 "GAME CRACKED BY: - FAIRLIGHT -...   "
 "AND AS USUAL JUST RELEASED AND TRAINED BY PROJECTX...  "
 "IF U WANNA TO CONTACT US... GO TO: WWW.TRSI.ORG       "
 "  INTRO CODING BY SIRIOKD/TRSI...       r"
 "                                                     "
 "                                                     "
 "                                                     "
 "                                                     "
 "                                                     "
 "#STOP#";


/*----------------------------------------------------------*/
void	StarsInit(void)
/*----------------------------------------------------------*/
{
static	ULONG m_RND = 0x3B9AF812;

	for (int i = 0; i < STAR_NUM; i++)
	{
		m_RND = ((m_RND << 7) + (m_RND >> 25) + 0x1ECA256D) ^ 0x9E59A92B;
		star_pos[i] = (m_RND & 511) % RES_W;
		star_spd[i] = ((m_RND >> 24) & 3)+1;
	}

	for (i = 0; i < (RES_W/2); i++) { rast[i] = 64 + ((i/12)&31); rast[i+(RES_W/2)] = 64 + ((31-(i/12))&31); }
	for (i = 0; i < RES_W*3; i++) rastI[i] = i%RES_W;

	for (i = 0; i < SCRPITCH*16; i++) scroll_buf[i] = 243;

}

/*----------------------------------------------------------*/
void	StarsDraw(void)
/*----------------------------------------------------------*/
{
UBYTE *dst;
int i;

int	cols[] = { 18, 20, 22, 24, 26 };


	dst = Frame;// + (RES_W*82);

	for (i = 0; i < STAR_NUM; i++)
	{
		int pos = star_pos[i] + (i*RES_W);

		if ((dst[pos] == 31) || (dst[pos] == 255)) dst[pos] = cols[ star_spd[i] ];
		star_pos[i] += star_spd[i];
		if (star_pos[i] >= RES_W) star_pos[i] -= RES_W;
	}
}



static	int	sineofs = 0;

/*---------------------------------------------------------------*/
void	sinetabSCROLL_FWD(int cnt)
/*---------------------------------------------------------------*/
{
	while (cnt--) sineofs = (sineofs - 1) & 511;
}


/*----------------------------------------------------------*/
void	ScrollShift(void)
/*----------------------------------------------------------*/
{
	for (int j = 0; j < 16; j++) { int off = j * SCRPITCH; for (int i = 0; i < SCRPITCH-1; i++) scroll_buf[off+i] = scroll_buf[off+i+1]; }
	scroll_shf++;
}


/*----------------------------------------------------------*/
void	ScrollDraw(void)
/*----------------------------------------------------------*/
{
#define	M_PI 3.14159265358979323846264338328f

UBYTE *dst,*src = scroll_buf;
int i;

	dst = Frame+(RES_W*188);

static float vY = 0.0f;
static float vX = 0.0f;


	int bounce = 4*RES_W;


//	for (i = 0; i < 16; i++) fastCpy(dst+(RES_W*i)+bounce, scroll_buf+(SCRPITCH*i), RES_W); 

	for (i = 0; i < RES_W; i++)
	{
		int sine   = sinetab[ (i+(sineofs )) & 511 ];

		int offset = (sine>>3) * RES_W;

		for (int k = 0; k < 16; k++)
		{
			UBYTE v = src[SCRPITCH * k] & 255;
			if (v == 243) continue;

			dst[offset+(RES_W * k)] = ( (v == 240) ? 144+(sine>>2) : v );

		}
		src++; dst++;
	}

	sinetabSCROLL_FWD(4);


	ScrollShift();
	ScrollShift();

	if (scroll_shf > 16)
	{
		scroll_shf = 0;
		char c = scroll_txt[scroll_txtpos];
		if (c == 'r') { scroll_txtpos = 0; c = scroll_txt[scroll_txtpos]; }

		scrWriteChar16(&scroll_Info2, scroll_txt[scroll_txtpos], (char*)scroll_buf+SCRPITCH-16, SCRPITCH, 1);
		scroll_txtpos++; if (scroll_txt[scroll_txtpos] == 0) scroll_txtpos = 0;
	}


}




/*----------------------------------------------------------*/
void	DrawLogoRow(int row, int x, int y)
/*----------------------------------------------------------*/
{
	if (x >= RES_W) return;
	if ((x + 452) < 0) return;

UBYTE *src = logo + 778 + (452*row);
int len = 452;

	if (x < 0) { len += x; src -= x; x = 0; }
	if ((x + len) > RES_W) len = RES_W-x;

UBYTE *dst = Frame + (RES_W*y) + x;

	fastCpy(dst, src, len);
}


static	int		logoSinPtr = 128;
static	float	logoSinAPtr = 1.99f;
static	float	logoSinAInc = -0.002f;

/*----------------------------------------------------------*/
void	DrawLogo(void)
/*----------------------------------------------------------*/
{
UBYTE *dst;

	for (int i = 0; i < 86; i++) 
	{
		float off = (float)(sinetab[(logoSinPtr+i)&511]-88) * logoSinAPtr;
		DrawLogoRow(i, 134 + (int)off, 8+i);

	}

	logoSinAPtr += logoSinAInc;
	if ((logoSinAPtr > 3.0f) || (logoSinAPtr < 0.2f)) logoSinAInc = -logoSinAInc;
	logoSinPtr++;


static int redline = 0;
	dst = Frame + (RES_W*96);
	dst += RES_W;		for (i = 0; i < RES_W; i++) *dst++ = rast[ rastIdx[i+redline] ];
	dst += RES_W*80;	for (i = 0; i < RES_W; i++) *dst++ = rast[ rastIdx[i-redline] ];

	redline += 8; if (redline > RES_W) redline -= RES_W;

}



/*----------------------------------------------------------*/
void	DrawTexter(void)
/*----------------------------------------------------------*/
{
static	ULONG m_RND = 0x7BF89A12;

	scrExecute(&scroll_Info);

	if (scroll_Info.Wait == 0)
	{
		UBYTE a,b,c,d;
		for (int i = 0; i < (TXTROWS*(FONTHEIGHT+2)); i++)
		{
			int offDst = ((i+CENTERY)*RES_W) + CENTERX;

			for (int j = 0; j < RES_W; j += 8)
			{
					m_RND = ((m_RND << 7) + (m_RND >> 25) + 0x6ECA756D) ^ 0x9E59A92B;

					a = (UBYTE)(((m_RND    ) & 15)+16);
					b = (UBYTE)(((m_RND>> 8) & 15)+16);
					c = (UBYTE)(((m_RND>>16) & 15)+16);
					d = (UBYTE)(((m_RND>>24) & 15)+16);
					
					Frame[offDst+j+0] = Frame[offDst+j+1] = a;
					Frame[offDst+j+2] = Frame[offDst+j+3] = b;
					Frame[offDst+j+4] = Frame[offDst+j+5] = c;
					Frame[offDst+j+6] = Frame[offDst+j+7] = d;
			}
		}
	}
	else
	{
		for (int i = 0; i < 80; i++)
			fastCpy(Frame+(CENTERY*RES_W)+(RES_W*i), FramT+(RES_W*i), RES_W);
	}
}



/*----------------------------------------------------------*/
void	CursorUp(void)
/*----------------------------------------------------------*/
{

	if (OptionCursor > 0) OptionCursor--;
	else
	{
		if (OptionPageCursor > 0) OptionPageCursor--;
	}


//	if (OptionPageCursor > 0) OptionPageCursor--;
}

/*----------------------------------------------------------*/
void	CursorDown(void)
/*----------------------------------------------------------*/
{

	if (OptionCursor < OptionCursorMoveMax-1) OptionCursor++;
	else
	{
		if ((OptionPageCursor+OptionCursorMoveMax) < TrainerDataSize+1) OptionPageCursor++;
	}


//	if ((OptionPageCursor+OptionCursorMoveMax) < TrainerDataSize-1) OptionPageCursor++;
}


/*----------------------------------------------------------*/
void	ChangeOption(void)
/*----------------------------------------------------------*/
{
int	pos = (OptionPageCursor + OptionCursor);


	if (pos == TrainerDataSize) {TrainerStart(); return; }

	pos = (pos < 0) ? 0 : ((pos >= TrainerDataSize) ? TrainerDataSize-1 : (pos));

	TrainerData[pos] = (TrainerData[pos] + 1) & 1;

}


/*----------------------------------------------------------*/
void	CursorDo(int op)
/*----------------------------------------------------------*/
{
	if (OptionCursorMoveWait == 0) 
	{
		switch (op)
		{
			case 0: CursorUp();		OptionCursorMoveWait = 5; break;
			case 1: CursorDown();	OptionCursorMoveWait = 5; break;
			case 2: ChangeOption();	OptionCursorMoveWait = 200;break;
		}

	}
	else
		OptionCursorMoveWait--;
}


/*----------------------------------------------------------*/
void	UpdateFrame(void)
/*----------------------------------------------------------*/
{
	fastSet(Frame,31,RES_W*240);


	if (TrainerGo == 0)	DrawOptions();
	else				DrawIntroInfo();



	
	DrawLogo();

	ScrollDraw();

	StarsDraw();

	DrawSirioKD();


}




/*--------------------------------------------------------------*/
void	tdxSetPal(UBYTE *pal)
/*--------------------------------------------------------------*/
{
	int r,g,b;

	for (int i = 0; i < 256; i++)
	{
		r = pal[0] << 2;
		g = pal[1] << 2;
		b = pal[2] << 2;
		pal += 3;

		palette[i] = (r << 16) | (g << 8) | b;
	}

}













/*----------------------------------------------------------------*/
class CXBoxSample : public CXBApplication
/*----------------------------------------------------------------*/
{
public:

	int isPAL50;

	LPDIRECT3DTEXTURE8      m_pVideoTexture;
	LPDIRECT3DVERTEXBUFFER8 m_pVB;

		    CXBoxSample();
    HRESULT Initialize();       // Initialize the sample
    HRESULT DoIntro();
    HRESULT Render();           // Render the scene
    HRESULT FrameMove();        // Perform per-frame updates

};
CXBoxSample xbApp;


//-----------------------------------------------------------------------------
// Name: main()
// Desc: Entry point to the program.
//-----------------------------------------------------------------------------
VOID __cdecl main()
{
    if( FAILED( xbApp.Create() ) ) return;
    xbApp.Run();
}





//-----------------------------------------------------------------------------
// Name: CXBoxSample (constructor)
// Desc: Constructor for CXBoxSample class
//-----------------------------------------------------------------------------
CXBoxSample::CXBoxSample() 
            :CXBApplication()
{
	isPAL50 = 1;
	if (XGetVideoStandard() != XC_VIDEO_STANDARD_PAL_I)  isPAL50 = 0;
	else if (XGetVideoFlags() & XC_VIDEO_FLAGS_PAL_60Hz) isPAL50 = 0; 

	m_d3dpp.FullScreen_RefreshRateInHz = (isPAL50 != 0) ? 50 : 60;
	m_d3dpp.BackBufferWidth = resX = SCRWIDTH;

	resY = ((isPAL50==0)||(stretchY==1)) ? 480 : 576;
	m_d3dpp.BackBufferHeight = resY;

	m_d3dpp.BackBufferFormat       = D3DFMT_X8R8G8B8;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	m_d3dpp.BackBufferCount        = 1;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
}




//-----------------------------------------------------------------------------
void	SetPal(UBYTE *pal, int idx, int r, int g, int b)
//-----------------------------------------------------------------------------
{
	idx *= 3;
	pal[idx    ] = r;
	pal[idx + 1] = g;
	pal[idx + 2] = b;
}

 
//-----------------------------------------------------------------------------
HRESULT CXBoxSample::Initialize()
//-----------------------------------------------------------------------------
{
	m_pd3dDevice->CreateTexture( RES_W, RES_H, 1, 0, D3DFMT_LIN_X8R8G8B8, D3DPOOL_MANAGED, &m_pVideoTexture);
    m_pd3dDevice->CreateVertexBuffer( 4*(6*sizeof(FLOAT) + sizeof(DWORD)), D3DUSAGE_WRITEONLY, 0L, D3DPOOL_DEFAULT, &m_pVB );


	scroll_Info.scrFont = (char*)imx+778;		scroll_Info.scrFontRow = 400;
	scroll_Info.scrDest = (char*)FramT+180;		scroll_Info.scrDestRow = RES_W;
	scroll_Info.scrPages = pages;				scroll_Info.scrFontMtx = fntMtx;
	scroll_Info.scrFontW = FONTWIDTH;			scroll_Info.scrFontH = FONTHEIGHT;
	scroll_Info.col = TXTCOLS;

	scrInit(&scroll_Info);
	scroll_Info.Wait = 10;
	scroll_Info.scrPage = 3;


	scroll_Info2.scrFont = (char*)imx2+778;		
	scroll_Info2.scrFontRow = 1024;
	scroll_Info2.scrFontMtx = fntMtx2;
	scroll_Info2.scrFontW = 16;					
	scroll_Info2.scrFontH = 16;


	StarsInit();
	OptionsInit();


UBYTE	myPal[768];

	for (int i = 0; i < 16*3; i++) myPal[i] =  imx[10+i];				// 00-15
	for (i = 0; i < 16; i++) SetPal(myPal,16+i, i*4,i*4,i*4);			// 16-31
	for (i = 0; i < 32; i++) SetPal(myPal,64+i, ((i*5)>>2),((i*4)>>2), 0);		// 64-95


	for (i = 0; i < 32*3;i++) myPal[(32*3)+i] = logo[10+i];				// 32-63
	for (i = 0; i < 452*86; i++) if (logo[778+i] != 31) logo[778+i] += 32;

	SetPal(skd,1,4,4,8);
	for (i = 0; i < 4*3; i++) myPal[(96*3)+i] = skd[i];					// 96-99
	for (i = 0; i < 32*8; i++) if (skd[16+i]) skd[16+i] += 96;


	for (i = 0; i < 4*3; i++) myPal[i+(240*3)] = imx2[10+i];			// 240-251
	for (i = 0; i < 1024*16; i++) imx2[778+i] += 240;


	for (i = 0; i < 16; i++) SetPal(myPal,144+i, 63,           i*4, 0);		// 144-159
	for (i = 0; i < 16; i++) SetPal(myPal,160+i, 63,      63-(i*4), (i*4));	// 160-175
	for (i = 0; i < 16; i++) SetPal(myPal,176+i, 63-(i*4),       0, 63);	// 176-191
	for (i = 0; i < 16; i++) SetPal(myPal,192+i, 0,          (i*4), 63);	// 192-207


	SetPal(myPal, 31, 0,0,0);
	SetPal(myPal,128, 0,8,18);
	SetPal(myPal,255, 0,2,6);


	tdxSetPal(myPal);

	FCp_Open(isPAL50);
	FCp_InitModule((char*)Module);
	FCp_Start();

    return S_OK;
}


//-----------------------------------------------------------------------------
HRESULT CXBoxSample::FrameMove()
//-----------------------------------------------------------------------------
{ 
    return S_OK;
}





//------------------------------------------------------------------------------
HRESULT CXBoxSample::DoIntro()
//------------------------------------------------------------------------------
{
	UpdateFrame();


D3DLOCKED_RECT d3dlr;

	m_pVideoTexture->LockRect(0,&d3dlr,0,0);

	UBYTE	*src = Frame;
	DWORD	*dst = (DWORD*)d3dlr.pBits;
	int		pitch = d3dlr.Pitch>>2;

	if (dst) 
	{
		for (int y = 0; y < RES_H; y++)
		{
			for (int x = 0; x < RES_W; x++) *dst++ = palette[*src++];
			dst += pitch - RES_W;
		}
	}


	m_pVideoTexture->UnlockRect(0);


//------------------------------------------------------------------------------

	// Set state to render the gamepad image
	m_pd3dDevice->SetTexture( 0, m_pVideoTexture );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,  D3DTADDRESS_CLAMP );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,  D3DTADDRESS_CLAMP );
	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,      FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,    FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_NONE );
	m_pd3dDevice->SetRenderState( D3DRS_FILLMODE,     D3DFILL_SOLID );
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,     D3DCULL_CCW );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	m_pd3dDevice->SetVertexShader( D3DFVF_XYZRHW|D3DFVF_TEX1|D3DFVF_DIFFUSE  );

	struct VERTEX { D3DXVECTOR4 p; D3DCOLOR vbColor; FLOAT tu, tv; };
    VERTEX* v;

	float sizeX = 720;
	float offsX = 0;

	float sizeY = RES_H*2;
	float offsY = (resY==480)?0:48.0f;



    m_pVB->Lock( 0, 0, (BYTE**)&v, 0L );
    v[0].p = D3DXVECTOR4(offsX+    0,offsY+    0, 0, 0 );  v[0].vbColor = TrainerColor; v[0].tu =      0;  v[0].tv =      0;
    v[1].p = D3DXVECTOR4(offsX+sizeX,offsY+    0, 0, 0 );  v[1].vbColor = TrainerColor; v[1].tu = 719.5f;  v[1].tv =      0;
    v[2].p = D3DXVECTOR4(offsX+sizeX,offsY+sizeY, 0, 0 );  v[2].vbColor = TrainerColor; v[2].tu = 719.5f;  v[2].tv = 239.5f;
    v[3].p = D3DXVECTOR4(offsX+    0,offsY+sizeY, 0, 0 );  v[3].vbColor = TrainerColor; v[3].tu =      0;  v[3].tv = 239.5f;
    m_pVB->Unlock();

	m_pd3dDevice->SetStreamSource( 0, m_pVB, 6*sizeof(FLOAT) + sizeof(DWORD) );
	m_pd3dDevice->DrawPrimitive( D3DPT_QUADLIST, 0, 1 );

	
	return S_OK;
}


//------------------------------------------------------------------------------
HRESULT CXBoxSample::Render()
//------------------------------------------------------------------------------
{
static	int startGo = 0;

	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,  0, 0), 1.0f, 0);
	FCp_Run();

	if (startGo < 15)	startGo++;
	else				DoIntro();

	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);



	if (TrainerGo == 0)
	{
		if (m_DefaultGamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) CursorDo(0);
		else
			if (m_DefaultGamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) CursorDo(1);
			  else
				if ( m_DefaultGamepad.bAnalogButtons[XINPUT_GAMEPAD_A] > 0 ) CursorDo(2);
				else
				  OptionCursorMoveWait = 0;
	}


	if (TrainerFade)
	{
		if (TrainerColor != 0x00FFFFFF) TrainerColor -= 0x01000000;
		else							TrainerLaunch();

		if (TrainerColor != 0x00FFFFFF) TrainerColor -= 0x01000000;
		else							TrainerLaunch();
	}
	else 
	{
		if (m_DefaultGamepad.wButtons & XINPUT_GAMEPAD_START)	
		{
			if (startPressed[0] == 0)
			{
				startPressed[0] = 1;
				startPressed[1] = 0;
				TrainerStart();
			}
		}
		else													
		{
			if (startPressed[0] == 1)
			{
				startPressed[0] = 0;
				startPressed[1] = 1;
			}
		}
	}

	return S_OK;
}



