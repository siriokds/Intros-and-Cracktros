#define	 RES_W 640
#define	 RES_H 480

#define LOGOTXWS 360
#define LOGOTXHS 118

#define LOGOWS 192
#define LOGOHS 64

#include <xtl.h>

//#define	 XFONT_TRUETYPE
//#include <xfont.h>

#include "CIniReader.h"

#include "XBApp.h"
#include "XBInput.h"
#include "XBFont.h"
#include "Types.h"

#include "mp3player.h"

#include "logopng.h"
#include "opt_updn.h"

#include "bob.h"

#include "ktexture.h"
#include "kStars.h"


int			resX, resY;

#define		SINEN	4096



typedef struct 
{
	int B1len;
	int B2pos;
	int B2len;
	int SCpos;
	int LGsiz;
	int LGamp;
	int SKpos;

} resInfo;

resInfo	resI[2] = 
{
//	{ 114, 480- 98,  98, 420,  88, 12, 374 },
//	{ 136, 576-118, 118, 504, 104, 15, 450 }
	{ 174, 480-178, 178, 316, LOGOHS, 3, 374 },
	{ 196, 576-178, 178, 504, 104, 15, 450 }
};




typedef struct 
{
	int posX;
	int posY;
	int sizX;
	int sizY;
	int alpha;

} t_screenshot;


t_screenshot	shot = { 334, 118,  260, 194, 200 };





/*----------------------------------------------------------------*/
class CXBoxSample : public CXBApplication
/*----------------------------------------------------------------*/
{
public:
	typedef struct { int posx; char c; } tchar;
	tchar chars[28];
	int	  charsStart;


	WCHAR	*Titles[64];
	WCHAR	*Infos[64];
	int		ntitles;

CIniReader	ir;


	int		 isPAL50;
	resInfo* rI;	
	CXBFont  m_Font2;
	CXBFont  m_Font1;
	int		 font_ok;
//	XFONT*   pFont;  


	GifTexture	*shots_tex[64];
	GifTexture	backtex;

	GifTexture	opt_updn_tex;


	GifTexture	logotex;
	GifTexture	bobtex;

	KStarField	starf;

	
	DWORD	cols[256];
	DWORD	colz[256];

	LPDIRECT3DVERTEXBUFFER8 m_pVB;
	LPDIRECT3DVERTEXBUFFER8 m_tVB[4];
	LPDIRECT3DVERTEXBUFFER8 m_fVB;
	LPDIRECT3DTEXTURE8      m_RGBtexture;

	float	SinTab[SINEN];
	float	CosTab[SINEN];

		    CXBoxSample();
    HRESULT Initialize();
    HRESULT DoIntro();

	HRESULT SetRGBtexture(int start);
	HRESULT RenderState(void);
	HRESULT BandFade(LPDIRECT3DVERTEXBUFFER8 vb, int alpha);
	HRESULT BandColor(LPDIRECT3DVERTEXBUFFER8 vb, 
					   int top, int height, int left, int width,
					   DWORD startColor, DWORD endColor, int alpha);

	HRESULT RasterLine(LPDIRECT3DVERTEXBUFFER8 vb, int top, LPDIRECT3DTEXTURE8 tx, int flp);

	HRESULT DrawQuad(float p_posX, float p_posY, int p_sizeX, int p_sizeY, 
			 		 int t_posX, int t_posY, int t_sizeX, int t_sizeY, 
					 int rotate, int alpha, DWORD col);


//	void TTFDisplayText(void);
	
	HRESULT Render();
    HRESULT FrameMove();

	int posL[10];

};
CXBoxSample xbApp;




#include "TrainerStuffs.h"




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
	m_d3dpp.BackBufferWidth = resX = RES_W;

	resY = 480;
//	resY = (isPAL50==0) ? 480 : 576;
	m_d3dpp.BackBufferHeight = resY;

	m_d3dpp.BackBufferFormat       = D3DFMT_X8R8G8B8;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	m_d3dpp.BackBufferCount        = 1;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;

