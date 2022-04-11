int	OptionCursor = 0;
int	OptionPageCursor = 0;
int OptionSineCursor = 0;
int OptionSineCursorOff = 0;
int OptionCursorMoveWait = 0;
int OptionCursorItemNum = 0;

int	TrainerGo = 0;
int	TrainerFade = 2;
int	TrainerDataSize = 2;

int OptionsOffset_X = 66;
int OptionsOffset_Y = 104;
int OptionsRowWidth = 230;
int OptionsRowHeight = 54;
int OptionsVisibleItems = 4;


int	startPressed = 0;
int	MasterAlpha = 255;
int MasterSwitch = 0;



#include <string.h>


typedef struct _UNICODE_STRING 
{
	USHORT Length;
	USHORT MaximumLength;
	PSTR  Buffer;

} UNICODE_STRING, *PUNICODE_STRING;

#define IN

extern "C" XBOXAPI DWORD WINAPI IoCreateSymbolicLink(IN PUNICODE_STRING SymbolicLinkName,IN PUNICODE_STRING DeviceName);
extern "C" XBOXAPI DWORD WINAPI IoDeleteSymbolicLink(IN PUNICODE_STRING SymbolicLinkName);


#define DriveD "\\??\\D:"
UNICODE_STRING DSymbolicLinkName = { strlen(DriveD), strlen(DriveD)+1, DriveD };

UNICODE_STRING GameDeviceName = { 0, 0, 0 };

char	t_dev[128];



/*----------------------------------------------------------*/
void	TrainerLaunch(void)
/*----------------------------------------------------------*/
{
int		item = OptionCursor + OptionPageCursor;

		strcpy(t_dev, "\\Device\\Cdrom0\\");
		strcat(t_dev, xbApp.ir.m_Items[item].Dir);

		GameDeviceName.Length = strlen(t_dev);
		GameDeviceName.MaximumLength = GameDeviceName.Length + 1;
		GameDeviceName.Buffer = t_dev;

		IoDeleteSymbolicLink(&DSymbolicLinkName);
		IoCreateSymbolicLink(&DSymbolicLinkName, &GameDeviceName);

LAUNCH_DATA LaunchData;	
		XLaunchNewImage( "d:\\default.xbe", &LaunchData );
}



/*----------------------------------------------------------*/
void	TrainerStart(void)
/*----------------------------------------------------------*/
{
		TrainerGo = 2;
		TrainerFade = 1;
}




/*----------------------------------------------------------*/
void	OptionsInit(void)
/*----------------------------------------------------------*/
{
	TrainerDataSize = xbApp.ntitles;

	OptionCursorItemNum = OptionsVisibleItems;

	if (OptionCursorItemNum > TrainerDataSize) 
				OptionCursorItemNum = TrainerDataSize;


}



/*-----------------------------------------------------------------------*/
void	OptionsWriteCursorBand(void)
/*-----------------------------------------------------------------------*/
{
	OptionSineCursorOff = 0;
	int alpha = 150 - (xbApp.SinTab[(OptionSineCursor + OptionSineCursorOff) & 4095] * 32);


	xbApp.BandColor(xbApp.m_tVB[0], 
					OptionsOffset_Y + 1 + (OptionCursor*OptionsRowHeight), OptionsRowHeight,
					OptionsOffset_X, OptionsRowWidth, 
					0xDFDF00, 0xDFDF00, alpha);
}


static	int arrowFlashCounter = 0;

/*-----------------------------------------------------------------------*/
void	OptionsWriteArrows(void)
/*-----------------------------------------------------------------------*/
{
	if (++arrowFlashCounter > 30) arrowFlashCounter = 0;

	if (arrowFlashCounter < 20)
	{
		if (xbApp.ir.m_ItemsNum > OptionsVisibleItems)
		{
			if (OptionPageCursor > 0)
			{
				xbApp.opt_updn_tex.SetTexture();

				xbApp.DrawQuad(
					OptionsOffset_X + (OptionsRowWidth - 32),
					OptionsOffset_Y - 16 - 2,
					32,16,
					0,0,32,16,0,128,0xDDDD00);
			}

			if ((OptionPageCursor + OptionCursorItemNum) < xbApp.ir.m_ItemsNum)
			{
				xbApp.opt_updn_tex.SetTexture();

				xbApp.DrawQuad(
					OptionsOffset_X + (OptionsRowWidth - 32),
					OptionsOffset_Y + (OptionsRowHeight * OptionsVisibleItems) + 2,
					32,16,
					32,0,32,16,0,128,0xDDDD00);
			}
		}

	}

}

/*-----------------------------------------------------------------------*/
void	OptionsWriteShot(int posX, int posY, int sizX, int sizY, int alpha)
/*-----------------------------------------------------------------------*/
{
	xbApp.shots_tex[OptionCursor+OptionPageCursor]->SetTexture();
	xbApp.DrawQuad(posX,posY,sizX,sizY,0,0,sizX,sizY,0,alpha,0xFFFFFFFF);
}

/*-----------------------------------------------------------------------*/
void	OptionsWriteItem(int x, int y, int item, int cursor, int opt)
/*-----------------------------------------------------------------------*/
{
	int alpha = 140 - (xbApp.SinTab[(OptionSineCursor + OptionSineCursorOff) & 4095] * 32);

	DWORD color = (cursor == 0) ? 0x00FFFF00: 0xFF000000;
	if (cursor == 0) color |= (alpha & 255) << 24;

		if (opt)
		{
			xbApp.m_Font2.DrawText( x, y, color, xbApp.Titles[item], 0,0);
		}
		else
		{
			xbApp.m_Font1.DrawText( 6+x, 24+y, color, xbApp.Infos[item], 0,0);
		}

}




/*----------------------------------------------------------*/
void	DrawOptions(void)
/*----------------------------------------------------------*/
{
	if (!xbApp.font_ok) return;

int centerx = OptionsOffset_X + 6;

	xbApp.m_Font2.Begin();
	for (int i = 0; i < OptionCursorItemNum; i++)
	{
		OptionsWriteItem(centerx, OptionsOffset_Y + (i*OptionsRowHeight), OptionPageCursor + i, (OptionCursor==i)?1:0, 1);
		OptionSineCursorOff += 16;

	}
	xbApp.m_Font2.End();



	OptionSineCursorOff = 0;

	xbApp.m_Font1.Begin();
	for (int i = 0; i < OptionCursorItemNum; i++)
	{
		OptionsWriteItem(centerx, OptionsOffset_Y + (i*OptionsRowHeight), OptionPageCursor + i, (OptionCursor==i)?1:0, 0);
		OptionSineCursorOff += 16;
	}
	xbApp.m_Font1.End();


	OptionSineCursor = (OptionSineCursor + 16) & 4095;
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
}

/*----------------------------------------------------------*/
void	CursorDown(void)
/*----------------------------------------------------------*/
{

	if (OptionCursor < OptionCursorItemNum - 1) OptionCursor++;
	else
	{
		if ((OptionPageCursor + OptionCursorItemNum) < TrainerDataSize) 
														OptionPageCursor++;
	}
}




/*----------------------------------------------------------*/
void	CursorDo(int op)
/*----------------------------------------------------------*/
{
	if (OptionCursorMoveWait == 0) 
	{
		switch (op)
		{
			case 0: CursorUp();		OptionCursorMoveWait = 10;  break;
			case 1: CursorDown();	OptionCursorMoveWait = 10;  break;
			case 2: TrainerStart();	OptionCursorMoveWait = 400; break;
		}

	}
	else
		OptionCursorMoveWait--;
}












