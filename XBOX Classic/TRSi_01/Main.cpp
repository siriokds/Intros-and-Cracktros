//Main header file for the XDK

#define	 SCRWIDTH 640

#include <xtl.h>
#include <XBApp.h>
#include <XBInput.h>
#include "Types.h"
#include "timing.h"
#include "FC_Lib.h"
#include "m_thalion.h"

float		scaleval = 1.0f;

#include "TriFiller.h"
#include "Gfx.h"
#include "scroll.h"
#include "VLetters.h"
#include "Fntz.h"


int			resX, resY;

int			stretchX = 1;
int			stretchY = 0;
ticker_t	timer;

float		fps  = 0;
float		lastfps[8];
float		last;


#define	 TXTCOLS	52
#define	 TXTROWS	20
#define	 CENTERX	((640-(TXTCOLS*8))/2)
#define	 CENTERY	((480-(TXTROWS*12))/2)


UBYTE	fill____01[65536];

DWORD	palette[256];

UBYTE	fill____02[65536];

UBYTE	Frame[640*480*2];

UBYTE	fill____03[65536];

UBYTE	FramT[(TXTCOLS*8)*(TXTROWS*12)];

UBYTE	fill____04[65536];

int		TriSetting[13] = 
{
	48, 640-48, 36,480-36,
	(int)(Frame), 640, 3,
	0,0, 400,0, 400,400
};


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

