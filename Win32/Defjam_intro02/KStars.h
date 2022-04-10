#ifndef KSTARFIELD_H_
#define KSTARFIELD_H_

#define MAXSTARS	2000
#include <windows.h>

class KStarFieldRecord
{
public:
	int m_X;
	int m_Y;
	int m_Z;

	signed short m_Color;
};


class KStarField
{
protected:
	int m_NumStars;
	
	int m_Width;
	int m_Height;
	
	int m_Left;
	int m_Right;
	int m_Top;
	int m_Bottom;

	int m_CenterX;
	int m_CenterY;

public:
	void Initialize(int sx, int sy);
	void Update(signed short *dst, int pitch);

	int m_StarVelocityX;
	int m_StarVelocityY;
	int m_StarVelocityZ;

	KStarFieldRecord m_Stars[MAXSTARS];
};



#endif // KSTARFIELD_H_
