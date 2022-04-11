//Main header file for the XDK

#define	 SCRWIDTH 720

#define	 RES_W 360
#define	 RES_H 240
#define	 BRD_W 24
#define	 BRD_H 18

#define	 FONTWIDTH	8
#define	 FONTHEIGHT	8
#define	 TXTCOLS	40
#define	 TXTROWS	10
#define	 CENTERX	0
#define	 CENTERY	82



#include <xtl.h>
#include <XBApp.h>
#include <XBInput.h>
#include "Types.h"
#include "FC_Lib.h"
#include "tristar.h"

#include "Gfx.h"
#include "scroll.h"
#include "Fntz.h"
#include "logo.h"
#include "siriokd2.h"


int			resX, resY;

int			stretchX = 1;
int			stretchY = 0;





UBYTE	fill____01[1024];

DWORD	palette[256];

UBYTE	fill____02[1024];

UBYTE	Frame[RES_W*RES_H*2];

UBYTE	fill____03[1024];

UBYTE	FramT[360*200];

UBYTE	fill____04[1024];



char	fntMtx[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890()+,!'@/><:.-";

struct	scrInfo		scroll_Info;

UBYTE	fill____05[1024];

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

	UBYTE *dst = Frame+(RES_W*177)+ RES_W-60;


	for (int i = 0; i < 8; i++)
	{
		int offY = (i*RES_W);

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
UBYTE	rast[RES_W];
int		rastI[RES_W*3];
int		*rastIdx = rastI+RES_W;



#define	SCRPITCH 412
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
		star_pos[i] = (m_RND & 511) % RES_W;
		star_spd[i] = ((m_RND >> 24) & 3)+1;
	}

	for (i = 0; i < (RES_W/2); i++) { rast[i] = 64 + ((i/6)&31); rast[i+(RES_W/2)] = 64 + ((31-(i/6))&31); }
	for (i = 0; i < RES_W*3; i++) rastI[i] = i%RES_W;

	for (i = 0; i < SCRPITCH*10; i++) scroll_buf[i] = 32;

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
		int pos = star_pos[i] + ((i<80)?(i*RES_W):((i+107)*RES_W));

		if (dst[pos] == 32) dst[pos] = cols[ star_spd[i] ];
		star_pos[i] += star_spd[i];
		if (star_pos[i] >= RES_W) star_pos[i] -= RES_W;
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

	dst = Frame+(RES_W*200);

static float vY = 0.0f;
static float vX = 0.0f;


	int bounce = (int(sin(vX)*10)+4)*RES_W;


	for (i = 0; i < 16; i++)
	{
		shf[i] = int(sin(vY)*32)+32;
		vY -= (M_PI / 700); 
		if (vY > M_PI) vY -= M_PI;
	}

	int l = 0;
	for (i = 0; i < 8; i++) 
	{ 
		fastCpy(dst+((RES_W*2)*i)+bounce,      scroll_buf+(SCRPITCH*i)+shf[l++], RES_W); 
		fastCpy(dst+((RES_W*2)*i)+RES_W+bounce,scroll_buf+(SCRPITCH*i)+shf[l++], RES_W); 
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
	UBYTE *dst = Frame + ((80-movlogo)*RES_W) + 20;

	for (int i = 0; i < movlogo; i++) 
	{
		fastCpy(dst+(RES_W*i),src+(320*i),320);
	}
	if (movlogo < 80) movlogo++;



	dst = Frame + (RES_W*80);
	dst += RES_W;		for (i = 0; i < RES_W; i++) *dst++ = rast[ rastIdx[i+redline] ];
	dst += RES_W*100;	for (i = 0; i < RES_W; i++) *dst++ = rast[ rastIdx[i-redline] ];

	redline += 8; if (redline > RES_W) redline -= RES_W;

}



/*----------------------------------------------------------*/
void	UpdateFrame(void)
/*----------------------------------------------------------*/
{
static	ULONG m_RND = 0x7BF89A12;

	fastSet(Frame,32,RES_W*240);



	if (scroll_Info.Wait == 0)
	{
		for (int i = 0; i < (TXTROWS*(FONTHEIGHT+2)); i++)
		{
			int offDst = ((i+CENTERY)*RES_W) + CENTERX;

			for (int j = 0; j < RES_W; j+=4)
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
		for (int i = 0; i < 100; i++)
			fastCpy(Frame+(CENTERY*RES_W)+(RES_W*i), FramT+(360*i), 360);
	}
			
	DrawLogo();

	ScrollDraw();
	StarsDraw();
	scrExecute(&scroll_Info);
	DrawSirioKD();
	UBYTE *dst = Frame + (RES_W*226); for (int i = 0; i < 14; i++) fastCpy(dst+(i*RES_W),dst-((i+1)*(RES_W*2)),RES_W);

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
HRESULT CXBoxSample::Initialize()
//-----------------------------------------------------------------------------
{
	m_pd3dDevice->CreateTexture( RES_W, RES_H, 1, 0, D3DFMT_LIN_X8R8G8B8, D3DPOOL_MANAGED, &m_pVideoTexture);
    m_pd3dDevice->CreateVertexBuffer( 4*(6*sizeof(FLOAT) + sizeof(DWORD)), D3DUSAGE_WRITEONLY, 0L, D3DPOOL_DEFAULT, &m_pVB );


	scroll_Info.scrFont = (char*)imx+778;	scroll_Info.scrFontRow = 416;
	scroll_Info.scrDest = (char*)FramT;		scroll_Info.scrDestRow = 360;
	scroll_Info.scrPages = pages;			scroll_Info.scrFontMtx = fntMtx;
	scroll_Info.scrFontW = FONTWIDTH;		scroll_Info.scrFontH = FONTHEIGHT;
	scroll_Info.col = TXTCOLS;

	scrInit(&scroll_Info);
	scroll_Info.Wait = 10;
	scroll_Info.scrPage = 3;

	StarsInit();



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
HRESULT CXBoxSample::Render()
//------------------------------------------------------------------------------
{
	FCp_Run();

	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,  0, 0), 1.0f, 0);


//-----------------------------------

	UpdateFrame();

//-----------------------------------


D3DLOCKED_RECT d3dlr;

	m_pVideoTexture->LockRect(0,&d3dlr,0,0);

	UBYTE	*src = Frame;
	DWORD	*dst = (DWORD*)d3dlr.pBits;
	int		pitch = d3dlr.Pitch>>2;

//	if (resY == 576) dst += pitch * 48;

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

	float sizeY = 576;
	float offsY = 0;

	DWORD mColor = 0xFFFFFFFF;

    m_pVB->Lock( 0, 0, (BYTE**)&v, 0L );
    v[0].p = D3DXVECTOR4(offsX+    0,offsY+    0, 0, 0 );  v[0].vbColor = mColor; v[0].tu =      0;  v[0].tv =      0;
    v[1].p = D3DXVECTOR4(offsX+sizeX,offsY+    0, 0, 0 );  v[1].vbColor = mColor; v[1].tu = 359.5f;  v[1].tv =      0;
    v[2].p = D3DXVECTOR4(offsX+sizeX,offsY+sizeY, 0, 0 );  v[2].vbColor = mColor; v[2].tu = 359.5f;  v[2].tv = 287.5f;
    v[3].p = D3DXVECTOR4(offsX+    0,offsY+sizeY, 0, 0 );  v[3].vbColor = mColor; v[3].tu =      0;  v[3].tv = 287.5f;
    m_pVB->Unlock();

	m_pd3dDevice->SetStreamSource( 0, m_pVB, 6*sizeof(FLOAT) + sizeof(DWORD) );
	m_pd3dDevice->DrawPrimitive( D3DPT_QUADLIST, 0, 1 );

	
//-----------------------------------
	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);


	if ( m_DefaultGamepad.bAnalogButtons[XINPUT_GAMEPAD_A] > 0 )
	{
		LAUNCH_DATA LaunchData;
		XLaunchNewImage( "d:\\game.xbe", &LaunchData );
	}


	return S_OK;
}



