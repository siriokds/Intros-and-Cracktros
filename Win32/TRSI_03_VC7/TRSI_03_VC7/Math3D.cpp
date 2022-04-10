//---------------------------------------------------------------------------

#include "Math3D.h"

#include <windows.h>
//---------------------------------------------------------------------------





Matrix3D IdentityMatrix =
{
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
};

float   SinTable[36000+9000];
float   *CosTable;


/*--------------------------------------------------------------------*/
inline float ANG2RAD(float ang)
/*--------------------------------------------------------------------*/
{
	return (ang*0.017453292519943295769236907684886f);
}


/*--------------------------------------------------------------------*/
void    Math3DInit(void)
/*--------------------------------------------------------------------*/
{

float   rad;
		for (int i = 0; i < 36000; i++)
        {
                rad = (float)(ANG2RAD(float(i))*0.01f);
                SinTable[i] = (float)sin(rad);
        }
		CosTable = SinTable + 9000;

/*
	SinTable[0] = 0.017453292519943295769236907684886f;

	__asm
	{
		pushad

		finit
		lea		edi,SinTable
		mov		ecx,36000+9000

		fld		[edi]				// (inc)
		fldz						// (ang)(inc)
calcTab:
		fld		st(0)				// (ang)(ang)(inc)
		fsin						// (anS)(ang)(inc)
		fstp	[edi]				// (ang)(inc) put value and pop!
		add		edi,4
		fadd	st,st(1)			// (anN)(PIi)(256)
		loop	calcTab

		finit

		popad
	}
*/

}


/*----------------------------------------------------------------*/
void 	fastCpy(UBYTE *dst, UBYTE *src, int todo)
/*----------------------------------------------------------------*/
{
	_asm
	{
		pushad
		mov		edi,dst
		mov		esi,src
		mov		ecx,todo
		push	ecx
		shr		ecx,2
		cld
		rep		movsd
		pop		ecx
		and		ecx,3
		rep		movsb
		popad
	}
}


/*--------------------------------------------------------------------*/
void    MtxCopy(Matrix3D *Dst, Matrix3D *Src)
/*--------------------------------------------------------------------*/
{
        fastCpy((unsigned char*)Dst,(unsigned char*)Src,sizeof(Matrix3D));
}

/*--------------------------------------------------------------------*/
void    MtxInit(Matrix3D *Dst)
/*--------------------------------------------------------------------*/
{
        MtxCopy(Dst, &IdentityMatrix);
}


/*--------------------------------------------------------------------*/
void    MtxSetPosition(Matrix3D *Dst, Point3D *Pnt)
/*--------------------------------------------------------------------*/
{
        Dst->_41 = Pnt->X;
        Dst->_42 = Pnt->Y;
        Dst->_43 = Pnt->Z;
}


/*--------------------------------------------------------------------*/
void    MtxSetRotation(Matrix3D *Mtx, Point3D *Ang)
/*--------------------------------------------------------------------*/
{
float   sinX,cosX,sinY,cosY,sinZ,cosZ;

        sinX = (float)sin(ANG2RAD(Ang->X));  cosX = (float)cos(ANG2RAD(Ang->X));
        sinY = (float)sin(ANG2RAD(Ang->Y));  cosY = (float)cos(ANG2RAD(Ang->Y));
        sinZ = (float)sin(ANG2RAD(Ang->Z));  cosZ = (float)cos(ANG2RAD(Ang->Z));
/*
int offset;

		offset = int(Ang->X*100);
        sinX = SinTable[offset];  cosX = CosTable[offset];

		offset = int(Ang->Y*100);
        sinY = SinTable[offset];  cosY = CosTable[offset];

		offset = int(Ang->Z*100);
        sinZ = SinTable[offset];  cosZ = CosTable[offset];
*/

        Mtx->_11 = sinZ*sinY*sinX + cosZ*cosX;
        Mtx->_12 = cosY*sinX;
        Mtx->_13 = sinZ*cosX - cosZ*sinY*sinX;

        Mtx->_21 = sinZ*sinY*cosX - cosZ*sinX;
        Mtx->_22 = cosY*cosX;
        Mtx->_23 = -cosZ*sinY*cosX - sinZ*sinX;

        Mtx->_31 = -sinZ*cosY;
        Mtx->_32 = sinY;
        Mtx->_33 = cosZ*cosY;
}

