#include "KStars.h"
#include <stdlib.h>


/*-----------------------------------------------------------------------------------------*/
void KStarField::Initialize(int sx, int sy, float *sintab, float *costab)
/*-----------------------------------------------------------------------------------------*/
{
	m_NumStars = 128;
	m_StarVelocityX = 0;
	m_StarVelocityY = 0;
	m_StarVelocityZ = 1;

	m_sinTab = sintab;
	m_cosTab = costab;
	m_rotate = 0;

	m_Width  = sx;
	m_Height = sy;
	
	m_Top = 0;
	m_Left = 0;
	m_Right = m_Width;
	m_Bottom = m_Height;
	
	m_CenterX = (m_Right - m_Left) >> 1;
	m_CenterY = (m_Bottom - m_Top) >> 1;
	


	for(int i = 0; i < m_NumStars; i++)
	{
		m_Stars[i].m_X = (float)(rand()%m_Width - (m_Width / 2));
		m_Stars[i].m_Y = (float)(rand()%m_Height - (m_Height / 2));
		m_Stars[i].m_Z = (float)(rand()%255);
		m_Stars[i].m_Color = m_color;
		m_Stars[i].m_Size = rand() & 3;
	}
}




/*-----------------------------------------------------------------------------------------*/
void KStarField::Update(void)
/*-----------------------------------------------------------------------------------------*/
{
KStarFieldRecord *p;


	
	while (m_rotate < 0) m_rotate += 4096;
	m_rotate %= 4096;
	float rotSin = m_sinTab[ m_rotate ];
	float rotCos = m_cosTab[ m_rotate ];


	for(int i = 0; i < m_NumStars; i++)
	{
		p = &m_Stars[i];
		if (p->m_Z < 1) p->m_Z = 255; 


		float mX = p->m_X;
		float mY = p->m_Y;

	    float tX = (mX * rotCos) - (mY * rotSin);
		float tY = (mX * rotSin) + (mY * rotCos);
		mX = tX; 
		mY = tY;


		m_x[i] = (int) ( ((mX * 128) / p->m_Z) ) + m_CenterX;
		m_y[i] = (int) ( ((mY * 128) / p->m_Z) ) + m_CenterY;

		p->m_X += m_StarVelocityX;
		p->m_Y += m_StarVelocityY;
		p->m_Z += m_StarVelocityZ;

		if (p->m_Z > 255) 
		{
			p->m_Z = 1;
			p->m_X = (float)(rand()%m_Width - (m_Width  / 2));
			p->m_Y = (float)(rand()%m_Height - (m_Height / 2));
			p->m_Color = m_color;
			p->m_Size = rand() & 3;
		}

		


	}


	m_rotate -= 6;
}

