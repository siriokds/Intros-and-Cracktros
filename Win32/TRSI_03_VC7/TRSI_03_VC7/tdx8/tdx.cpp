#include "tdx.h"

#include <ddraw.h>


#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }



LPDIRECTDRAW7			g_pDD = 0;
LPDIRECTDRAWSURFACE7	g_pDDSPrimary = 0;
LPDIRECTDRAWSURFACE7	g_pDDSBack = 0;
LPDIRECTDRAWPALETTE		g_pDDPalette = 0;
HWND					g_hWnd;
int 					g_Pitch;

DWORD					Palette[256];


/*-----------------------------------------------*/
void	__stdcall UpdatePalette(void)
/*-----------------------------------------------*/
{
	g_pDDPalette->SetEntries(0,0,256,(LPPALETTEENTRY)&Palette);
}

/*-----------------------------------------------*/
void	__stdcall tdxClose(void)
/*-----------------------------------------------*/
{
    SAFE_RELEASE( g_pDDPalette );
    SAFE_RELEASE( g_pDDSBack );
    SAFE_RELEASE( g_pDDSPrimary );
    
    if( g_pDD ) g_pDD->SetCooperativeLevel( g_hWnd, DDSCL_NORMAL );
    SAFE_RELEASE( g_pDD );
}




/*-----------------------------------------------*/
HRESULT __stdcall tdxOpen(DWORD hWnd)
/*-----------------------------------------------*/
{
HRESULT hr;

	tdxClose();
	g_hWnd = (HWND)hWnd;
	
//------------  
	if (FAILED( hr = DirectDrawCreateEx( NULL, (VOID**)&g_pDD, IID_IDirectDraw7, NULL ) ) ) return E_FAIL;
	if (FAILED(g_pDD->SetCooperativeLevel(g_hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN))) return E_FAIL;
	if (FAILED(g_pDD->SetDisplayMode(640, 480, 8, 60, 0 ))) return E_FAIL;

//------------  
	DDSURFACEDESC2 ddsd; for(int i=0; i<sizeof(ddsd); i++) ((char*)(&ddsd))[i]=0;

	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = 1;

	if (FAILED(hr = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL))) return E_FAIL;

	
//------------  
	DDSCAPS2 ddscaps; for(i=0; i<sizeof(ddscaps); i++) ((char*)(&ddscaps))[i]=0;

	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	
	if (FAILED(g_pDDSPrimary->GetAttachedSurface(&ddscaps, &g_pDDSBack))) return E_FAIL;
	g_pDDSBack->AddRef();

//------------  

	if (FAILED(g_pDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, (LPPALETTEENTRY)&Palette, &g_pDDPalette, NULL))) return E_FAIL;
	if (FAILED(g_pDDSPrimary->SetPalette(g_pDDPalette))) return E_FAIL;

//------------  

	RECT m_rcWindow;
        SetRect( &m_rcWindow, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	
	return S_OK;
}






/*-----------------------------------------------*/
void	__stdcall tdxSetPal(UBYTE *pal)
/*-----------------------------------------------*/
{
	for (int i = 0; i < 256; i++) 
	{
		Palette[i] = ((*((DWORD*)pal) & 0x003F3F3F) << 2) | 0xFF000000;
		pal += 3;
	}
	
	UpdatePalette();
}

/*-----------------------------------------------*/
void	__stdcall tdxSetPal8(UBYTE *pal)
/*-----------------------------------------------*/
{
	for (int i = 0; i < 256; i++) 
	{
		Palette[i] = *((DWORD*)pal) | 0xFF000000;
		pal += 3;
	}
	UpdatePalette();
}


/*-----------------------------------------------*/
void	__stdcall tdxWait4VBlank(int end)
/*-----------------------------------------------*/
{
	g_pDD->WaitForVerticalBlank( end ? DDWAITVB_BLOCKEND : DDWAITVB_BLOCKBEGIN, 0);
}


/*-----------------------------------------------*/
void	Clear()
/*-----------------------------------------------*/
{
	DDBLTFX fx;
	fx.dwSize = sizeof(fx);
	fx.dwFillColor = 0xffffff;
	g_pDDSBack->Blt(0, 0, 0, DDBLT_COLORFILL|DDBLT_WAIT, &fx);
}






/*------------------------------------*/
HRESULT __stdcall tdxPresent(void)
/*------------------------------------*/
{
    HRESULT hr;

    if( NULL == g_pDDSPrimary && NULL == g_pDDSBack ) return E_POINTER;

    while( 1 )
    {
	hr = g_pDDSPrimary->Flip( NULL, 0 );

        if( hr == DDERR_SURFACELOST )
        {
		if (FAILED(hr = g_pDD->RestoreAllSurfaces())) return hr;

		SAFE_RELEASE(g_pDDPalette);
		if (FAILED(g_pDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, (LPPALETTEENTRY)&Palette, &g_pDDPalette, NULL))) return E_FAIL;
		if (FAILED(g_pDDSPrimary->SetPalette(g_pDDPalette))) return E_FAIL;
		UpdatePalette();
        }

        if( hr != DDERR_WASSTILLDRAWING ) return hr;
    }
}



/*----------------------------------------------------*/
HRESULT __stdcall tdxUpdate(UBYTE *src, int pitch)
/*----------------------------------------------------*/
{
	HRESULT hr;

	hr = tdxPresent(); if (FAILED(hr)) return hr;
		

	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
	if (FAILED(g_pDDSBack->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL))) return E_FAIL;

	UBYTE *dst = (UBYTE*)ddsd.lpSurface;
  	g_Pitch = ddsd.lPitch;


	for (int y = 0; y < 240; y++)
	{
		for (int x = 0; x < 320; x++) 
		{ 
			dst[0] = src[0];
			dst[1] = src[0];
			dst[0 + g_Pitch] = src[0];
			dst[1 + g_Pitch] = *src++;
			dst += 2;
		}
		dst += g_Pitch;
		
		src += pitch - 320;
		dst += g_Pitch - 640;
	}

	if (FAILED(g_pDDSBack->Unlock(NULL))) return E_FAIL;
	
	return S_OK;
}



