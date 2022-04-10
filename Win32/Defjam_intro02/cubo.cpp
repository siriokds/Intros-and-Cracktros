
#define BOX    45.0


#define POINTS 14
Point3D CubeVerts[POINTS] =
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


Point3D d[POINTS];
Point3D s[POINTS];

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



DWORD  faceCol[24] =
{
        16,16,16,16,
        8,8,8,8,
        0,0,0,0,
        16,16,16,16,
        8,8,8,8,
        0,0,0,0
};


typedef struct {
        int r,g,b;
} RGBq;

#define RGB2COL(rgb) ((rgb.b<<16)|(rgb.g<<8)|rgb.r)

RGBq    faceC[24] =
{
        { 0,255,255 },
        { 0,255,255 },
        { 0,255,255 },
        { 0,255,255 },

        { 0,255,255 },
        { 0,255,255 },
        { 0,255,255 },
        { 0,255,255 },

        { 0,255,255 },
        { 0,255,255 },
        { 0,255,255 },
        { 0,255,255 },


        { 255,255,0 },
        { 255,255,0 },
        { 255,255,0 },
        { 255,255,0 },

        { 255,255,0 },
        { 255,255,0 },
        { 255,255,0 },
        { 255,255,0 },

        { 255,255,0 },
        { 255,255,0 },
        { 255,255,0 },
        { 255,255,0 }
};

RGBq    faceB[24] =
{
        { 0,255,255 },
        { 255,255,0 },
        { 0,255,255 },
        { 255,255,0 },

        { 0,255,0 },
        { 255,0,0 },
        { 0,255,0 },
        { 255,0,0 },

        { 0,0,255 },
        { 255,0,255 },
        { 0,0,255 },
        { 255,0,255 },

        { 0,255,255 },
        { 255,255,0 },
        { 0,255,255 },
        { 255,255,0 },

        { 0,255,0 },
        { 255,0,0 },
        { 0,255,0 },
        { 255,0,0 },

        { 0,0,255 },
        { 255,0,255 },
        { 0,0,255 },
        { 255,0,255 },

};





#define  FUOCO     1.0

#define  XDIST     256
#define  YDIST     256

#define  XCNTR     256
#define  YCNTR     256



Matrix3D MtxFinal;

Matrix3D MtxObject;
Matrix3D MtxCamera;


Point3D   ObjPosition = { 0.0, 0.0, 0.0, 1.0 };
Point3D   ObjRotation = { 0.0, 0.0, 0.0, 1.0 };
Point3D   CamPosition = { 0.0, 0.0, 0.0, 1.0 };
Point3D   CamRotation = { 0.0, 0.0, 0.0, 1.0 };


float     IncX = 3, IncY = 2, IncZ = 4;







/*---------------------------------------------------------------*/
void    World2Screen(Point3D *pnt, Point3D *src)
/*---------------------------------------------------------------*/
{
int     zt;

   zt = src->Z*FUOCO + XCNTR;  if (zt == 0) zt = 1;
   pnt->X = ((src->X*XDIST) / zt);

   zt = src->Z*FUOCO + YCNTR;  if (zt == 0) zt = 1;
   pnt->Y = ((src->Y*YDIST) / zt);

   pnt->Z = src->Z;
}



/*-----------------------------------------------------------------*/
void    FaceVisible(int i)
/*-----------------------------------------------------------------*/
{
int     v1,w1,v2,w2;

        v1 = s[face[i].l2].X - s[face[i].l1].X;
        w1 = s[face[i].l3].X - s[face[i].l1].X;

        v2 = s[face[i].l2].Y - s[face[i].l1].Y;
        w2 = s[face[i].l3].Y - s[face[i].l1].Y;

        face[i].val = -(((v1 * w2) - (v2 * w1)) / (48*FUOCO));
        face[i].shown = face[i].val > 0;

}




