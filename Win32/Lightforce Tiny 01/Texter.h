//---------------------------------------------------------------------------
#ifndef TexterH
#define TexterH
//---------------------------------------------------------------------------
#include "Windows.h"
#include "FastBMP.h"
#include "Mtx.h"

	
class	TTexter {

private:
	TMtx		*InfoMtx;

	TFastBMP	*Font;
	int			FontWidth, FontHeight;
	int			NumCharX, NumCharY;

	char		*Matrix;

	char		**Pages;
	int			PagePos;

	char		*Text;
	int			TextPos;

	TFastBMP	*Frame;
	int			NumCharXD, NumCharYD;

	int			CursorX, CursorY;
	int			Waiting;

	int			GetMatrixPos(char c);

public:
	~TTexter();
	TTexter(TFastBMP *fnt, TFastBMP *frm, char *mtx, char **pgs, TMtx *imtx = NULL, int fntW = 8, int fntH = 8);

	void		CursorIncX(void);
	void		CursorIncY(void);

	void		CharCopy(TFastBMP *Dst, int DstX, int DstY, char c);

	char		GetNextChar(void);
	void		ComputeChar(char c);
	void		CharWrite(char c);

	void		RunOnce(void);

};



#endif
