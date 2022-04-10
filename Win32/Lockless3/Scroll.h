
struct	scrInfo 
{
	char 	*scrFont;
	int	scrFontRow;
	char 	*scrDest;
	int	scrDestRow;

	char 	*scrText;
	char 	*scrFontMtx;
	int	scrFontW, scrFontH;
	int	TextPos, Wait;
	int	cX, cY;
	int	col, row;
	int	left, top;
	int	stop;
};


/*------------------------------------------------------------------------*/
void	scrInit(struct scrInfo * scroll)
/*------------------------------------------------------------------------*/
{
	scroll->TextPos = 0;
	scroll->Wait = 0;
	scroll->cX = 0;
	scroll->cY = 0;
	scroll->stop = 0;

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

//getchr:

	src = scroll->scrFont;
	dst = scroll->scrDest + 
		((scroll->cY * (scroll->scrFontH+2)) * scroll->scrDestRow) + 
		(scroll->cX * scroll->scrFontW);
	
// GETCHAR


	c = scroll->scrText[scroll->TextPos++];

	if (c == 's') { scroll->stop = 1; return 0; }
//	if (c == 'é') { scroll->TextPos = 0; goto getchr; }
//	if (c == 'w') { scroll->Wait = (60*3); return 0; }
//	if (c == 'c') { scrCR(scroll); goto getchr; }
	
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
int		scrExecute(struct scrInfo * scroll)
/*------------------------------------------------------------------------*/
{
	if (scroll->stop) return 1;
	if (scroll->Wait) { scroll->Wait--; return 0; }
	scrWriteNext(scroll);
	return 0;
}

