#include "CIniReader.h"



void	CIniReader::Reset(void)
{
	if (m_Items)
	{
		for (int i = 0; i < CINI_MAXTITLES; i++)
								m_Items[i].Reset();
	}

	m_ItemsNum = 0;

	m_posX = -1;
	m_posY = -1;
	m_sizX = -1;
	m_sizY = -1;
	m_alpha = 200;

	m_options_posX = -1;
	m_options_posY = -1;
	m_options_sizX = -1;
	m_options_sizY = -1;
	m_options_rows = -1;

}


	CIniReader::CIniReader(void)
{
	m_Items = new CItem[CINI_MAXTITLES];
	m_ItemsNum = 0;

	m_posX = -1;
	m_posY = -1;
	m_sizX = -1;
	m_sizY = -1;
	m_alpha = 200;

	m_options_posX = -1;
	m_options_posY = -1;
	m_options_sizX = -1;
	m_options_sizY = -1;
	m_options_rows = -1;

}



	CIniReader::~CIniReader()
{
	if (m_Items) 
	{
		delete [] m_Items;
		m_Items = 0;
	}

	m_ItemsNum = 0;
}




void	CIniReader::ReadIni(char *fname)
{
FILE	*fp = fopen(fname,"rt");

		if (!fp) return;


char	temp[128];

		while (!feof(fp))
		{
			fgets(temp, 128, fp);
			ComputeLine(temp);
		}
		fclose(fp);
}




int		CIniReader::SplitLine(char *line, char *command, char *param)
{
char	*u = line;

	while ((*u != '=') && (*u != 0)) u++;
	if (*u == 0) return -1;

char	*p = line;
int		i;

		while (*p == ' ') p++;
		for (i = 0; i < 128; i++)
		{
			command[i] = *p++;
			if (*p == '=') break;
		}
		command[i+1] = 0;
		while (command[i] == ' ' && i > 0) command[i--] = 0;



		p = u + 1;

		while (*p == ' ') p++;
		for (i = 0; i < 128; i++)
		{
			param[i] = *p++;
			if (*p == 0) break;
		}
		param[i+1] = 0;
		while (param[i] == ' ' && i > 0) param[i--] = 0;

		return 0;
}




int		CIniReader::SplitCoord(char *line, int *x, int *y)
{
char	*p1s = line;
char	*p1e = p1s;
int		second = 0;

	while ((*p1e != ',') && (*p1e != 0)) p1e++;
	if (*p1e == ',') second = 1;
	*p1e = 0;
	*x = atoi(p1s);

	if (y != 0)
	{
		if (second)
		{
			char *p2s = p1e + 1;
			char *p2e = p2s;
			while (*p2e != 0) p2e++;
		
			*y = atoi(p2s);
		}
	}

	return 0;
}





void	CIniReader::ComputeLine(char *line)
{
char	Command[128];
char	Param[128];

		int len = strlen(line); 
		if (len > 1) 
		{ 
			if (line[len-1] == '\n') line[len-1] = 0; 
			if ((line[0] == '/') && (line[1] == '/')) return;
		}


		if (SplitLine(line,Command,Param) == -1) return;


		if (!strcmp(Command,"TITLE"))
		{ strncpy(m_Items[m_ItemsNum].Title, Param, 64); }
		else if (!strcmp(Command,"INFO"))
		{ strncpy(m_Items[m_ItemsNum].Info, Param, 64); }
		else if (!strcmp(Command,"GFX"))
		{ strncpy(m_Items[m_ItemsNum].Shot, Param, 64);	}
		else if (!strcmp(Command,"DIR"))
		{ strncpy(m_Items[m_ItemsNum].Dir, Param, 64);	}
		else if (!strcmp(Command,"SCREENSHOTS_POS"))
		{ SplitCoord(Param, &m_posX, &m_posY); }
		else if (!strcmp(Command,"SCREENSHOTS_SIZE"))
		{ SplitCoord(Param, &m_sizX, &m_sizY); }
		else if (!strcmp(Command,"TITLES_POS"))
		{ SplitCoord(Param, &m_options_posX, &m_options_posY); }
		else if (!strcmp(Command,"TITLES_ROWSIZE"))
		{ SplitCoord(Param, &m_options_sizX, &m_options_sizY); }
		else if (!strcmp(Command,"TITLES_ROWS"))
		{ SplitCoord(Param, &m_options_rows, 0); }


		if ((m_Items[m_ItemsNum].Title[0] != 0)
		&&  (m_Items[m_ItemsNum].Info[0] != 0)
		&&  (m_Items[m_ItemsNum].Shot[0] != 0)
		&&  (m_Items[m_ItemsNum].Dir[0] != 0) )
		{
			m_ItemsNum++;
		}


}