//	m_d3dpp.MultiSampleType        = D3DMULTISAMPLE_2_SAMPLES_SUPERSAMPLE_HORIZONTAL_LINEAR;
	m_d3dpp.MultiSampleType        = D3DMULTISAMPLE_NONE;

	rI = &resI[0];

//	rI = ((isPAL50==0) ? &resI[0] : &resI[1]);
}



 
//-----------------------------------------------------------------------------
void	cutcols(int *r, int *g, int *b)
//-----------------------------------------------------------------------------
{
	if (*r < 0) *r = 0;		if (*r > 255) *r = 255;
	if (*g < 0) *g = 0;		if (*g > 255) *g = 255;
	if (*b < 0) *b = 0;		if (*b > 255) *b = 255;

}







//-----------------------------------------------------------------------------
HRESULT CXBoxSample::SetRGBtexture(int start)
//-----------------------------------------------------------------------------
{
D3DLOCKED_RECT d3dlr;

	m_RGBtexture->LockRect(0,&d3dlr,0,0);

	DWORD *p = (DWORD*)d3dlr.pBits;
	int	  pitch = d3dlr.Pitch>>2;

	for (int i = 0; i < 256; i++) p[i] = colz[(i+start)&255];
	
	m_RGBtexture->UnlockRect(0);

    return S_OK;
}



//-----------------------------------------------------------------------------
WCHAR * CHAR2WCHAR(char * szIn)
//-----------------------------------------------------------------------------
{
	int len = strlen(szIn);
	WCHAR * szOut = (WCHAR*) malloc ((len + 1) * sizeof(WCHAR));
	memset(szOut,0,sizeof(WCHAR)*(len+1));
	char *p = ((char*)szOut);

	for (int i = 0; i < len; i++) { *p = *szIn++; p += 2; }

	return szOut;
}




