//---------------------------------------------------------------------------
#ifndef Math3DH
#define Math3DH

#include "Types.h"
#include "rtlib2.h"
//#include <math.h>

//---------------------------------------------------------------------------

typedef struct {
        float _11, _12, _13, _14;
        float _21, _22, _23, _24;
        float _31, _32, _33, _34;
        float _41, _42, _43, _44;
} Matrix3D;


typedef struct {
        float X,Y,Z,W;
} Point3D;


typedef struct {
        int  l1,l2,l3;
        bool shown;
        int  val;
} Face3D;


void    Math3DInit(void);

void    MtxCopy(Matrix3D *Dst, Matrix3D *Src);
void    MtxInit(Matrix3D *Dst);

void    MtxSetRotation(Matrix3D *Mtx, Point3D *Ang);
void    MtxSetPosition(Matrix3D *Dst, Point3D *Pnt);
void    MtxSetCoords(Matrix3D *Mtx, Point3D *Pos, Point3D *Ang);
void    MtxMul(Matrix3D *a, Matrix3D *b);

void    MtxApplyToPoint(Point3D *Dst, Point3D *Src, Matrix3D *Mtx);
void    MtxApplyToPoint(Point3D *Dst, SBYTE *Src, Matrix3D *Mtx);



#endif
