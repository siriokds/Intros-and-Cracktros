#include "Math3D.h"
#include "TriFiller.h"


#define  FUOCO     1.0
#define  XDIST     256
#define  YDIST     256
#define  XCNTR     256
#define  YCNTR     256



Point3D d[128];
Point3D s[128];

Matrix3D MtxFinal;
Matrix3D MtxObject;
Matrix3D MtxCamera;


Point3D   ObjPosition = { 0.0, 0.0, 0.0, 1.0 };
Point3D   ObjRotation = { 0.0, 0.0, 0.0, 1.0 };
Point3D   CamPosition = { 550.0, 0.0, -150.0, 1.0 };
Point3D   CamRotation = { 0.0, 0.0, 0.0, 1.0 };



typedef	struct
{
	int pnt;
	int	tri;
	SBYTE *pntd;
	SBYTE *trid;
} t_vletter;

t_vletter	vletters[64];


SBYTE	vletters_data[] = 
{
#include "vletters\\Letter_!extra_!.h"
#include "vletters\\Letter_!extra_plus.h"
#include "vletters\\Letter_!extra_minus.h"
#include "vletters\\Letter_!extra_point.h"
#include "vletters\\Letter_!extra_2point.h"
#include "vletters\\Letter_!extra_comma.h"
#include "vletters\\Letter_!extra_slash.h"
#include "vletters\\Letter_A.h"
#include "vletters\\Letter_B.h"
#include "vletters\\Letter_C.h"
#include "vletters\\Letter_D.h"
#include "vletters\\Letter_E.h"
#include "vletters\\Letter_F.h"
#include "vletters\\Letter_G.h"
#include "vletters\\Letter_H.h"
#include "vletters\\Letter_I.h"
#include "vletters\\Letter_J.h"
#include "vletters\\Letter_K.h"
#include "vletters\\Letter_L.h"
#include "vletters\\Letter_M.h"
#include "vletters\\Letter_N.h"
#include "vletters\\Letter_O.h"
#include "vletters\\Letter_P.h"
#include "vletters\\Letter_Q.h"
#include "vletters\\Letter_R.h"
#include "vletters\\Letter_S.h"
#include "vletters\\Letter_T.h"
#include "vletters\\Letter_U.h"
#include "vletters\\Letter_V.h"
#include "vletters\\Letter_W.h"
#include "vletters\\Letter_X.h"
#include "vletters\\Letter_Y.h"
#include "vletters\\Letter_Z.h"
#include "vletters\\Letter_0.h"
#include "vletters\\Letter_1.h"
#include "vletters\\Letter_2.h"
#include "vletters\\Letter_3.h"
#include "vletters\\Letter_4.h"
#include "vletters\\Letter_5.h"
#include "vletters\\Letter_6.h"
#include "vletters\\Letter_7.h"
#include "vletters\\Letter_8.h"
#include "vletters\\Letter_9.h"
};


char	vmatrix[] = "!+-.:,/ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789°";


/*------------------------------------------*/
int		char2code(char c)
/*------------------------------------------*/
{
int	founded = -1;
char *p = vmatrix;

	while ((*p != '°') && (*p != c)) p++;
	if (*p != '°') founded = p-vmatrix;

	return founded;
}




/*------------------------------------------*/
void	vfont_init(void)
/*------------------------------------------*/
{
SBYTE	*p = vletters_data;

	for (int i = 0; i < 43; i++)
	{
		vletters[i].pnt = (int)(*p++);
		vletters[i].pntd = p;
		p += 2 * vletters[i].pnt;
		vletters[i].tri = (int)(*p++);
		vletters[i].trid = p;
		p += 3 * vletters[i].tri;
	}

}


/*---------------------------------------------------------------*/
void    World2Screen(Point3D *pnt, Point3D *src)
/*---------------------------------------------------------------*/
{
double  zt;

   zt = src->Z*FUOCO + XCNTR;  if (zt == 0) zt = 1;
   pnt->X = (float)((src->X*XDIST) / zt);

   zt = src->Z*FUOCO + YCNTR;  if (zt == 0) zt = 1;
   pnt->Y = (float)((src->Y*YDIST) / zt);

   pnt->Z = src->Z;
}