//-----------------------------------------------------------------------------
HRESULT CXBoxSample::Initialize()
//-----------------------------------------------------------------------------
{

	charsStart = 0;
	for (int i = 0; i < 28; i++) { chars[i].posx = (i * 24); chars[i].c = ' '; }


	int r = 0xFF, g = 0x00, b = 0x00;
	DWORD *p = cols;

	for (int i = 0; i < 42; i++) {	*p++ = 0xFF000000 + (r<<16) + (g << 8) + (b); g += 6; cutcols(&r,&g,&b); } // R-G
	for (int i = 0; i < 42; i++) {	*p++ = 0xFF000000 + (r<<16) + (g << 8) + (b); r -= 6; cutcols(&r,&g,&b); } // G-V
	for (int i = 0; i < 42; i++) {	*p++ = 0xFF000000 + (r<<16) + (g << 8) + (b); b += 6; cutcols(&r,&g,&b); } // V-C
	for (int i = 0; i < 42; i++) {	*p++ = 0xFF000000 + (r<<16) + (g << 8) + (b); g -= 6; cutcols(&r,&g,&b); } // C-B
	for (int i = 0; i < 42; i++) {	*p++ = 0xFF000000 + (r<<16) + (g << 8) + (b); r += 6; cutcols(&r,&g,&b); } // B-F
	for (int i = 0; i < 42; i++) {	*p++ = 0xFF000000 + (r<<16) + (g << 8) + (b); b -= 6; cutcols(&r,&g,&b); } // F-R
	p[0] = p[-1];	p[1] = p[-1];	p[2] = p[-1];	p[3] = p[-1];


	p = colz;
	float fr = 0, fg = 0, fb = 0;
	for (int i = 0; i < 128; i++) 
	{	
		*p++ = 0xFF000000 + (int(fr)<<16) + (int(fg) << 8) + (int(fb)); 
		fr = fr + 1.9f; 
		fg = fg + 1.7f; 
	}
	for (int i = 0; i < 128; i++) 
	{	
		*p++ = 0xFF000000 + (int(fr)<<16) + (int(fg) << 8) + (int(fb)); 
		fr = fr - 1.9f; 
		fg = fg - 1.7f; 
	}


	m_pd3dDevice->CreateTexture(256,1, 1,0, D3DFMT_LIN_A8R8G8B8, D3DPOOL_MANAGED, &m_RGBtexture);
	SetRGBtexture(0);


    m_pd3dDevice->CreateVertexBuffer( 4*(6*sizeof(FLOAT) + sizeof(DWORD)), D3DUSAGE_WRITEONLY, 0L, D3DPOOL_DEFAULT, &m_pVB );
    m_pd3dDevice->CreateVertexBuffer( 4*5*sizeof(FLOAT), D3DUSAGE_WRITEONLY, 0L, D3DPOOL_DEFAULT, &m_fVB );

    m_pd3dDevice->CreateVertexBuffer( 4*5*sizeof(FLOAT), D3DUSAGE_WRITEONLY, 0L, D3DPOOL_DEFAULT, &m_tVB[0] );
    m_pd3dDevice->CreateVertexBuffer( 4*5*sizeof(FLOAT), D3DUSAGE_WRITEONLY, 0L, D3DPOOL_DEFAULT, &m_tVB[1] );
    m_pd3dDevice->CreateVertexBuffer( 4*5*sizeof(FLOAT), D3DUSAGE_WRITEONLY, 0L, D3DPOOL_DEFAULT, &m_tVB[2] );
    m_pd3dDevice->CreateVertexBuffer( 4*5*sizeof(FLOAT), D3DUSAGE_WRITEONLY, 0L, D3DPOOL_DEFAULT, &m_tVB[3] );


	bobtex.Init(bob,0);

//	logotex.InitPNG("d:\\data\\media\\logo.png");
	logotex.InitPNG((char*)logopng, 43134);

	opt_updn_tex.InitPNG((char*)opt_updn, 3943);


	backtex.InitPNG("d:\\data\\media\\screen.png");

	starf.Initialize(resX,resY, SinTab, CosTab);

	double incr = 6.283185307179586476925286766559 / double(SINEN);
	double val = 0;

	for (int i = 0; i < SINEN; i++)
	{
		SinTab[i] = (float)sin(val);
		CosTab[i] = (float)cos(val);

		val += incr;
	}

	SinTab[0] = 0;
	CosTab[0] = 1;


/*
	FCp_Open(isPAL50);
	FCp_InitModule((char*)Module);
	FCp_Start();

*/



	ir.ReadIni( "d:\\data\\config.txt" );
	ntitles = ir.m_ItemsNum;

	if (ir.m_posX != -1) shot.posX = ir.m_posX;
	if (ir.m_posY != -1) shot.posY = ir.m_posY;
	if (ir.m_sizX != -1) shot.sizX = ir.m_sizX;
	if (ir.m_sizY != -1) shot.sizY = ir.m_sizY;
	if (ir.m_alpha != -1) shot.alpha = ir.m_alpha;

	if (ir.m_options_posX != -1) OptionsOffset_X = ir.m_options_posX;
	if (ir.m_options_posY != -1) OptionsOffset_Y = ir.m_options_posY;
	if (ir.m_options_sizX != -1) OptionsRowWidth = ir.m_options_sizX;
	if (ir.m_options_sizY != -1) OptionsRowHeight = ir.m_options_sizY;
	if (ir.m_options_rows != -1) OptionsVisibleItems = ir.m_options_rows;

	if ( ntitles > 0 )
	{
	  for (int i = 0; i < ntitles; i++)
		{
			Titles[i] = CHAR2WCHAR( ir.m_Items[i].Title );
			Infos[i]  = CHAR2WCHAR( ir.m_Items[i].Info  );
		}
	}

	OptionsInit();



	font_ok = 1;
	if ( FAILED( m_Font2.Create( "d:\\data\\media\\fontb.xpr" ) ) ) font_ok = 0;
	if ( FAILED( m_Font1.Create( "d:\\data\\media\\fontn.xpr" ) ) ) font_ok = 0;

//	XFONT_OpenTrueTypeFont(L"D:\\gill.ttf", 16384, &pFont );



	char temp[128];
	for (int i = 0; i < ntitles; i++)
	{
		shots_tex[i] = new GifTexture();
		if (shots_tex[i])
		{
			strcpy(temp, "d:\\data\\shots\\");
			strcat(temp, ir.m_Items[i].Shot);
			shots_tex[i]->InitPNG(temp);
		}
	}


	mp3_init("d:\\data\\media\\music.mp3", isPAL50);
	mp3_start();
	mp3_setmastervol(80);

    return S_OK;
}




