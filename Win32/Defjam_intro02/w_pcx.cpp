#include <stdio.h>
#include <string.h>
#include "types.h"

struct	tPCX_Header
{
	char  Manufact;
	char  Version;
	char  Encoding;
	char  BitPerPixel;
	short Xmin,Ymin;
	short Xmax,Ymax;
	short Hres,Vres;
	char  Palette[48];
	char  Reserved;
	char  ColorPlanes;
	short BytesPerLine;
	short PaletteType;
	char  Filler[58];

};

struct	tPCX_Header PcxHeader;
char	*PCX_tempData;


/*------------------------------------------------------------------------*/
int		PCX_Size(char *S)
/*------------------------------------------------------------------------*/
{
int	size = -1;
	memcpy(&PcxHeader,S,sizeof(PcxHeader));
	if (PcxHeader.Manufact == 10)
	{
		size = 	((PcxHeader.Xmax-PcxHeader.Xmin)+1) * 
			((PcxHeader.Ymax-PcxHeader.Ymin)+1);
	}
	return size;
}




/*------------------------------------------------------------------------*/
void	PCX_Decode(char *S, SWORD *F)
/*------------------------------------------------------------------------*/
{
int 	total, total2, n;
char	val,size;
char	*_D, *D, *PAL;

	if ((total = PCX_Size(S)) == -1) return;
	
	D = _D = PCX_tempData = (char*)malloc(total+768);

	if (D == NULL) return;

	PAL = PCX_tempData+total;

	total2 = total;
	
	S += 128;
	
	do {
		val = *S++;
		if ((val & 0xC0) == 0xC0)
		{
			size = val & 0x3F;
			val  = *S++;
			memset(D,val,size);
			D += size;
			total -= size;
		} else
		{
			*D++ = val;
			total--;
		}

	} while (total);


	for (n = 0; n < 768; n++) *(PAL++) = *(++S);

	
	PAL = PCX_tempData+total2;
	
	int r,g,b,c;

	while (total2--)
	{
		c = *_D++;
		
		r = (PAL[(c*3)+0] >> 4) & 15;
		g = (PAL[(c*3)+1] >> 4) & 15;
		b = (PAL[(c*3)+2] >> 4) & 15;
		
		*F++ = ((SWORD)((r<<12)|(g<<7)|(b<<1)));
	}

	free(PCX_tempData);
}

