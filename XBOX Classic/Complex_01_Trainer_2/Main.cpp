#define	 RES_W 640
#define	 RES_H 480


#include <xtl.h>
#include <XBApp.h>
#include <XBInput.h>
#include "RemapDrives.h"
#include "Types.h"

#include "TrainerData.h"


#include "font.h"
#include "tfont.h"
#include "rfont.h"
#include "logo.h"
#include "bob.h"
#include "skd.h"

#include "ktexture.h"
#include "kStars.h"

#include "FC_Lib.h"
//#include "frantic.h"
#include "try12.h"


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
	{ 114, 480- 98,  98, 420,  88, 12, 374 },
	{ 136, 576-118, 118, 504, 104, 15, 450 }
};





/*----------------------------------------------------------------*/
class CXBoxSample : public CXBApplication
/*----------------------------------------------------------------*/
{
public:
	typedef struct { int posx; char c; } tchar;
	tchar chars[28];
	int	  charsStart;

	int		 isPAL50;
	resInfo* rI;	

	GifTexture	logotex;
	GifTexture	gtex;
	GifTexture	bobtex;
	GifTexture	tftex;
	GifTexture	rftex;
	GifTexture	skdtex;

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

	HRESULT rFont_DrawLetter(int x, int y, char c, int rotate, int alpha);
	HRESULT tFont_DrawLetter(int x, int y, char c, int rot, int col, int alpha);
	HRESULT tFont_DrawString(int x, int y, char *s, int col, int alpha);

	HRESULT SetRGBtexture(int start);
	HRESULT RenderState(void);
	HRESULT BandFade(LPDIRECT3DVERTEXBUFFER8 vb, int alpha);
	HRESULT BandColor(LPDIRECT3DVERTEXBUFFER8 vb, 
					   int top, int height, int left, int width,
					   DWORD startColor, DWORD endColor);

	HRESULT RasterLine(LPDIRECT3DVERTEXBUFFER8 vb, int top, LPDIRECT3DTEXTURE8 tx, int flp);

	HRESULT DrawQuad(float p_posX, float p_posY, int p_sizeX, int p_sizeY, 
			 		 int t_posX, int t_posY, int t_sizeX, int t_sizeY, 
					 int rotate, int alpha, DWORD col);

    
	
	HRESULT Render();           // Render the scene
    HRESULT FrameMove();        // Perform per-frame updates


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

	resY = (isPAL50==0) ? 480 : 576;
	m_d3dpp.BackBufferHeight = resY;

	m_d3dpp.BackBufferFormat       = D3DFMT_X8R8G8B8;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	m_d3dpp.BackBufferCount        = 1;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;

//	m_d3dpp.MultiSampleType        = D3DMULTISAMPLE_2_SAMPLES_SUPERSAMPLE_HORIZONTAL_LINEAR;
	m_d3dpp.MultiSampleType        = D3DMULTISAMPLE_NONE;


	rI = ((isPAL50==0) ? &resI[0] : &resI[1]);
}



 
//-----------------------------------------------------------------------------
void	cutcols(int *r, int *g, int *b)
//-----------------------------------------------------------------------------
{
	if (*r < 0) *r = 0;
	if (*g < 0) *g = 0;
	if (*b < 0) *b = 0;

	if (*r > 255) *r = 255;
	if (*g > 255) *g = 255;
	if (*b > 255) *b = 255;
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
HRESULT CXBoxSample::Initialize()
//-----------------------------------------------------------------------------
{
	OptionsInit();

	charsStart = 0;
	for (int i = 0; i < 28; i++) 
	{
		chars[i].posx = (i * 24);
		chars[i].c = ' ';
	}


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

	gtex.Init(font,0);
	tftex.Init(tfont,0);
	rftex.Init(rfont,0);
	bobtex.Init(bob,0);
	skdtex.Init(skd,0);
	logotex.Init(logo,0);


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



//------------------------------------------------------------------------------------
HRESULT CXBoxSample::BandColor(LPDIRECT3DVERTEXBUFFER8 vb, 
							   int top, int height, int left, int width,
							   DWORD startColor, DWORD endColor)
//------------------------------------------------------------------------------------
{
	float lf = float(left);
	float rt = float(left+width);
	float tp = float(top);
	float bt = float(top+height);
    
	
	struct BACKGROUNDVERTEX { D3DXVECTOR4 p; D3DCOLOR color; };
    BACKGROUNDVERTEX* v;
    
    vb->Lock( 0, 0, (BYTE**)&v, 0L );
    v[0].p = D3DXVECTOR4(  lf - 0.5f,  tp - 0.5f, 1.0f, 1.0f );  v[0].color = startColor;
    v[1].p = D3DXVECTOR4(  rt - 0.5f,  tp - 0.5f, 1.0f, 1.0f );  v[1].color = endColor;
    v[2].p = D3DXVECTOR4(  lf - 0.5f,  bt - 0.5f, 1.0f, 1.0f );  v[2].color = startColor;
    v[3].p = D3DXVECTOR4(  rt - 0.5f,  bt - 0.5f, 1.0f, 1.0f );  v[3].color = endColor;
    vb->Unlock();

    // Set states
    m_pd3dDevice->SetTexture( 0, NULL );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE ); 
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE ); 


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


	if (!Hflipped)
	{
		tSX = 0;
		tEX = 256 - 0.5f;
	}
	else
	{
		tSX = 256 - 0.5f;
		tEX = 0;
	}

	pX[0] = 0;		pY[0] = top;
	pX[1] = RES_W;	pY[1] = top;
	pX[2] = RES_W;	pY[2] = top+1;
	pX[3] = 0;		pY[3] = top+1;



    m_pVB->Lock( 0, 0, (BYTE**)&v, 0L );

    v[0].p = D3DXVECTOR4(pX[0], pY[0],  0, 0 );  
	v[0].vbColor = 0xFFFFFFFF; 
	v[0].tu = tSX;
	v[0].tv = 0;

    v[1].p = D3DXVECTOR4(pX[1], pY[1],  0, 0 );  
	v[1].vbColor = 0xFFFFFFFF;
	v[1].tu = tEX;
	v[1].tv = 0;
    
	v[2].p = D3DXVECTOR4(pX[2], pY[2],  0, 0 );  
	v[2].vbColor = 0xFFFFFFFF;
	v[2].tu = tEX;
	v[2].tv = 1.5f;
    
	v[3].p = D3DXVECTOR4(pX[3], pY[3],  0, 0 );  
	v[3].vbColor = 0xFFFFFFFF;
	v[3].tu = tSX;
	v[3].tv = 1.5f;
    
	m_pVB->Unlock();


	m_pd3dDevice->SetStreamSource( 0, m_pVB, 6*sizeof(FLOAT) + sizeof(DWORD) );
	m_pd3dDevice->DrawPrimitive( D3DPT_QUADLIST, 0, 1 );


    return S_OK;
}