/*
void CXBoxSample::TTFDisplayText(void)
{
LPDIRECT3DSURFACE8  pFrontBuffer;
      m_pd3dDevice->GetBackBuffer(-1,D3DBACKBUFFER_TYPE_MONO,&pFrontBuffer);


WCHAR szbuff[200] = {0};

      swprintf(szbuff, L"Hello World");

      //Top left corner of where we want to draw our text.
      float xpos = 100.0f;
      float ypos = 100.0f;
 

      //Display our text.

//      pFont->SetTextHeight(24);
//      pFont->SetTextColor(D3DCOLOR_XRGB(30,255,20));

      pFont->SetTextColor(D3DCOLOR_XRGB(30,255,20));
      pFont->TextOut( pFrontBuffer, szbuff, -1, (long)xpos, (long)ypos );

      pFrontBuffer->Release();

}// End DisplayText()

*/

//-----------------------------------------------------------------------------
HRESULT CXBoxSample::FrameMove()
//-----------------------------------------------------------------------------
{ 
    return S_OK;
}



//------------------------------------------------------------------------------------
HRESULT CXBoxSample::BandColor(LPDIRECT3DVERTEXBUFFER8 vb, 
							   int top, int height, int left, int width,
							   DWORD startColor, DWORD endColor, int alpha)
//------------------------------------------------------------------------------------
{
	float lf = float( left );   float rt = float( left + width );
	float tp = float( top  );   float bt = float( top + height );
    

	alpha = (alpha < 0) ? 0 : ((alpha > 255) ? 255 : alpha);
	alpha <<= 24;

	startColor = (startColor & 0x00FFFFFF) | alpha;
	endColor   = (endColor   & 0x00FFFFFF) | alpha;
	
	struct BACKGROUNDVERTEX { D3DXVECTOR4 p; D3DCOLOR color; };
    BACKGROUNDVERTEX* v;
    
    vb->Lock( 0, 0, (BYTE**)&v, 0L );

		v[0].p = D3DXVECTOR4(  lf - 0.5f,  tp - 0.5f, 1.0f, 1.0f );  v[0].color = startColor;
		v[1].p = D3DXVECTOR4(  rt - 0.5f,  tp - 0.5f, 1.0f, 1.0f );  v[1].color = startColor;
		v[2].p = D3DXVECTOR4(  lf - 0.5f,  bt - 0.5f, 1.0f, 1.0f );  v[2].color = endColor;
		v[3].p = D3DXVECTOR4(  rt - 0.5f,  bt - 0.5f, 1.0f, 1.0f );  v[3].color = endColor;

    vb->Unlock();



    // Set states
    m_pd3dDevice->SetTexture( 0, NULL );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE ); 

	if (alpha == 255)
	{
		m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE ); 
	}
	else
	{
		m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	}

    m_pd3dDevice->SetVertexShader( D3DFVF_XYZRHW|D3DFVF_DIFFUSE );
    m_pd3dDevice->SetStreamSource( 0, vb, 5*sizeof(FLOAT) );
    
    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );


    return S_OK;
}




//------------------------------------------------------------------------------------------
HRESULT CXBoxSample::RasterLine(LPDIRECT3DVERTEXBUFFER8 vb, int top, 
								LPDIRECT3DTEXTURE8 tx, int Hflipped)