/*-----------------------------------------------------------------*/
void	DrawCube(void)
/*-----------------------------------------------------------------*/
{
Point3D *p3d;
int     StartX = Image1->Width / 2;
int     StartY = Image1->Height / 2;

TPoint  pt[3];
DWORD   r,g,b;
SWORD   col1,col2;


        for (int i = 0; i < FACES; i++)
        {
                p3d = &s[ face[i].l1 ];
                pt[0].x = StartX + p3d->X;
                pt[0].y = StartY + (-p3d->Y);

                p3d = &s[ face[i].l2 ];
                pt[1].x = StartX + p3d->X;
                pt[1].y = StartY + (-p3d->Y);

                p3d = &s[ face[i].l3 ];
                pt[2].x = StartX + p3d->X;
                pt[2].y = StartY + (-p3d->Y);


                if (face[i].shown == TRUE)
                {
//                  Image1->Canvas->Brush->Color = faceCol[i];

                  r = (faceC[i].r*face[i].val)/256/16;
                  g = (faceC[i].g*face[i].val)/256/16;
                  b = (faceC[i].b*face[i].val)/256/16;

                  col1 = ((SWORD)((r<<12)|(g<<7)|(b<<1)));

                  r /= 2; g /= 2; b /= 2;
                  col2 = ((SWORD)((r<<12)|(g<<7)|(b<<1)));

                  Image1->Canvas->Pen->Color = TColor(col2);
                  Image1->Canvas->Brush->Color = TColor(col1);
/*
                  Image1->Canvas->Pen->Color = TColor((zt/2) << faceCol[i]);
                  Image1->Canvas->Brush->Color = TColor(zt << faceCol[i]);
*/
                  Image1->Canvas->Polygon(pt, 2);
                }


                Image1->Canvas->Pixels[pt[0].x][pt[0].y] = clWhite;

                Image1->Canvas->Pixels[pt[1].x][pt[1].y] = clWhite;

                Image1->Canvas->Pixels[pt[2].x][pt[2].y] = clWhite;

        }
}




/*-----------------------------------------------------------------*/
void    RunOnce(void)
/*-----------------------------------------------------------------*/
{
RGBq    BackG = { 20,127,127 };

        Image1->Canvas->Brush->Color = TColor(RGB2COL(BackG));
        Image1->Canvas->FillRect(TRect(0,0,Image1->Width,Image1->Height));


        MtxSetCoords(&MtxObject, &ObjPosition, &ObjRotation);
        MtxSetCoords(&MtxCamera, &CamPosition, &CamRotation);

        MtxInit(&MtxFinal);
        MtxMul(&MtxFinal,&MtxObject);
        MtxMul(&MtxFinal,&MtxCamera);


        for (int i = 0; i < POINTS; i++)
        {
                MtxApplyToPoint(&d[i], &CubeVerts[i], &MtxFinal);
        }

        for (int i = 0; i < POINTS; i++) World2Screen(&s[i], &d[i]);
        for (int i = 0; i < FACES; i++)  FaceVisible(i);


        DrawCube(Sender);

        if ((ObjRotation.X = ObjRotation.X + IncX) >= 360) ObjRotation.X -= 360;
        if ((ObjRotation.Y = ObjRotation.Y + IncY) >= 360) ObjRotation.Y -= 360;
        if ((ObjRotation.Z = ObjRotation.Z + IncZ) >= 360) ObjRotation.Z -= 360;

//        if ((CamRotation.X = CamRotation.X + IncX) >= 360) CamRotation.X -= 360;
//        if ((CamPosition.X = CamPosition.X + IncX) >= 360) CamPosition.X -= 360;


        Label1->Caption = FloatToStr(ObjRotation.X);
        Label2->Caption = FloatToStr(ObjRotation.Y);
        Label3->Caption = FloatToStr(ObjRotation.Z);

        Label4->Caption = IntToStr(face[0].val);
        Label5->Caption = IntToStr(face[1].val);
}
