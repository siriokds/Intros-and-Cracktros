#include "KStars.h"
#include <stdlib.h>


#define	  RGB16(r,g,b) ((signed short)((r<<12)|(g<<7)|(b<<1)))	// USES 4bit for R for G and B (0~15)

/*-----------------------------------------------------------------------------------------*/
void KStarField::Initialize(int sx, int sy)
/*-----------------------------------------------------------------------------------------*/
{
	m_NumStars = 400;
	m_StarVelocityX = 0;
	m_StarVelocityY = 2;
	m_StarVelocityZ = 4;

	m_Width  = sx;
	m_Height = sy;
	
	m_Top = 0;
	m_Left = 0;
	m_Right = m_Width;
	m_Bottom = m_Height;
	
	m_CenterX = (m_Right - m_Left) / 2;
	m_CenterY = (m_Bottom - m_Top) / 2;
	


	for(int i = 0; i < m_NumStars; i++)
	{
		m_Stars[i].m_X = rand()%m_Width - (m_Width / 2);
		m_Stars[i].m_Y = rand()%m_Height - (m_Height / 2);
		m_Stars[i].m_Z = rand()%255;

	}
}




/*-----------------------------------------------------------------------------------------*/
void KStarField::Update(signed short *dst, int pitch)
/*-----------------------------------------------------------------------------------------*/
{
int newx, newy, offset;
KStarFieldRecord *p;
int col;

	for(int i = 0; i < m_NumStars; i++)
	{
		p = &m_Stars[i];
		if (p->m_Z < 1) p->m_Z = 255; if (p->m_Z > 255) p->m_Z = 1;
		if (p->m_X < -(m_Width/2)) p->m_X = (m_Width/2); 
		if (p->m_X > (m_Width/2)) p->m_X = -(m_Width/2);
		if (p->m_Y < -(m_Height/2)) p->m_Y = (m_Height/2); 
		if (p->m_Y > (m_Height/2)) p->m_Y = -(m_Height/2);

		newx = ((p->m_X * 128) / p->m_Z) + m_CenterX;
		newy = ((p->m_Y * 128) / p->m_Z) + m_CenterY;
			
		if((newx > m_Left) && (newx < m_Right) && 
		   (newy > m_Top) && (newy < m_Bottom))
		{
			col = (16-(p->m_Z/16))&15;

			m_Stars[i].m_Color = RGB16(col,col,col);

			offset = (newy * pitch) + newx;

			if (dst[offset]==0) dst[offset] = p->m_Color;
			if (dst[offset+1]==0) dst[offset+1] = p->m_Color;
		}

		p->m_X += m_StarVelocityX;
		p->m_Y += m_StarVelocityY;
		p->m_Z -= m_StarVelocityZ;
	}

}