//------------------------------------------------------------------------------------------
{
    m_pd3dDevice->SetTexture( 0, tx );


struct VERTEX { D3DXVECTOR4 p; D3DCOLOR vbColor; FLOAT tu, tv; };
VERTEX* v;

float pX[4], pY[4];
float tSX, tEX;


	if (!Hflipped)  { tSX = 0; tEX = 256 - 0.5f; }
	else			{ tSX = 256 - 0.5f; tEX = 0; }


	pX[0] = 0.0f;			pY[0] = (float)top;
	pX[1] = (float)RES_W;	pY[1] = (float)top;
	pX[2] = (float)RES_W;	pY[2] = (float)top+1;
	pX[3] = 0.0f;			pY[3] = (float)top+1;



    m_pVB->Lock( 0, 0, (BYTE**)&v, 0L );

		v[0].p = D3DXVECTOR4(pX[0], pY[0],  0, 0 );  
		v[0].vbColor = 0xFFFFFFFF; v[0].tu = tSX; v[0].tv = 0;

		v[1].p = D3DXVECTOR4(pX[1], pY[1],  0, 0 );  
		v[1].vbColor = 0xFFFFFFFF; v[1].tu = tEX; v[1].tv = 0;
	    
		v[2].p = D3DXVECTOR4(pX[2], pY[2],  0, 0 );  
		v[2].vbColor = 0xFFFFFFFF; v[2].tu = tEX; v[2].tv = 1.5f;
	    
		v[3].p = D3DXVECTOR4(pX[3], pY[3],  0, 0 );  
		v[3].vbColor = 0xFFFFFFFF; v[3].tu = tSX; v[3].tv = 1.5f;
    
	m_pVB->Unlock();


	m_pd3dDevice->SetStreamSource( 0, m_pVB, 6*sizeof(FLOAT) + sizeof(DWORD) );
	m_pd3dDevice->DrawPrimitive( D3DPT_QUADLIST, 0, 1 );


    return S_OK;
}





//------------------------------------------------------------------------------------
HRESULT CXBoxSample::BandFade(LPDIRECT3DVERTEXBUFFER8 vb, int alpha)
//------------------------------------------------------------------------------------
{
	float lf = 0, rt = 640;
	float tp = 0, bt = 576;

	if (alpha > 255) alpha = 255;
    alpha = (alpha & 255) << 24;

	struct BACKGROUNDVERTEX { D3DXVECTOR4 p; D3DCOLOR color; };
    BACKGROUNDVERTEX* v;
    
    vb->Lock( 0, 0, (BYTE**)&v, 0L );

		v[0].p = D3DXVECTOR4(  lf - 0.5f,  tp - 0.5f, 1.0f, 1.0f );  v[0].color = alpha;
		v[1].p = D3DXVECTOR4(  rt - 0.5f,  tp - 0.5f, 1.0f, 1.0f );  v[1].color = alpha;
		v[2].p = D3DXVECTOR4(  lf - 0.5f,  bt - 0.5f, 1.0f, 1.0f );  v[2].color = alpha;
		v[3].p = D3DXVECTOR4(  rt - 0.5f,  bt - 0.5f, 1.0f, 1.0f );  v[3].color = alpha;

    vb->Unlock();

    // Set states
    m_pd3dDevice->SetTexture( 0, NULL );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE ); 

	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    m_pd3dDevice->SetVertexShader( D3DFVF_XYZRHW|D3DFVF_DIFFUSE );
    m_pd3dDevice->SetStreamSource( 0, vb, 5*sizeof(FLOAT) );

    
    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

    return S_OK;
}





//-------------------------------------------------------------------------------------------
HRESULT CXBoxSample::RenderState(void) 
//-------------------------------------------------------------------------------------------
{
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

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,	  FALSE);
	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,      FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,    FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_NONE );
	m_pd3dDevice->SetRenderState( D3DRS_FILLMODE,     D3DFILL_SOLID );
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,     D3DCULL_CCW );

	
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	m_pd3dDevice->SetVertexShader( D3DFVF_XYZRHW|D3DFVF_TEX1|D3DFVF_DIFFUSE  );


	return S_OK;
}







//-------------------------------------------------------------------------------------------
HRESULT CXBoxSample::DrawQuad(float p_posX, float p_posY, int p_sizeX, int p_sizeY, 
							  int t_posX, int t_posY, int t_sizeX, int t_sizeY, 
							  int rotate, int alpha,  DWORD col)
