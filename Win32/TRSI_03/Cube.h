#include "Math3D.h"
//#include <stdio.h>


#define  BOX	   80.0

#define  FUOCO     1.0

#define  XDIST     256
#define  YDIST     256

#define  XCNTR     256
#define  YCNTR     256

#define  TRUE      1




#define CUBEPOINTS 14
Point3D CubeVerts[CUBEPOINTS] =
{
         { -BOX,-BOX,-BOX, 1.0 }
        ,{  BOX,-BOX,-BOX, 1.0 }
        ,{  BOX, BOX,-BOX, 1.0 }
        ,{ -BOX, BOX,-BOX, 1.0 }

        ,{ -BOX,-BOX, BOX, 1.0 }
        ,{  BOX,-BOX, BOX, 1.0 }
        ,{  BOX, BOX, BOX, 1.0 }
        ,{ -BOX, BOX, BOX, 1.0 }

        ,{ 0, 0, -(BOX*1.7), 1.0 }
        ,{ 0, 0,  (BOX*1.7), 1.0 }
        ,{ -(BOX*1.7), 0, 0, 1.0 }
        ,{  (BOX*1.7), 0, 0, 1.0 }
        ,{ 0, -(BOX*1.7), 0, 1.0 }
        ,{ 0,  (BOX*1.7), 0, 1.0 }
};


#define FACES 12
Face3D  face[12] =
{
         { 0, 2, 1, TRUE, 0}
        ,{ 0, 3, 2, TRUE, 0}

        ,{ 4, 5, 6, TRUE, 0}
        ,{ 4, 6, 7, TRUE, 0}



        ,{ 1, 6, 5, TRUE, 0}
        ,{ 1, 2, 6, TRUE, 0}

        ,{ 3, 4, 7, TRUE, 0}
        ,{ 3, 0, 4, TRUE, 0}


        ,{ 2, 7, 6, TRUE, 0}
        ,{ 2, 3, 7, TRUE, 0}


        ,{ 0, 5, 4, TRUE, 0}
        ,{ 0, 1, 5, TRUE, 0}
};

/*
DWORD  faceCol[24] =
{
        16,16,16,16,
        8,8,8,8,
        0,0,0,0,
        16,16,16,16,
        8,8,8,8,
        0,0,0,0
};
*/



Point3D d[CUBEPOINTS];
Point3D s[CUBEPOINTS];

Matrix3D MtxFinal;
Matrix3D MtxObject;
Matrix3D MtxCamera;


Point3D   ObjPosition = { 0.0, 0.0, 0.0, 1.0 };
Point3D   ObjRotation = { 0.0, 0.0, 0.0, 1.0 };
Point3D   CamPosition = { 0.0, 0.0, 0.0, 1.0 };
Point3D   CamRotation = { 0.0, 0.0, 0.0, 1.0 };


float     IncX = 0.75, IncY = 0.5,  IncZ = 1;

//float     IncX = 0, IncY = 0, IncZ = 0.25;




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
void    FaceVisible(Point3D *pnt, int i)
/*-----------------------------------------------------------------*/
{
float   v1,w1,v2,w2;

        v1 = pnt[face[i].l2].X - pnt[face[i].l1].X;
        w1 = pnt[face[i].l3].X - pnt[face[i].l1].X;

        v2 = pnt[face[i].l2].Y - pnt[face[i].l1].Y;
        w2 = pnt[face[i].l3].Y - pnt[face[i].l1].Y;

        face[i].val = (int)(-(((v1 * w2) - (v2 * w1)) / (48*FUOCO)));
        face[i].shown = face[i].val > 0;

}




/*-----------------------------------------------------------------*/
void	DrawCube(int *set)
/*-----------------------------------------------------------------*/
{

//		PutTriangle(set, 10,10, 10,50, 50,50,4);	return;	
	
Point3D *p3d;
float   StartX = 320;
float   StartY = 240;

int		pt_x[3];
int		pt_y[3];


//DWORD   r,g,b;
//DWORD   col1,col2;

int		clo[4] = { 3, 4, 5, 6 };


        for (int i = 0; i < FACES; i++)
        {
                p3d = &s[ face[i].l1 ];
                pt_x[0] = StartX + p3d->X;
                pt_y[0] = StartY + (-p3d->Y);

                p3d = &s[ face[i].l2 ];
                pt_x[1] = StartX + p3d->X;
                pt_y[1] = StartY + (-p3d->Y);

                p3d = &s[ face[i].l3 ];
                pt_x[2] = StartX + p3d->X;
                pt_y[2] = StartY + (-p3d->Y);


                if (face[i].shown == TRUE)
                {

/*
				r = (faceC[i].r*face[i].val)/256;
                  g = (faceC[i].g*face[i].val)/256;
                  b = (faceC[i].b*face[i].val)/256;
                  col1 = (b << 16) | (g << 8) | r;

                  Image1->Canvas->Brush->Color = TColor(col1);
                  Image1->Canvas->Polygon(pt, 2);
*/
					int val = face[i&0xFFFFFFFE].val;

				  int col = 3 + ((val >> 8) & 7);
				 
				PutTriangle(set, pt_x[0], pt_y[0], pt_x[1], pt_y[1], pt_x[2], pt_y[2], col);

                }


        }
}



void    RunCube(int *set)
{
int	i;

//RGBq    BackG = { 20,127,127 };
//        Image1->Canvas->Brush->Color = TColor(RGB2COL(BackG));
//        Image1->Canvas->FillRect(TRect(0,0,Image1->Width,Image1->Height));


        MtxSetCoords(&MtxObject, &ObjPosition, &ObjRotation);
        MtxSetCoords(&MtxCamera, &CamPosition, &CamRotation);

        MtxInit(&MtxFinal);
        MtxMul(&MtxFinal,&MtxObject);
        MtxMul(&MtxFinal,&MtxCamera);


        for (i = 0; i < CUBEPOINTS; i++)
        {
                MtxApplyToPoint(&d[i], &CubeVerts[i], &MtxFinal);
        }

        for (i = 0; i < CUBEPOINTS; i++) World2Screen(&s[i], &d[i]);
        for (i = 0; i < FACES; i++)  FaceVisible(s, i);


        DrawCube(set);

        if ((ObjRotation.X = ObjRotation.X + IncX) >= 360) ObjRotation.X -= 360;
        if ((ObjRotation.Y = ObjRotation.Y + IncY) >= 360) ObjRotation.Y -= 360;
        if ((ObjRotation.Z = ObjRotation.Z + IncZ) >= 360) ObjRotation.Z -= 360;

//        if ((CamRotation.X = CamRotation.X + IncX) >= 360) CamRotation.X -= 360;
//        if ((CamPosition.X = CamPosition.X + IncX) >= 360) CamPosition.X -= 360;
}