/*-----------------------------------------------------------------*/
void	vfont_drawletter(int *set, int lt)
/*-----------------------------------------------------------------*/
{
t_vletter *vl = &vletters[lt];

int posX = 320;
int	posY = 240;


	for (int tri = 0; tri < vl->tri; tri++)
	{
		int p1 = vl->trid[(tri*3)+0];
		int p2 = vl->trid[(tri*3)+1];
		int p3 = vl->trid[(tri*3)+2];

		PutTriangle(set, 
			posX + int(s[p1].X*2), posY + int(s[p1].Y*2),
			posX + int(s[p2].X*2), posY + int(s[p2].Y*2),
			posX + int(s[p3].X*2), posY + int(s[p3].Y*2),
			-1);
	}
}





char	vtext[] = "    5.. 4.. 3.. 2.. 1.. GO!   TRISTAR AND RED SECTOR INC. "
				  "PROUDLY PRESENTS...  X-MEN2 +4 TRAINER.... "
				  "GAME CRACKED BY: - FAIRLIGHT -...   AND AS USUAL JUST RELEASED AND TRAINED BY SETH GECKO / TRSI... "
				  "  INTRO CODING BY SIRIOKD/TRSI...      w"
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";


int		vtext_pos = 0;
float	rotval = 700;
float	letcnt = 0;
int		texton = 0;
int		letpos = 0;
char	letters[] = "        ";


/*---------------------------------------------------------------------------------------*/
void    vfont_runletter(int *set, int lt, float rval, Point3D *obj_pos, Point3D *obj_rot)
/*---------------------------------------------------------------------------------------*/
{
		if (lt == -1) return;
		if (rval > 840) return;



	int	i;
		float ang = ((600-rval) * 0.003067961575771282459436175179f) + 5.02f;
		obj_pos->Z = (float)( 400 + (cos(ang) * 1050.0f));
		obj_pos->X = (float)(-100 + (sin(ang) * 500.0f));



        MtxSetCoords(&MtxCamera, &CamPosition, &CamRotation);
        MtxSetCoords(&MtxObject, obj_pos, obj_rot);

        MtxInit(&MtxFinal);
        MtxMul(&MtxFinal,&MtxObject);
        MtxMul(&MtxFinal,&MtxCamera);


        for (i = 0; i < vletters[lt].pnt; i++) MtxApplyToPoint(&d[i], vletters[lt].pntd + (i*2), &MtxFinal);
        for (i = 0; i < vletters[lt].pnt; i++) World2Screen(&s[i], &d[i]);

		vfont_drawletter(set, lt);

}


/*------------------------------------------------------------------------*/
float	rounding(float val, float max)
/*------------------------------------------------------------------------*/
{
	while (val > max) val -= max;
	while (val < 0)   val += max;
	return val;
}


/*------------------------------------------------------------------------*/
void    vfont_run(int *set)
/*------------------------------------------------------------------------*/
{

	vfont_runletter(set, char2code(letters[0]), rounding(rotval,    1050), &ObjPosition, &ObjRotation);
	vfont_runletter(set, char2code(letters[1]), rounding(rotval-150,1050), &ObjPosition, &ObjRotation);
	vfont_runletter(set, char2code(letters[2]), rounding(rotval-300,1050), &ObjPosition, &ObjRotation);
	vfont_runletter(set, char2code(letters[3]), rounding(rotval-450,1050), &ObjPosition, &ObjRotation);
	vfont_runletter(set, char2code(letters[4]), rounding(rotval-600,1050), &ObjPosition, &ObjRotation);
	vfont_runletter(set, char2code(letters[5]), rounding(rotval-750,1050), &ObjPosition, &ObjRotation);
	vfont_runletter(set, char2code(letters[6]), rounding(rotval-900,1050), &ObjPosition, &ObjRotation);

	rotval += 5 * scaleval;
	if (rotval > 750) { texton = 1; }


	if (texton)
	{
	
		letcnt += 5 * scaleval;
		if (letcnt > 150)
		{
			letcnt -= 150;
		
			if (vtext[vtext_pos] == 'w') vtext_pos = 0;
			letters[letpos] = vtext[vtext_pos++];
			letpos = (letpos + 1) % 7;
		}
	}


}