//-------------------------------------------------------------------------------------------
{ 
struct VERTEX { D3DXVECTOR4 p; D3DCOLOR vbColor; FLOAT tu, tv; };
VERTEX* v;

float hX, hY,  sX, sY,  eX, eY;
float pX[4], pY[4];
float rotSin, rotCos;


		hX = float(p_sizeX) * 0.5f;
		hY = float(p_sizeY) * 0.5f;

		sX =                - hX;	sY =                - hY;
		eX = float(p_sizeX) - hX;	eY = float(p_sizeY) - hY;


		pX[0] = sX; pY[0] = sY;
		pX[1] = eX; pY[1] = sY;
		pX[2] = eX; pY[2] = eY;
		pX[3] = sX; pY[3] = eY;


		while (rotate < 0) rotate += SINEN;
		rotate %= SINEN;
		rotSin = SinTab[ rotate ];
		rotCos = CosTab[ rotate ];


		for (int i = 0; i < 4; i++)
		{
		    float tX = (pX[i] * rotCos) - (pY[i] * rotSin);
			float tY = (pX[i] * rotSin) + (pY[i] * rotCos);
			pX[i] = tX + float(p_posX) + hX + 0.25f; 
			pY[i] = tY + float(p_posY) + hY + 0.25f;
		}


		col = ((alpha&0xFF) << 24) | (col&0xFFFFFF);


    m_pVB->Lock( 0, 0, (BYTE**)&v, 0L );

		v[0].p = D3DXVECTOR4(pX[0], pY[0],  0, 0 );  
		v[0].vbColor = col; 
		v[0].tu = float(t_posX);		
		v[0].tv = float(t_posY);

		v[1].p = D3DXVECTOR4(pX[1], pY[1],  0, 0 );  
		v[1].vbColor = col;
		v[1].tu = float(t_posX + t_sizeX) - 0.5f;
		v[1].tv = float(t_posY);
	    
		v[2].p = D3DXVECTOR4(pX[2], pY[2],  0, 0 );  
		v[2].vbColor = col;
		v[2].tu = float(t_posX + t_sizeX) - 0.5f;
		v[2].tv = float(t_posY + t_sizeY) - 0.5f;
	    
		v[3].p = D3DXVECTOR4(pX[3], pY[3],  0, 0 );  
		v[3].vbColor = col;
		v[3].tu = float(t_posX);
		v[3].tv = float(t_posY + t_sizeY) - 0.5f;
    
	m_pVB->Unlock();



	m_pd3dDevice->SetStreamSource( 0, m_pVB, 6*sizeof(FLOAT) + sizeof(DWORD) );
	m_pd3dDevice->DrawPrimitive( D3DPT_QUADLIST, 0, 1 );


	return S_OK;
}




#define LOGO_L	224
#define LOGO_T	128

typedef struct { float l, t, w, h; } lstat;

lstat	stats[16] = 
{
	{ LOGO_L, LOGO_T, LOGOWS, LOGOHS }, { LOGO_L, LOGO_T, LOGOWS, LOGOHS },
	{ LOGO_L, LOGO_T, LOGOWS, LOGOHS }, { LOGO_L, LOGO_T, LOGOWS, LOGOHS },
	{ LOGO_L, LOGO_T, LOGOWS, LOGOHS },	{ LOGO_L, LOGO_T, LOGOWS, LOGOHS },
	{ LOGO_L, LOGO_T, LOGOWS, LOGOHS },	{ LOGO_L, LOGO_T, LOGOWS, LOGOHS },
	{ LOGO_L, LOGO_T, LOGOWS, LOGOHS },	{ LOGO_L, LOGO_T, LOGOWS, LOGOHS },
	{ LOGO_L, LOGO_T, LOGOWS, LOGOHS },	{ LOGO_L, LOGO_T, LOGOWS, LOGOHS },
	{ LOGO_L, LOGO_T, LOGOWS, LOGOHS },	{ LOGO_L, LOGO_T, LOGOWS, LOGOHS },
	{ LOGO_L, LOGO_T, LOGOWS, LOGOHS },	{ LOGO_L, LOGO_T, LOGOWS, LOGOHS }
};

int	statP = 0;




