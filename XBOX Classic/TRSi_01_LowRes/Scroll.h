
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
	scroll->rows = (scroll->scrFontH +2) * 16;
}


/*------------------------------------------------------------------------*/
void	scrClr(struct scrInfo * scroll)
/*------------------------------------------------------------------------*/
{
int row1, row2;

	for (int i = 0; i < 2; i++)
	{
		row1 = scroll->rows - (scroll->clrcnt*2);
		row2 = scroll->rows - 1 - row1;

		fastSet((UBYTE*)scroll->scrDest+(scroll->scrDestRow*row1),0, 32 * scroll->scrFontW);
		fastSet((UBYTE*)scroll->scrDest+(scroll->scrDestRow*row2),0, 32 * scroll->scrFontW);

		if (scroll->clrcnt) scroll->clrcnt--;
	}
}

/*------------------------------------------------------------------------*/
void	scrCR(struct scrInfo * scroll)
/*------------------------------------------------------------------------*/
{
	scroll->cX = 0;
	scroll->cY++;
}


/*------------------------------------------------------------------------*/
int	scrWriteNext(struct scrInfo * scroll)
/*------------------------------------------------------------------------*/
{
int	i;
char c, *src, *dst;

getchr:

	src = scroll->scrFont;
	dst = scroll->scrDest + 
		((scroll->cY * (scroll->scrFontH+2)) * scroll->scrDestRow) + 
		(scroll->cX * scroll->scrFontW);
	
// GETCHAR


	c = scroll->scrText[scroll->TextPos++];

//	if (c == 's') { scroll->stop = 1; return 0; }
//	if (c == 'é') { scroll->TextPos = 0; goto getchr; }

	if (c == 'w') 
	{ 
		scroll->Wait = int(500.0f * scaleval);
		return 0; 
	}

	if (c == 'q') 
	{ 
		scroll->Wait = int(700.0f * scaleval);
		return 0; 
	}

//	if (c == 'c') { scrCR(scroll); goto getchr; }
	if (c == ' ') { if (++scroll->cX >= scroll->col) scrCR(scroll); goto getchr; }
	
// SEARCH

	for (i = 0; ; i++) 
	{
		if (c == scroll->scrFontMtx[i]) break;
		if (c == 'é') { i = 0; break; }

	}


// CALCPOS

	src += i * scroll->scrFontW;

// COPY
	for (i = 0; i < scroll->scrFontH; i++)
	{
		copyROW8( src + (scroll->scrFontRow * i),
				   dst + (scroll->scrDestRow * i));
	}
	
	
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
		scroll->Wait--;

		if (scroll->Wait == 120) 
		{
			scroll->clrcnt = scroll->rows / 2;
			scroll->TextPos = 0;
			scroll->cX = scroll->cY = 0;
			if (scroll->scrPages[scroll->scrPage] == 0) scroll->scrPage = 0;
			scroll->scrText = scroll->scrPages[scroll->scrPage++];
		}
		else if (scroll->Wait < 120) scrClr(scroll); 

		return 0; 
	}


	scrWriteNext(scroll);
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


