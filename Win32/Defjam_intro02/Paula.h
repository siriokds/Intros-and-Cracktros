//---------------------------------------------------------------------------
#ifndef PaulaH
#define PaulaH
//---------------------------------------------------------------------------


#include <windows.h>
#include "Types.h"
#include "SoundServer.h"

typedef int (*PFUNC)(void);


void	paulaOpen(PFUNC UserFunc, int UserFuncRate);
void	paulaStart(void);
void	paulaClose(void);

void	paulaSetRate(int rate);
void	paulaReset(void);
void	paulaSetFreq(int voc, int freq);
void	paulaSetPeriod(int voc, int period);
void	paulaSetVolume(int voc, int vol);
void	paulaSetSamplePos(int voc, SBYTE *Address);
void	paulaSetSampleLen(int voc, UWORD Length);
void	paulaPlayVoice(int voc);
void	paulaStopVoice(int voc);
void	paulaVoice2(int voc, int md, SBYTE *address, ULONG loopSta, ULONG loopEnd);
void	paulaWriteDmaCon(UWORD amigaWord);
void	paulaUpdate(SWORD *dst, int nbSampleToCompute);




#define MIXFREQ		44100
#define FRACBITS	12

#define	NUMCHN		4
#define	MAXVOL		64
#define VOLTABLEN	((MAXVOL+1) * 256 * sizeof(SWORD))
#define Period2Freq(p) (3546895/period) // Amiga PAL  (50Hz)

//#define Period2Freq(p) (3579545/period) // Amiga NTSC (60Hz)






#endif