//------------------------------------------------------------------------------
HRESULT CXBoxSample::DoIntro()
//------------------------------------------------------------------------------
{
	RenderState();


	backtex.SetTexture();
	DrawQuad(0, 0, 640, 480,  0,0,640,480,  0, 255, 0xFFFFFF);





// BOBS

	RenderState();

static int bobN = 0;

	starf.Update();
	bobtex.SetTexture();

	for (int i = 0; i < starf.m_NumStars; i++)
	{
		int size = int((255 - starf.m_Stars[i].m_Z) / 5) >> starf.m_Stars[i].m_Size;
		int salpha = int((255 - starf.m_Stars[i].m_Z) * 0.75f);

		salpha = (salpha * 75) / 100;

		if (starf.m_Stars[i].m_Z > 4)
			DrawQuad(starf.m_x[i],starf.m_y[i], size,size,  64,0,64,64,  0, salpha, 0xFFFFFF);
//			DrawQuad(starf.m_x[i],starf.m_y[i], size,size,  ((starf.m_Stars[i].m_Color+1)&3)*64,0,64,64,  0, salpha, 0xFFFFFF);
	}

	if (++bobN > 700)
	{
		bobN = 0;
		starf.m_color = (starf.m_color + 2) & 3;
	}




// OPTIONS

	RenderState();
	OptionsWriteShot( shot.posX, shot.posY, shot.sizX, shot.sizY, shot.alpha );

	RenderState();
	OptionsWriteCursorBand();
	RenderState();
	OptionsWriteArrows();
	DrawOptions();



// LOGO

	RenderState();

static  int	 wAmp = 0;
		int  wAmpI = 4;
		int  wampl = 16 + int(SinTab[wAmp] * 18);


		wAmp += wAmpI;
		wAmp &= 4095;

static float logoWSinePos = 0.0f;
static float logoWSineInc = 32.0f;
static float logoHSinePos = 1024.0f;
static float logoHSineInc = 12.0f;

	int width  = LOGOWS + ( SinTab[ ((int)logoWSinePos)&4095 ] * (7+wampl) );
	int height = LOGOHS + ( SinTab[ ((int)logoHSinePos)&4095 ] * 8  );
	logoWSinePos += logoWSineInc;
	logoHSinePos += logoHSineInc;
	int left = LOGO_L - ((width  - LOGOWS)>>1);
	int top  = LOGO_T - ((height - rI->LGsiz)>>1);

	stats[statP].w = width;
	stats[statP].h = height;
	stats[statP].l = left;
	stats[statP].t = top;


	int alphas[] = { 128, 54 };

	logotex.SetTexture();

	for (int i = 0; i < 2; i++)
	{
		lstat *s = &stats[((statP-1) + (i*9)) & 15];
		DrawQuad(s->l, 250 + s->t, s->w, s->h,  0,0,LOGOTXWS,LOGOTXHS,  0, alphas[i], 0xFFFFFF);
	}

	statP = (statP + 1) & 15;




//	TTFDisplayText();



static int masterAlpha = 255;


//--------------------------

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


	if (TrainerFade == 1)
	{
		if (masterAlpha < 255) 
		{
			BandFade(m_fVB, masterAlpha);
			masterAlpha += 4;
			mp3_setmastervolperc(255 - masterAlpha);

//			mp3_setmastervol(255 - masterAlpha);

		}
		else TrainerLaunch();
		
	}
	else 
	{
		if (TrainerFade == 2)
		{
			if (masterAlpha > 0) 
			{
				BandFade(m_fVB, masterAlpha);
				masterAlpha -= 32;
				if (masterAlpha <= 0) 
				{
					masterAlpha = 0;
					TrainerFade = 0;
				}
//				mp3_setmastervolperc(255 - masterAlpha);
			}
			
		}

		if (m_DefaultGamepad.wButtons & XINPUT_GAMEPAD_START)	
		{
			if ( (startPressed & 1) == 0)
			{
				startPressed = 1;
				TrainerStart();
			}
		}
		else { if ((startPressed & 1) == 1) startPressed = 2; }
	}


	return S_OK;
}




int	raster_wait = 25;

//------------------------------------------------------------------------------
HRESULT CXBoxSample::Render()
//------------------------------------------------------------------------------
{
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,  0, 0), 1.0f, 0);

	if (raster_wait > 0) 
	{ 
		raster_wait--;
	}
	else
	{

//		FCp_Run();
		DoIntro();
	}

	mp3_run();
	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);

	return S_OK;
}



