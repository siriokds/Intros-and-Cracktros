#ifndef KSTARFIELD_H_
#define KSTARFIELD_H_

#define MAXSTARS	2000


class KStarFieldRecord
{
public:
	float m_X;
	float m_Y;
	float m_Z;
	int   m_Color;
	int   m_Size;
};


class KStarField
{
protected:
	
	int m_Width;
	int m_Height;
	
	int m_Left;
	int m_Right;
	int m_Top;
	int m_Bottom;

	int m_CenterX;
	int m_CenterY;
	float *m_sinTab;
	float *m_cosTab;

public:
	void Initialize(int sx, int sy, float *st, float *ct);
	void Update(void);

	int m_NumStars;
	float m_StarVelocityX;
	float m_StarVelocityY;
	float m_StarVelocityZ;

	int m_rotate;
	int m_color;

	KStarFieldRecord m_Stars[MAXSTARS];
	int	m_x[MAXSTARS];
	int	m_y[MAXSTARS];
};



#endif // KSTARFIELD_H_
