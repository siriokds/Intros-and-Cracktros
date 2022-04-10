
struct	scrInfo 
{
	char *scrFont;
	int	 scrFontRow;
	char *scrDest;
	int	 scrDestRow;

	char **scrPages;
	int	 scrPage;
	char *scrText;
	char *scrFontMtx;
	int	 scrFontW, scrFontH;
	int	 TextPos, Wait;
	int	 cX, cY;
	int	 col, row;
	int	 left, top;
	int	 stop;
	int	 clrcnt;
	int	 rows;
};


/*------------------------------------------------------------------------*/
void	scrInit(struct scrInfo * scroll)
/*------------------------------------------------------------------------*/
{
	scroll->scrPage = 0;
	scroll->scrText = scroll->scrPages[scroll->scrPage++];
	scroll->TextPos = 0;
	scroll->Wait = 0;
	scroll->cX = 0;
	scroll->cY = 0;
	scroll->stop = 0;
	scroll->rows = scroll->scrFontH * 8;
}


/*------------------------------------------------------------------------*/
void	scrClr(struct scrInfo * scroll)
/*------------------------------------------------------------------------*/
{
	for (int i = 0; i < (scroll->scrFontH+2) * 10; i++)
		fastSet((UBYTE*)scroll->scrDest+(scroll->scrDestRow*i),0, scroll->scrDestRow);

}

/*------------------------------------------------------------------------*/
void	scrCR(struct scrInfo * scroll)
/*------------------------------------------------------------------------*/
{
	scroll->cX = 0;
	scroll->cY++;
}



/*--------------------------------------------------------------------------------------*/
void	scrWriteChar(struct scrInfo * scroll, char c, char *dst, int dst_pitch, int tr)
/*--------------------------------------------------------------------------------------*/
{
// SEARCH
	for (int i = 0; ; i++) 
	{
		if (c == scroll->scrFontMtx[i]) break;
		if (scroll->scrFontMtx[i] == 0) { i = 0; break; }
	}

// CALCPOS
	char *src = scroll->scrFont + (i * scroll->scrFontW);

// COPY
	if (tr)
	{
		for (i = 0; i < scroll->scrFontH; i++)
		{
			int oS = scroll->scrFontRow * i;
			int oD = dst_pitch * i;

			for (int j = 0; j < scroll->scrFontW; j++)
			{
				if ( src[oS+j] ) dst[oD+j] = src[oS+j];
			}
		}
	}
	else
	{
		for (i = 0; i < scroll->scrFontH; i++)
		{
			copyROW8( src + (scroll->scrFontRow * i), dst + (dst_pitch * i));
		}
	}

	
}

/*------------------------------------------------------------------------*/
int	scrWriteNext(struct scrInfo * scroll)
/*------------------------------------------------------------------------*/
{
char c, *dst;

//getchr:

	dst = scroll->scrDest + 
		((scroll->cY * (scroll->scrFontH+2)) * scroll->scrDestRow) + 
		(scroll->cX * scroll->scrFontW);
	
// GETCHAR


	c = scroll->scrText[scroll->TextPos++];

//	if (c == 's') { scroll->stop = 1; return 0; }
//	if (c == 'é') { scroll->TextPos = 0; goto getchr; }

	if (c == 'w') 
	{ 
		scroll->Wait = int(400.0f * scaleval);
		return 0; 
	}

	if (c == 'q') 
	{ 
		scroll->Wait = int(500.0f * scaleval);
		return 0; 
	}

	if (c == ' ') goto exnd;

	scrWriteChar( scroll, c, dst, scroll->scrDestRow, 0 );

	
exnd:	
	if (++scroll->cX >= scroll->col) scrCR(scroll);

	return 1;
}


/*------------------------------------------------------------------------*/
int		scrDo(struct scrInfo * scroll)
/*------------------------------------------------------------------------*/
{
//	if (scroll->stop) return 1;
	if (scroll->Wait > 0) 
	{ 
		if (--scroll->Wait == 0) 
		{
			scroll->clrcnt = scroll->rows / 2;
			scroll->TextPos = 0;
			scroll->cX = scroll->cY = 0;
			if (scroll->scrPages[scroll->scrPage] == 0) scroll->scrPage = 0;
			scroll->scrText = scroll->scrPages[scroll->scrPage++];

			scrClr(scroll);
		}

		return 0; 
	}

	for (int i = 0; i < 10; i++) scrWriteNext(scroll);

	return 0;
}



float	scrCounter = 0.0f;

/*------------------------------------------------------------------------*/
int		scrExecute(struct scrInfo * scroll)
/*------------------------------------------------------------------------*/
{
	if (scaleval < 5) scrCounter += scaleval;

	while (scrCounter > 1)
	{
		scrCounter -= 1;
		scrDo(scroll);
	}
	return 0;
}


