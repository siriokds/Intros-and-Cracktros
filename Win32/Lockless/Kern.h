
/*---------------------------------------------------------------*/
void	copy1Tile(UBYTE *frameb, int x, int y, int pitch)
/*---------------------------------------------------------------*/
{
char	*til = (char*)imx + 778 + (256*16*3)+(16*15);
char	*dst = (char*)frameb + (y*pitch) + x + 16;


	for (int j = 0; j < 16; j++)
	{
		copyROW16( til + (j<<8), dst + (j*pitch) );
	}
}




/*---------------------------------------------------------------*/
void	copyTile(UBYTE *frameb, int pitch)
/*---------------------------------------------------------------*/
{
int		srcX = (16*2);
int		srcY = (16*4)+2;

char	*src = (char*)imx + 778 + srcX + (srcY << 8);
char	*srcrow;
static	int col = 256;

	if (!scroll_Info.Bck) return;

	src += ((col >> 4) &511);

	for (int i = 0; i < 15; i++)
	{
		srcrow = src + (i << 8);
		for (int j = 0; j < 20+1; j++)
		{
			if (srcrow[j & 511])  copy1Tile(frameb, (j<<4)-(col&15), (i<<4), pitch);
		}
	}


	col += 4; if (col > 2200) col = 256;

}



static	int	sinevel = 2;
static	int	sineofs = 0;

/*
void	sinetabSCROLL_REW(int cnt)
{
	while (cnt--)
		sineofs = (sineofs + 1) & 511;
}
*/


/*---------------------------------------------------------------*/
void	sinetabSCROLL_FWD(int cnt)
/*---------------------------------------------------------------*/
{
	while (cnt--)
		sineofs = (sineofs - 1) & 511;
}

/*---------------------------------------------------------------*/
void	scroller(char *frameb, int pitch)
/*---------------------------------------------------------------*/
{
int		sine,offset;
int		i,k,v;
char	*src = SCRBUF;
char	*dst = 16 + frameb + (pitch*4) + (pitch * ((RESY - 200) / 2));
char	*col = (char*)imx+778+256*64;//FNTBUF + (320*48) + 640;

	scrExecute(&scroll_Info);


//	for (i = 0; i < 16; i++) 
//		memcpy(16+frameb+(i*pitch),src+(i*pitch),pitch);

//	return;

/*
float at = 0;
float t = 0;
int	j;
*/

	for (i = 0; i < RESX; i++)
	{
	  sine  = sinetab[ (i+(sineofs)) & 511 ];

/*
	  j = (int)t;
	  t += at;
	  at += 0.005f;

	  sine  = (sinetab[ (j+(sineofs)) & 511 ] / 2) + 44;
	  sine += (sinetab[ (j+(sineofs*2)) & 511 ] / 8) - 11;
*/

	  offset = sine * pitch;

	  for (k = 0; k < 16; k++)
	  {
	  	v = src[pitch * k] & 255;
	  	if (!v) continue;
	  	if (v == 254) 	dst[offset+(pitch * k)] = col[sine+k];
	  			else 	dst[offset+(pitch * k)] = v;
	  }
	
	  src++; 
	  dst++;
	}

	if (scroll_Info.Wait)	sinevel = 3;
			else 			sinevel = 2;

	sinetabSCROLL_FWD(sinevel);

}
