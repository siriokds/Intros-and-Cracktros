#ifndef	__CINIREADER_H__
#define	__CINIREADER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	CINI_MAXTITLES	64
#define	CINI_MAXTEXT	64


class CItem
{
public:
	char Title[CINI_MAXTEXT];
	char Info[CINI_MAXTEXT];
	char Shot[CINI_MAXTEXT];
	char Dir[CINI_MAXTEXT];

	void	Reset(void)
	{
		memset(Title, 0, CINI_MAXTEXT);
		memset(Info,  0, CINI_MAXTEXT);
		memset(Shot,  0, CINI_MAXTEXT);
		memset(Dir,   0, CINI_MAXTEXT);
		
	}

	CItem()
	{
		Reset();
	}
};


class CIniReader
{
public:
	int	 m_posX;
	int  m_posY;
	int  m_sizX;
	int  m_sizY;
	int  m_alpha;


	int	 m_options_posX;
	int	 m_options_posY;
	int	 m_options_sizX;
	int	 m_options_sizY;
	int	 m_options_rows;

	CItem *	m_Items;
	int		m_ItemsNum;

	 CIniReader(void);
	~CIniReader();

	void Reset(void);
	void ReadIni(char *fname);
	int	 SplitLine(char *line, char *command, char *param);
	int	 SplitCoord(char *line, int *x, int *y);
	void ComputeLine(char *line);

};


#endif	//__CINIREADER_H__