char	fntMtx[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-()><:.,!?+_/@";

struct	scrInfo		scroll_Info;

char	page1[] = 
// 0         1         2         3         4
// 0123456789012345678901234567890123456789012345678901
// -------------------------||-------------------------
  "                                                    "
  "                                                    "
  "                                                    "
  "                                                    "
  "        TRISTAR AND RED SECTOR INC. PRESENTS:       "
  "        -------------------------------------       "
  "                                                    "
  "                 XMEN-2 +4 TRAINER                  "
  "                 -----------------                  "
  "                                                    "
  "                                                    "
  "          TRAINED BY  > SETH GECKO / TRSI <         "
  "                                                    "
  "            GAME CRACKED BY  > FAIRLIGHT <          "
  "                                                    "
  "        -------------------------------------       "
  "                                                    "
  "                                                    "
  "                                                    "
  "                                                   w";



char	page2[] = 
// 0         1         2         3         4
// 0123456789012345678901234567890123456789012345678901
// -------------------------||-------------------------
  "       ---------------------------------------      "
  "                                                    "
  "            OUR GOLDEN REGARDS GO OUT TO:           "
  "                                                    "
  "       ---------------------------------------      "
  "                                                    "
  "                                                    "
  "          FAIRLIGHT - QUARTEX - SECTION 8           "
  "                                                    "
  "         DANISH GOLD - KALISTO - RAZOR 1911         "
  "                                                    "
  "           IMMERSION - LOCKLESS - DEVIANCE          "
  "                                                    "
  "            MYTH - SCOOPEX - THE SILENTS            "
  "                                                    "
  "          GENESIS - SOUNDDRINKER - PARADOX          "
  "                                                    "
  "               HAUJOBB - FARBRAUSCH                 "
  "                                                    "
  "       ---------------------------------------     w";

char	page3[] = 
// 0         1         2         3         4
// 0123456789012345678901234567890123456789012345678901
// -------------------------||-------------------------
  "                                                    "
  "                                                    "
  "                  INTRO CREDITS:                    "
  "    --------------------------------------------    "
  "                                                    "
  "    ORIGINAL AMIGA INTRO .......... FROG/PARADOX    "
  "                                                    "
  "    INTRO CODING BY ............... SIRIOKD/TRSI    "
  "                                                    "
  "    2D FONT BY .................... ADEC/TRSI       "
  "                                                    "
  "    3D FONT BY .................... SIRIOKD/TRSI    "
  "                                                    "
  "    MUZAK REPLAYER BY ............. SIRIOKD/TRSI    "
  "                                                    "
  "    MUZAK BY ...................... MONTY           "
  "                                                    "
  "    --------------------------------------------    "
  "                                                    "
  "                                                   w";


char	page4[] = 
// 0         1         2         3         4
// 0123456789012345678901234567890123456789012345678901
// -------------------------||-------------------------
  "                                                    "
  "----------------------------------------------------"
  "                                                    "
  "                                                    "
  "     @@@@@@@@@@@ @@@@@@@@@   @@@@@@@@@  @@@@@       "
  "        @@@@@    @@@@@  @@@ @@@@@  @@@              "
  "        @@@@@    @@@@@@@@@  @@@@@@      @@@@@       "
  "        @@@@@    @@@@@@@@@@   @@@@@@@   @@@@@@      "
  "        @@@@@    @@@@@  @@@       @@@@@ @@@@@@      "
  "        @@@@@    @@@@@  @@@ @@@@@  @@@@ @@@@@@      "
  "        @@@@@    @@@@@  @@@ @@@@@@@@@@@ @@@@@@      "
  "        @@@@@    @@@@@  @@@ @@@@@@@@@@  @@@@@@      "
  "                                                    "
  "                                                    "
  "          VISIT OUR WEBSITE AT: WWW.TRSI.ORG        "
  "                                                    "
  "----------------------------------------------------"
  "                                                    "
  "                                                    "
  "                                                   w";

char	*pages[] = { page1, page2, page3, page4, 0 };


/*----------------------------------------------------------*/
void	DrawSirioKD(void)
/*----------------------------------------------------------*/
{
static int rota2 = 0;

	int posX = 570;
	
	UBYTE *src = imx + 778 + 400 + 16;

	static float vY = 0.0f;

	UBYTE *dst = Frame+(640*418)+posX-14 - (640 * int(sin(vY)*30) );
	vY += (3.14159265358979323846264338328f / 95) * scaleval;
	if (vY > 3.14159265358979323846264338328f) vY -= 3.14159265358979323846264338328f;


	for (int i = 0; i < 10; i++)
	{
		int offY = (i*640*2);

		for (int j = 0; j < 32; j++)
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

	for (int i = 0; i < (TXTROWS*12); i++)
	{
		int offSrc = i * (TXTCOLS*8);

		int offDst = ((i+CENTERY)*640) + CENTERX;

		for (int j = 0; j < (TXTCOLS*8); j++)
		{
			if (FramT[offSrc+j]) 
			{
//				Frame[offDst+j+(1924)] |= 0x10;
				Frame[offDst+j] = FramT[offSrc+j];
			}
		}
	}
			
			
	vfont_run(TriSetting);
	DrawSirioKD();

	scrExecute(&scroll_Info);

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
	m_pd3dDevice->CreateTexture( 640, 480, 1, 0, D3DFMT_LIN_X8R8G8B8, D3DPOOL_MANAGED, &m_pVideoTexture);
    m_pd3dDevice->CreateVertexBuffer( 4*(6*sizeof(FLOAT) + sizeof(DWORD)), D3DUSAGE_WRITEONLY, 0L, D3DPOOL_DEFAULT, &m_pVB );

	last = (float)GetTickCount();


	timer.Init();
	Math3DInit();
	vfont_init();

	scroll_Info.scrFont = (char*)imx+778;
	scroll_Info.scrFontRow = 512;
	scroll_Info.scrDest = (char*)FramT;
	scroll_Info.scrDestRow = TXTCOLS*8;

	scroll_Info.scrPages = pages;
	scroll_Info.scrFontMtx = fntMtx;
	scroll_Info.scrFontW = 8;
	scroll_Info.scrFontH = 10;
	scroll_Info.col = TXTCOLS;

	scrInit(&scroll_Info);


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

	tdxSetPal(myPal);



	FCp_Open(isPAL50);
	FCp_InitModule((char*)fcModule);
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

	fps  = (float)(1.0f/(((float)GetTickCount() - last) / 1000.0f));
    last = (float)GetTickCount();



//-----------------------------------

	scaleval = timer.Passed() * 0.1f;
	if (scaleval < 0) scaleval = 1.0f;

	UpdateFrame();

//-----------------------------------


D3DLOCKED_RECT d3dlr;

	m_pVideoTexture->LockRect(0,&d3dlr,0,0);

	UBYTE	*src = Frame;
	DWORD	*dst = (DWORD*)d3dlr.pBits;
	int		pitch = d3dlr.Pitch>>2;

	if (dst) 
	{
		for (int y = 0; y < 480; y++)
		{
			for (int x = 0; x < 640; x++) *dst++ = palette[*src++];
			dst += pitch - 640;
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

	float sizeX = 640.0f;
	float offsX = (SCRWIDTH-640)/2;

	float sizeY = 576.0f;
	float offsY = 0;

	DWORD mColor = 0xFFFFFFFF;

    m_pVB->Lock( 0, 0, (BYTE**)&v, 0L );
    v[0].p = D3DXVECTOR4(offsX+    0,offsY+    0, 0, 0 );  v[0].vbColor = mColor; v[0].tu =      0;  v[0].tv =      0;
    v[1].p = D3DXVECTOR4(offsX+sizeX,offsY+    0, 0, 0 );  v[1].vbColor = mColor; v[1].tu = 639.5f;  v[1].tv =      0;
    v[2].p = D3DXVECTOR4(offsX+sizeX,offsY+sizeY, 0, 0 );  v[2].vbColor = mColor; v[2].tu = 639.5f;  v[2].tv = 575.5f;
    v[3].p = D3DXVECTOR4(offsX+    0,offsY+sizeY, 0, 0 );  v[3].vbColor = mColor; v[3].tu =      0;  v[3].tv = 575.5f;
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



