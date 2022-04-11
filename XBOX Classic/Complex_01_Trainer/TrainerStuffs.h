UBYTE	TrainerData[32];

int	OptionMaxChar;
int	OptionCursor = 0;
int	OptionPageCursor = 0;
int OptionSineCursor = 0;
int OptionCursorMoveWait = 0;
int OptionCursorMoveMax = 0;

int			TrainerGo = 0;
int			TrainerFade = 0;



int	startPressed = 0;
int	MasterAlpha = 255;
int MasterSwitch = 0;

/*----------------------------------------------------------*/
void	TrainerLaunch(void)
/*----------------------------------------------------------*/
{
	RemapDrive('T');

	HANDLE hTrn = CreateFile("T:\\cpx", 
							GENERIC_WRITE, 0, NULL, 
							CREATE_ALWAYS, 
							FILE_ATTRIBUTE_NORMAL, NULL);


	if (hTrn != INVALID_HANDLE_VALUE)
	{
		DWORD written = 0;
		WriteFile(hTrn, (LPCVOID)TrainerData, (DWORD)TrainerDataSize, &written, NULL);
		FlushFileBuffers(hTrn);
		CloseHandle(hTrn);

		LAUNCH_DATA LaunchData;	
		XLaunchNewImage( "d:\\cpx", &LaunchData );
	}
}

/*----------------------------------------------------------*/
void	TrainerStart(void)
/*----------------------------------------------------------*/
{
		if (++TrainerGo >= 2) TrainerFade = 1;
}


/*----------------------------------------------------------*/
int		OptionStrLen(int item)
/*----------------------------------------------------------*/
{
int len = 32;
char *p = Options+(item*32)+31;

	while ((len > 0) && (*p == ' ')) { *p-- = 0; len--; }

//	Options[(item*32)+31] = 0;
	if (len > 0) Options[(item*32)+len] = 0;

	return len;
}


/*----------------------------------------------------------*/
void	OptionsInit(void)
/*----------------------------------------------------------*/
{
int i,k;

	OptionMaxChar = 0;
	TrainerDataSize &= 31;

	for (i = 0; i < 32; i++)
	{
		TrainerData[i] = 0;

		k = OptionStrLen(i); if (k > OptionMaxChar) OptionMaxChar = k;

	}

	OptionCursorMoveMax = 7;
	if (TrainerDataSize+1 < OptionCursorMoveMax) OptionCursorMoveMax = TrainerDataSize+1;
}





/*-------------------------------------------------------------------------------------------*/
void	OptionsWriteChar(int x, int y, float shiftX, float shiftY, char c, int rot, int col)
/*-------------------------------------------------------------------------------------------*/
{
	int alpha = (MasterSwitch == 0) ? MasterAlpha : 255-MasterAlpha;
	xbApp.tFont_DrawLetter(float(x*8)+shiftX, (y*24)+shiftY, c, rot, col, alpha);

}



/*----------------------------------------------------------------------------------*/
void	OptionsWriteStringZ(int x, int y, float shiftX, float shiftY, char *s, int rot, int col)
/*----------------------------------------------------------------------------------*/
{
	for (int i = 0; *s; i++) 
		OptionsWriteChar(x+i,y, shiftX, shiftY, *s++, rot, col);
}


/*----------------------------------------------------------*/
void	OptionsWriteStringZCentered(int y, char *s, int rot, int col)
/*----------------------------------------------------------*/
{
int len = strlen(s);
int posx = ((RES_W>>3) - len) >> 1;


	for (int i = 0; *s; i++) 
		OptionsWriteChar(posx+i,y,0,0,*s++, rot, col);
}



/*-----------------------------------------------------------------------*/
void	OptionsWriteItem(int x, int y, int item, int onoff, int cursor)
/*-----------------------------------------------------------------------*/
{
float shift = 0;
int	rot = 0;

	if (cursor)	shift = (xbApp.SinTab[OptionSineCursor] * 16);
	if (cursor)	rot = int(xbApp.SinTab[OptionSineCursor] * 200);
//	if (cursor)	shift = 16;;


	if (item >= TrainerDataSize)
	{
		char startString[] = "START GAME";
		int len = strlen(startString);
		OptionsWriteStringZ(x+OptionMaxChar-len+1, y, shift, 4, startString, rot, 0xFFFFFFFF);
		return;
	}


char *s = Options+(item*32);
int  i,len = strlen(s);

	OptionsWriteStringZ(x, y, shift, 4, s, rot, 0xFFFFFFFF);
	for (i = 0; i < OptionMaxChar-len+7; i++) OptionsWriteChar(x+len+1+i, y, shift, 4, '.', rot, 0xFFFFFFFF);
	OptionsWriteStringZ(x+OptionMaxChar+9, y, shift, 4, (onoff)?"YES":"NO", rot, 0xFFFFFFFF);
}