/*--------------------------------------------------------------------*/
void    MtxSetCoords(Matrix3D *Mtx, Point3D *Pos, Point3D *Ang)
/*--------------------------------------------------------------------*/
{
        MtxInit(Mtx);
        MtxSetPosition(Mtx, Pos);
        MtxSetRotation(Mtx, Ang);
}


/*-------------------------------------------------------------------------------------*/
void    MtxApplyToPoint(Point3D *Dst, Point3D *Src, Matrix3D *Mtx)
/*-------------------------------------------------------------------------------------*/
{
        Dst->X = (Src->X * Mtx->_11) + (Src->Y * Mtx->_21) + (Src->Z * Mtx->_31) + Mtx->_41;
        Dst->Y = (Src->X * Mtx->_12) + (Src->Y * Mtx->_22) + (Src->Z * Mtx->_32) + Mtx->_42;
        Dst->Z = (Src->X * Mtx->_13) + (Src->Y * Mtx->_23) + (Src->Z * Mtx->_33) + Mtx->_43;
}


/*-------------------------------------------------------------------------------------*/
void    MtxApplyToPoint(Point3D *Dst, SBYTE *Src, Matrix3D *Mtx)
/*-------------------------------------------------------------------------------------*/
{
        Dst->X = (float(Src[0]) * Mtx->_11) 
			   + (float(Src[1]) * Mtx->_21) 
			   + Mtx->_31 + Mtx->_41;

        Dst->Y = (float(Src[0]) * Mtx->_12) 
			   + (float(Src[1]) * Mtx->_22) 
			   + Mtx->_32 + Mtx->_42;

        Dst->Z = (float(Src[0]) * Mtx->_13) 
			   + (float(Src[1]) * Mtx->_23) 
			   + Mtx->_33 + Mtx->_43;

}



/*--------------------------------------------------------------------*/
void    MtxMul(Matrix3D *a, Matrix3D *b)
/*--------------------------------------------------------------------*/
{
Matrix3D temp;

  temp._11 = a->_11 * b->_11 + a->_12 * b->_21 + a->_13 * b->_31 + a->_14 * b->_41;
  temp._12 = a->_11 * b->_12 + a->_12 * b->_22 + a->_13 * b->_32 + a->_14 * b->_42;
  temp._13 = a->_11 * b->_13 + a->_12 * b->_23 + a->_13 * b->_33 + a->_14 * b->_43;
  temp._14 = a->_11 * b->_14 + a->_12 * b->_24 + a->_13 * b->_34 + a->_14 * b->_44;

  temp._21 = a->_21 * b->_11 + a->_22 * b->_21 + a->_23 * b->_31 + a->_24 * b->_41;
  temp._22 = a->_21 * b->_12 + a->_22 * b->_22 + a->_23 * b->_32 + a->_24 * b->_42;
  temp._23 = a->_21 * b->_13 + a->_22 * b->_23 + a->_23 * b->_33 + a->_24 * b->_43;
  temp._24 = a->_21 * b->_14 + a->_22 * b->_24 + a->_23 * b->_34 + a->_24 * b->_44;

  temp._31 = a->_31 * b->_11 + a->_32 * b->_21 + a->_33 * b->_31 + a->_34 * b->_41;
  temp._32 = a->_31 * b->_12 + a->_32 * b->_22 + a->_33 * b->_32 + a->_34 * b->_42;
  temp._33 = a->_31 * b->_13 + a->_32 * b->_23 + a->_33 * b->_33 + a->_34 * b->_43;
  temp._34 = a->_31 * b->_14 + a->_32 * b->_24 + a->_33 * b->_34 + a->_34 * b->_44;

  temp._41 = a->_41 * b->_11 + a->_42 * b->_21 + a->_43 * b->_31 + a->_44 * b->_41;
  temp._42 = a->_41 * b->_12 + a->_42 * b->_22 + a->_43 * b->_32 + a->_44 * b->_42;
  temp._43 = a->_41 * b->_13 + a->_42 * b->_23 + a->_43 * b->_33 + a->_44 * b->_43;
  temp._44 = a->_41 * b->_14 + a->_42 * b->_24 + a->_43 * b->_34 + a->_44 * b->_44;

  MtxCopy(a,&temp);
}

