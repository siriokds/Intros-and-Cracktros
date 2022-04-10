#include "Texter.h"

#define CURSOR_STARTX	1
#define CURSOR_STARTY	10
#define PAGE_WAIT		250

/*-----------------------------------------------------------------*/
TTexter::~TTexter()
/*-----------------------------------------------------------------*/
{
}

/*-----------------------------------------------------------------*/
TTexter::TTexter(TFastBMP *fnt, TFastBMP *frm,
				 char *mtx, char **pgs, TMtx *imtx,
				 int fntW, int fntH)
/*-----------------------------------------------------------------*/
{
	Waiting = 1;
	CursorX = CURSOR_STARTX;
	CursorY = CURSOR_STARTY;

	InfoMtx = imtx;

	Frame = frm;
	Font = fnt;
	Matrix = mtx;
	
	Pages = pgs;
	PagePos = 0;

	Text = Pages[0];
	TextPos = 0;

	FontWidth = fntW;
	FontHeight = fntH;

	NumCharX = fnt->Width / FontWidth;
	NumCharY = fnt->Height / FontHeight;

	if (Frame)
	{
		NumCharXD = Frame->Width / FontWidth;
		NumCharYD = Frame->Height / FontHeight;
	}

}

/*-----------------------------------------------------------------*/
int		TTexter::GetMatrixPos(char c)
/*-----------------------------------------------------------------*/
{
int		i,pos = -1;
char	*p = Matrix;

	for (i = 0; p[i] != 0; i++)
	{
		if (p[i] == c) 
		{
			pos = i;
			break;
		}
	}
	return pos;
}


/*-----------------------------------------------------------------*/
void	TTexter::CharCopy(TFastBMP *Dst, int DstX, int DstY, char c)
/*-----------------------------------------------------------------*/
{
int	pos, x, y;

	if (Dst == NULL) return;

	if ((pos = GetMatrixPos(c)) == -1)
	{
		x = 0;
		y = 0;

	} else 
	{
		y = pos / NumCharX;
		x = pos % NumCharX;
	}

	Font->DrawRect(Dst, DstX, DstY, 
					x*FontWidth, y*FontHeight, 
					FontWidth, FontHeight);
}


/*-----------------------------------------------------------------*/
void	TTexter::CursorIncY(void)
/*-----------------------------------------------------------------*/
{
		CursorY++;
		if (CursorY >= NumCharYD) CursorY = CURSOR_STARTY;
}

/*-----------------------------------------------------------------*/
void	TTexter::CursorIncX(void)
/*-----------------------------------------------------------------*/
{
		CursorX++;
		if (CursorX >= NumCharXD)
		{
			CursorX = CURSOR_STARTX;
			CursorIncY();
		}
}



/*-----------------------------------------------------------------*/
void	TTexter::CharWrite(char c)
/*-----------------------------------------------------------------*/
{
	if (InfoMtx) InfoMtx->Set(CursorX,CursorY,2);

	CharCopy(Frame, CursorX*FontWidth, CursorY*FontHeight, c);
	CursorIncX();
}


/*-----------------------------------------------------------------*/
char	TTexter::GetNextChar(void)
/*-----------------------------------------------------------------*/
{
char	c = '*';

	if (Text == NULL) return c;

	if ((c = Text[TextPos]) == 0)
	{
		TextPos = 0;
		c = Text[0];
	}

	TextPos++;

	return c;
}


/*-----------------------------------------------------------------*/
void	TTexter::ComputeChar(char c)
/*-----------------------------------------------------------------*/
{
	if (c == ']')
	{
		CursorX = CURSOR_STARTX;
		CursorIncY();
		return;
	}

	if (c == '|')
	{
		Waiting = PAGE_WAIT;
		return;
	}

	CharWrite(c);
}


/*-----------------------------------------------------------------*/
void	TTexter::RunOnce(void)
/*-----------------------------------------------------------------*/
{
	if (Waiting > 1) { Waiting--; return; }

	if (Waiting == 1)
	{
		Waiting = 0;
		CursorX = CURSOR_STARTX;
		CursorY = CURSOR_STARTY;

		TextPos = 0;
		Text = Pages[PagePos++];
		if (Text == 0) { PagePos = 0; Text = Pages[PagePos++]; }

		Frame->Fill(19,0,71, 
					(FontHeight*CURSOR_STARTY),
					Frame->Height-(FontHeight*10) 
		);

		if (InfoMtx)
			InfoMtx->SetRect(CURSOR_STARTX, 
						 CURSOR_STARTY, 
						InfoMtx->GetSizeX()-CURSOR_STARTX, 
						InfoMtx->GetSizeY()-CURSOR_STARTY,2);

		return;
	}

	ComputeChar(GetNextChar());
}