/*-----------------------------------------------------------------------*/
void	OptionsWriteStart(int x, int y, int cursor)
/*-----------------------------------------------------------------------*/
{
float shift = 0;
char startString[] = "START GAME";
int len = strlen(startString);

int	rot = 0;

	if (cursor)	shift = (xbApp.SinTab[OptionSineCursor] * 16);
	if (cursor)	rot = int(xbApp.SinTab[OptionSineCursor] * 200);
//	if (cursor)	shift = 16;;

	OptionsWriteStringZ(x+OptionMaxChar-len+1, y, shift, 4, startString, rot, 0xFFFFFFFF);
}



/*----------------------------------------------------------*/
void	DrawOptions(void)
/*----------------------------------------------------------*/
{
int	len = strlen(TrainerTitle);

int cursor = OptionCursor;
int centerx = ((RES_W>>3)-(OptionMaxChar+12))>>1;
int centery = xbApp.isPAL50 ? 10 : 8;


	int col = (TrainerTitleColor & 0x00FFFFFF) | 0xFF000000;
	OptionsWriteStringZCentered(centery-2,TrainerTitle,0, col);




int	sta = OptionPageCursor;

	for (int i = 0; i < OptionCursorMoveMax; i++)
		OptionsWriteItem(centerx, centery+i, sta+i, TrainerData[sta+i], (cursor==i)?1:0);


	OptionSineCursor = (OptionSineCursor + 72) & 4095;
}



/*----------------------------------------------------------*/
void	CursorUp(void)
/*----------------------------------------------------------*/
{

	if (OptionCursor > 0) OptionCursor--;
	else
	{
		if (OptionPageCursor > 0) OptionPageCursor--;
	}


//	if (OptionPageCursor > 0) OptionPageCursor--;
}

/*----------------------------------------------------------*/
void	CursorDown(void)
/*----------------------------------------------------------*/
{

	if (OptionCursor < OptionCursorMoveMax-1) OptionCursor++;
	else
	{
		if ((OptionPageCursor+OptionCursorMoveMax) < TrainerDataSize+1) OptionPageCursor++;
	}


//	if ((OptionPageCursor+OptionCursorMoveMax) < TrainerDataSize-1) OptionPageCursor++;
}


/*----------------------------------------------------------*/
void	ChangeOption(void)
/*----------------------------------------------------------*/
{
int	pos = (OptionPageCursor + OptionCursor);


if (pos == TrainerDataSize) { TrainerStart(); return; }

	pos = (pos < 0) ? 0 : ((pos >= TrainerDataSize) ? TrainerDataSize-1 : (pos));

	TrainerData[pos] = (TrainerData[pos] + 1) & 1;

}


/*----------------------------------------------------------*/
void	CursorDo(int op)
/*----------------------------------------------------------*/
{
	if (OptionCursorMoveWait == 0) 
	{
		switch (op)
		{
			case 0: CursorUp();		OptionCursorMoveWait = 8;   break;
			case 1: CursorDown();	OptionCursorMoveWait = 8;   break;
			case 2: ChangeOption();	OptionCursorMoveWait = 400; break;
		}

	}
	else
		OptionCursorMoveWait--;
}








/*----------------------------------------------------------*/
void	DrawIntroInfo(int last)
/*----------------------------------------------------------*/
{
int centery = xbApp.isPAL50 ? 8 : 6;
int centerx = 20;

	OptionsWriteStringZ(centerx,centery+0,0,0,credits1,0, 0xFFFF00C0);
	OptionsWriteStringZ(centerx,centery+1,0,6,credits2,0, 0xFFFFFFFF);
	OptionsWriteStringZ(centerx,centery+2,0,6,credits3,0, 0xFFFFFFFF);
	OptionsWriteStringZ(centerx,centery+3,0,6,credits4,0, 0xFFFFFFFF);
	OptionsWriteStringZ(centerx,centery+4,0,6,credits5,0, 0xFFFFFFFF);
	OptionsWriteStringZ(centerx,centery+5,0,6,credits6,0, 0xFFFFFFFF);

	
static int k = 0;

	if (last)
	{
		if (++k < 40) OptionsWriteStringZ(centerx, centery+7, 0, 0, credits7, 0, 0xFF00FFC0);
		k &= 63;
	}

}




