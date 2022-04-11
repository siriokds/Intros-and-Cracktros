#include "Types.h"
#include "cGif.h"

cGifImage	gImage;

/*----------------------------------------------------------------*/
class	GifTexture
/*----------------------------------------------------------------*/
{
public:

LPDIRECT3DTEXTURE8      m_texture;
	int		m_width, m_height;

	int		Init(UBYTE *gifPtr, int filled);
	int		InitPNG(char *fname);
	int		InitPNG(char *memptr, int pngsize);
	void	SetTexture(void) { 	g_pd3dDevice->SetTexture( 0, m_texture ); }

};




/*----------------------------------------------------------------*/
int	GifTexture::Init(UBYTE *s, int filled)
/*----------------------------------------------------------------*/
{
	if (!s) return -1;
	if (gImage.GetSize(s, &m_width, &m_height) == -1) return -1;

	g_pd3dDevice->CreateTexture( m_width, m_height, 1, 0, D3DFMT_LIN_A8R8G8B8, D3DPOOL_MANAGED, &m_texture);

	DWORD *mem = new DWORD[m_width*m_height];
	gImage.LoadGifFromMemory(s, (UBYTE*)mem);
	
	D3DLOCKED_RECT d3dlr;
	m_texture->LockRect(0,&d3dlr,0,0);

	DWORD	*src = mem;
	DWORD	*dst = (DWORD*)d3dlr.pBits;
	int	pitch = d3dlr.Pitch>>2;

	if (filled)
	{
		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++) *dst++ = *src++;
		}
				
		dst += pitch - m_width;
	}
	else
	{
		DWORD zerocol = *src;
		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++) 
			{
				*dst++ = (*src == zerocol) ? 0 : *src;
				src++;
			}
			dst += pitch - m_width;
		}
	}

	m_texture->UnlockRect(0);

	delete [] mem;	

	return 0;
}


/*----------------------------------------------------------------*/
int	GifTexture::InitPNG(char *fname)
/*----------------------------------------------------------------*/
{
	DWORD dwColorKey = 0;
	D3DXIMAGE_INFO info;

	if ( D3DXCreateTextureFromFileEx(
				g_pd3dDevice, 
				fname,
				D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_LIN_A8R8G8B8, D3DPOOL_MANAGED,
				D3DX_FILTER_NONE , D3DX_FILTER_NONE, dwColorKey, &info, NULL, &m_texture)!=D3D_OK) return -1;
				
		return -1;

	return 0;
}


/*----------------------------------------------------------------*/
int	GifTexture::InitPNG(char *memptr, int pngsize)
/*----------------------------------------------------------------*/
{
	DWORD dwColorKey = 0;
	D3DXIMAGE_INFO info;

	if ( D3DXCreateTextureFromFileInMemoryEx(
				g_pd3dDevice, 
				memptr, pngsize,
				D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_LIN_A8R8G8B8, D3DPOOL_MANAGED,
				D3DX_FILTER_NONE , D3DX_FILTER_NONE, dwColorKey, &info, NULL, &m_texture)!=D3D_OK) return -1;
				
		return -1;

	return 0;
}
