
struct	scrInfo 
{
	char *scrFont;
	int	scrFontRow;
	char *scrDest;
	int	scrDestRow;

	char *scrText;
	char *scrFontMtx;
	int	scrFontW, scrFontH;
	int	TextPos, ScrlPos, Wait, Vel, Bck;
};


/*------------------------------------------------------------------------*/
void	scrInit(struct scrInfo * scroll)
/*------------------------------------------------------------------------*/
{
	scroll->TextPos = 0;
	scroll->ScrlPos = 0;
	scroll->Wait = 0;
	scroll->Vel = 2;
	scroll->Bck = 0;

}

/*------------------------------------------------------------------------*/
int		scrWriteNext(struct scrInfo * scroll)
/*------------------------------------------------------------------------*/
{
int	i, rowsize;
char	c, *p, *src, *dst;

	src = scroll->scrFont;
	dst = scroll->scrDest + (scroll->scrDestRow - scroll->scrFontW);
	
// GETCHAR

	c = scroll->scrText[scroll->TextPos++];
	if (c == 'ø') { scroll->TextPos = 1; c = scroll->scrText[0]; }

	if (c == 'a') { scroll->Vel = 1; return 1; }
	if (c == 'b') { scroll->Vel = 2; return 1; }
	if (c == 'c') { scroll->Vel = 3; return 1; }
	if (c == 'z') { scroll->Wait = (60*3); return 0; }
	if (c == 'g') { scroll->Bck = 1; return 1; }
	
// SEARCH

	for (i = 0, p = scroll->scrFontMtx; *p != 'ø'; i++, p++) 
		if (*p == c) break;


// CALCPOS

	rowsize = scroll->scrFontRow * scroll->scrFontH;
	
	src += (i / scroll->scrFontW) * rowsize;
	src += (i & (scroll->scrFontW-1)) * scroll->scrFontW;


// COPY
	for (i = 0; i < scroll->scrFontH; i++)
	{
//		memcpy(  dst + (scroll->scrDestRow * i)
//			,src + (scroll->scrFontRow * i)
//			,scroll->scrFontW);

		copyROW16( src + (scroll->scrFontRow * i),
				   dst + (scroll->scrDestRow * i));
	}
	return 1;
}

/*---------------------------------------------------------------*/
void	scrShiftBuffer(struct scrInfo * scroll)
/*---------------------------------------------------------------*/
{
int	i,k,s = scroll->scrDestRow - 1;

	for (i = 0; i < scroll->scrFontH; i++)
	{
		k = scroll->scrDestRow * i;
//		memmove( scroll->scrDest + k, scroll->scrDest + k + 1, s);
		fastMove((UBYTE*) scroll->scrDest + k, (UBYTE*)scroll->scrDest + k + 1, s);
	}
}

/*------------------------------------------------------------------------*/
void	scrExecute(struct scrInfo * scroll)
/*------------------------------------------------------------------------*/
{
int	i;
int	scr;

	if (scroll->Wait) { scroll->Wait--; return; }
	
	for (i = 0; i < scroll->Vel; i++) 
	{
		if (!scroll->ScrlPos)
		{ 
			scr = scrWriteNext(scroll);
			scroll->ScrlPos = scroll->scrFontW;
		}

		if (scr)
		{
			scroll->ScrlPos--;
			scrShiftBuffer(scroll);
		}
	}
}
