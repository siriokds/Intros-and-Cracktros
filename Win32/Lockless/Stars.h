#include <stdlib.h>
#define NROW RESY

struct	Tstar
{
	int pos;
	int vel;
};

struct	Tstar	stars[NROW];

/*---------------------------------------------------------------------*/
void	star_init()
/*---------------------------------------------------------------------*/
{
int	i;

	for (i = 0; i < NROW; i++)
	{
		stars[i].pos = getRand() & 1023;
		stars[i].vel = getRand() & 3;
	}
}

/*---------------------------------------------------------------------*/
void	star_draw(char *dst, int pitch)
/*---------------------------------------------------------------------*/
{
int	i, offs = 0;

	for (i = 0; i < NROW; i++, offs += pitch)
	{
		if (	(stars[i].pos >= 0) && 
			(stars[i].pos < RESX+16) && 
			(!dst[offs+stars[i].pos])
			)
		{
			dst[offs+stars[i].pos] = stars[i].vel + 16;
		}

		stars[i].pos += ( stars[i].vel + 2 );
		stars[i].pos &= 1023;
	}
}