//------------------------------------------------------------------------------------
HRESULT CXBoxSample::BandFade(LPDIRECT3DVERTEXBUFFER8 vb, int alpha)
//------------------------------------------------------------------------------------
{
	float lf = 0;
	float rt = 640;
	float tp = 0;
	float bt = 576;
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
HRESULT CXBoxSample::rFont_DrawLetter(int x, int y, char c, int rotate, int alpha)
//-------------------------------------------------------------------------------------------
{ 
char rmtx[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-";


	if ((c != ' ') && (c != 0))
	{
		char *s = rmtx;
		int ofs = 0;

		while (1)
		{
			if (*s == 0) break;
			if (*s == c)
			{
				rftex.SetTexture();
				DrawQuad(x, y, 8, 24,  ofs*16,0, 16,16,  rotate, alpha, 0xFFFFFF);
				break;
			}

			s++; ofs++;
		}

	}

	return S_OK;
}


//-------------------------------------------------------------------------------------------
HRESULT CXBoxSample::tFont_DrawLetter(int x, int y, char c, int rotate, int col, int alpha)
//-------------------------------------------------------------------------------------------
{ 
char tmtx[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890/?!@.,()-\"':><+";


	if ((c != ' ') && (c != 0))
	{
		char *s = tmtx;
		int ofs = 0;

		while (1)
		{
			if (*s == 0) break;
			if (*s == c)
			{
				tftex.SetTexture();
				DrawQuad(x, y, 8*1, 12*2,  ofs*9,0, 8,12,  rotate, alpha, col);
				break;
			}

			s++; ofs++;
		}

	}

	return S_OK;
}



//-------------------------------------------------------------------------------------------
HRESULT CXBoxSample::tFont_DrawString(int x, int y, char *s, int col, int alpha)
//-------------------------------------------------------------------------------------------
{ 
	while (*s) 
	{
		tFont_DrawLetter(x, y, *s, 0, col, alpha);
		x += 8;
		s++;
	}


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






typedef struct { float l, t, w, h; } lstat;

lstat	stats[16] = 
{
	{ 96, 16, 448, 88 },
	{ 96, 16, 448, 88 },
	{ 96, 16, 448, 88 },
	{ 96, 16, 448, 88 },
	{ 96, 16, 448, 88 },
	{ 96, 16, 448, 88 },
	{ 96, 16, 448, 88 },
	{ 96, 16, 448, 88 },
	{ 96, 16, 448, 88 },
	{ 96, 16, 448, 88 },
	{ 96, 16, 448, 88 },
	{ 96, 16, 448, 88 },
	{ 96, 16, 448, 88 },
	{ 96, 16, 448, 88 },
	{ 96, 16, 448, 88 },
	{ 96, 16, 448, 88 }
};

int	statP = 0;




//------------------------------------------------------------------------------
HRESULT CXBoxSample::DoIntro()
//------------------------------------------------------------------------------
{
	RenderState();


// BOBS

static int bobN = 0;

	starf.Update();
	bobtex.SetTexture();

	for (int i = 0; i < starf.m_NumStars; i++)
	{
		int size = int((255 - starf.m_Stars[i].m_Z) / 5) >> starf.m_Stars[i].m_Size;
		int salpha = int((255 - starf.m_Stars[i].m_Z) * 0.85f);

		if (starf.m_Stars[i].m_Z > 4)
			DrawQuad(starf.m_x[i],starf.m_y[i], size,size,  starf.m_Stars[i].m_Color*64,0,64,64,  0, salpha, 0xFFFFFF);
	}

	if (++bobN > 700)
	{
		bobN = 0;
		starf.m_color = (starf.m_color + 1) & 3;
	}




// CIRCLE

	char rtext[] = "COMPLEX - INTRO - DIVISION --- ";

static int srotate = 0;
	
	int rotate = 1024;
	int amplitude = 128;

static	int iAlp = 4;
static 	int iDir = 1;

	for (int i = 0; i < 64; i++)
	{
			int srot = (srotate+(i*amplitude))&4095;
			float posx = CosTab[srot] * float(amplitude*2);
			float posy = SinTab[srot] * (resY/480.0f) * float(amplitude*2);
			rFont_DrawLetter(320+posx, (resY>>1)+posy, rtext[i%32], srotate + rotate, 48);
			rotate += amplitude;
	}

	srotate -= 4;

	iAlp += iDir;
	if (iAlp > 60) iDir = -iDir;
	if (iAlp < 4)  iDir = -iDir;



// BANDS
static int c1 = 0;

	int o = isPAL50 ? 1 : 0;

	BandColor(m_tVB[0],         0  ,rI->B1len,  0, RES_W, 0xFF000000, 0xFF000000);
	BandColor(m_tVB[1], rI->B2pos  ,rI->B2len,  0, RES_W, 0xFF000000, 0xFF000000);

	RenderState();

	SetRGBtexture(c1);
	RasterLine(m_tVB[2], rI->B1len-2, m_RGBtexture, 0);
	RasterLine(m_tVB[3], rI->B2pos+2, m_RGBtexture, 1);
	c1 = (c1 + 3) & 255;









// OPTIONS

	if (TrainerGo == 0)	DrawOptions();
	else				
	{
		if (MasterAlpha > 0)
		{
			MasterSwitch = 0;	DrawOptions();
			MasterSwitch = 1;	DrawIntroInfo(0);

			MasterAlpha -= 4;
			if (MasterAlpha < 0) MasterAlpha = 0;
		}
		else
			DrawIntroInfo(TrainerFade?0:1);
	}




// LOGO

static  int	 wAmp = 0;
		int  wAmpI = 4;
		int  wampl = 32 + int(SinTab[wAmp] * 32);


		wAmp += wAmpI;
		wAmp &= 4095;

static float logoWSinePos = 0.0f;
static float logoWSineInc = 28.0f;
static float logoHSinePos = 1024.0f;
static float logoHSineInc = 16.0f;

	int width  = 448 + (SinTab[ ((int)logoWSinePos)&4095 ] * (28+wampl));
	int height = rI->LGsiz + (SinTab[ ((int)logoHSinePos)&4095 ] * rI->LGamp);
	logoWSinePos += logoWSineInc;
	logoHSinePos += logoHSineInc;
	int left = 96 - ((width  - 448)>>1);
	int top  = 24 - ((height - rI->LGsiz)>>1);

	stats[statP].w = width;
	stats[statP].h = height;
	stats[statP].l = left;
	stats[statP].t = top;


	int alphas[2] = { 255, 64 };

	logotex.SetTexture();

	for (int i = 0; i < 2; i++)
	{
		lstat *s = &stats[((statP-1) + (i*8)) & 15];
		DrawQuad(s->l, s->t, s->w, s->h,  0,0,448,88,  0, alphas[i], 0xFFFFFF);
	}

	statP = (statP + 1) & 15;





// SCROLLER


static int txtptr = 0;

char mtx[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789><*/\\,.'&-:+@";
static int rot = 0;
static int pos = 640;

DWORD alpha = 255;

static int c2 = 0;

	


	gtex.SetTexture();


	// draw


	int rotation = int(SinTab[rot&4095] * 1024.0f);
	rot += 32;
	if (rotation < 0) rotation += 4096;

	for (int i = 0; i < 28; i++)
	{

		int j = (charsStart + i) % 28;

//		alpha = 255 - (DWORD)((float(abs(chars[i].posx - 320)) / 330) * 255);
		char c = chars[j].c;

		if ((c != ' ') && (c != 0))
		{
			char *s = mtx;
			int ofs = 0;

			while (1)
			{
				if (*s == 0) break;
				if (*s == c)
				{
					int col = (chars[j].posx + 24) >> 2;
					DrawQuad(chars[j].posx, rI->SCpos, 8*2,8*2,  ofs*8,0, 8,8, rotation, alpha, cols[(c2+col) & 255]);
					break;
				}

				s++; ofs++;
			}

		}
	}



	c2 = (c2 - 1) & 255;

	// update
	for (int i = 0; i < 28; i++)
	{
		if (--chars[i].posx <= -24)
		{
			chars[i].posx = 648;

			char c = scrolltext[txtptr++]; if ((c == 0) || (c == 'r')) { txtptr = 0; c = scrolltext[txtptr++]; }
			chars[i].c = c;
			charsStart = (charsStart + 1) % 28;
		}
	}






// SIRIOKD

	skdtex.SetTexture();
	DrawQuad(520, rI->SKpos, 64,16,  4,0,32,8,  0, 255, 0xFFFFFF);



static int masterAlpha = 0;


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


	if (TrainerFade)
	{
		if (masterAlpha < 255) 
		{
			BandFade(m_fVB, masterAlpha);
			masterAlpha += 2;
		}
		else TrainerLaunch();
		
	}
	else 
	{
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


/*

char temp[64];

	sprintf(temp,"%d - %d",iDir,iAlp);
	tFont_DrawString(64, 480-64, temp, 0xFFFFFFFF);
*/

	return S_OK;
}


//------------------------------------------------------------------------------
HRESULT CXBoxSample::Render()
//------------------------------------------------------------------------------
{
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,  0, 0), 1.0f, 0);

	FCp_Run();

	DoIntro();

	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);

	return S_OK;
}



