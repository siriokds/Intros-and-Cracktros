#include "Paula.h"

#define PAULA_BUFSIZE_MAX 16384

#define	CHANSIZE		(sizeof(KMixChn)*NUMCHN)
#define	VOLTSIZE		(VOLTABLEN*sizeof(SWORD))
#define	MIXBSIZE		(PAULA_BUFSIZE_MAX*sizeof(SWORD))
#define LOCALSIZE		(CHANSIZE+VOLTSIZE+MIXBSIZE+256)



typedef struct
{
	UBYTE	Num;

	BOOL	Active;
	UBYTE	MasterVol;

	ULONG	SampleLen, SampleLen_i;
	SBYTE	*SamplePos, *SamplePos_i;

	ULONG	PosIndex;

	SLONG	Freq;
	UBYTE	Volume;

} KMixChn;


BOOL	Filter = FALSE;



SBYTE	*LocalMem;

KMixChn	*MixChn;
SWORD	*VolTab;
SWORD	*LocalBuffer;
SBYTE	*nullBytes;

BOOL	Opened = FALSE;
SLONG	MixingFrequency = MIXFREQ;
PFUNC	UserFunction;
SLONG	UserFunctionRate;	//(50)

SLONG	nbSample;
SLONG	innerSampleCount;

CSoundServer *pServer;




/*-----------------------------------------------------------------------*/
int	ValidVoice(int voc)
/*-----------------------------------------------------------------------*/
{
	return ((voc >= 0) && (voc < NUMCHN));
}


/*-----------------------------------------------------------------------*/
void	FirMemory(SWORD *d, int len)
/*-----------------------------------------------------------------------*/
{
	for (int j = 1; j < len-1; j++)
	{
		d[j] = (d[j-2] / 4) + (d[j] / 2) + (d[j+2] / 4);

	}
}


/*-----------------------------------------------------------------------*/
void	InitVolTab(void)
/*-----------------------------------------------------------------------*/
{
SWORD	*pVolTab = VolTab;
int		vol,byt;


		for (byt = 0; byt < 256; byt++) *pVolTab++ = 0;

		for (vol = 1; vol < (MAXVOL+1); vol++, pVolTab += 256)
		{
  			for (byt = -128; byt < 128; byt++)
			  pVolTab[(UBYTE)byt] = (byt * vol * 256) / MAXVOL / NUMCHN;
		}
}




/*---------------------------------------------------------------------------------------------------------------------*/
SLONG	MixMonoNormal(SBYTE *src, SWORD *dst, ULONG idx, ULONG inc, ULONG todo, SWORD *Vol_L)
/*---------------------------------------------------------------------------------------------------------------------*/
{
UBYTE	sample1, sample2, sample3, sample4;
ULONG	remain;

	if ((src == NULL) || (dst == NULL) || (Vol_L == NULL) ) return idx;


    remain = todo & 3;

    for(todo>>=2; todo; todo--)
    {
        sample1 = src[idx >> FRACBITS];	idx += inc;
        sample2 = src[idx >> FRACBITS];	idx += inc;
        sample3 = src[idx >> FRACBITS];	idx += inc;
        sample4 = src[idx >> FRACBITS];	idx += inc;
        
        *dst++ += Vol_L[sample1];
        *dst++ += Vol_L[sample2];
        *dst++ += Vol_L[sample3];
        *dst++ += Vol_L[sample4];
    }


    while(remain--)
    {
        *dst++ += Vol_L[ src[idx >> FRACBITS] ];
		idx += inc;
    }
    

    return idx;
}





/*---------------------------------------------------------------------------------------------------------------------*/
void	AddChannel(int voc, SWORD *dst, ULONG todo)
/*---------------------------------------------------------------------------------------------------------------------*/
{
SWORD 	*pVolTab_L;
ULONG	inc, lim, vol_L;
ULONG	samplesUntilEnd, stepsUntilEnd, mixNow;

KMixChn	*chan = &MixChn[voc];


	if ((chan->Active == FALSE)	|| (chan->SampleLen_i < 4) || (chan->Freq < 16) ) return;

	vol_L = (chan->Volume * chan->MasterVol) / 64;

	inc = ((MixChn[voc].Freq << FRACBITS) / MixingFrequency) ;

	pVolTab_L = VolTab + (vol_L * 256);


	lim = chan->SampleLen_i << FRACBITS;

	while(todo > 0)
	{
		while(chan->PosIndex >= lim)
		{
			chan->PosIndex -= lim;
			chan->SamplePos_i = chan->SamplePos;
			chan->SampleLen_i = chan->SampleLen;
			lim = chan->SampleLen_i << FRACBITS;
		}

		if (chan->SampleLen_i < 3) { todo = 0; chan->Active = FALSE; return; } 
		



		samplesUntilEnd = lim - chan->PosIndex;


		stepsUntilEnd = samplesUntilEnd / inc;
		if (samplesUntilEnd % inc) stepsUntilEnd++;


		mixNow = (stepsUntilEnd > todo) ? todo : stepsUntilEnd;
		todo -= mixNow;


		if (mixNow <= 0) return;


		if (vol_L == 0) chan->PosIndex += (mixNow * inc); 
		else
		{
			chan->PosIndex = MixMonoNormal(	chan->SamplePos_i, 
												dst, 
												chan->PosIndex, 
												inc,
												mixNow,
												pVolTab_L);
		}



		dst += mixNow;
	}

}


//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************


/*-----------------------------------------------------------------------*/
void	paulaReset(void)
/*-----------------------------------------------------------------------*/
{
int	voc;

	for (voc = 0; voc < NUMCHN; voc++)
	{
		MixChn[voc].Num = voc;

		MixChn[voc].MasterVol = 64;

		MixChn[voc].Active = FALSE;

		MixChn[voc].SampleLen = 0;
		MixChn[voc].SampleLen_i = 0;

		MixChn[voc].SamplePos = &nullBytes[0];
		MixChn[voc].SamplePos_i = &nullBytes[0];

		MixChn[voc].Volume = 0;
		MixChn[voc].Freq = 0;
		MixChn[voc].PosIndex = 0;

	}

	innerSampleCount = 0;

}










/*-----------------------------------------------------------------------*/
void	paulaSetFreq(int voc, int freq)
/*-----------------------------------------------------------------------*/
{
	if (!ValidVoice(voc)) return;

	MixChn[voc].Freq = freq & 0xFFFF;

	if (MixChn[voc].Freq < 2)
	{
		MixChn[voc].Freq = 0;
		MixChn[voc].Active = FALSE;
	}

}


/*-----------------------------------------------------------------------*/
void	paulaSetPeriod(int voc, int period)
/*-----------------------------------------------------------------------*/
{
	if (!ValidVoice(voc)) return;

	if (period == 0)
	{
		MixChn[voc].Active = FALSE;
		return;
	}

	if (period < 0x40)  period = 0x40;
	if (period > 0x6B00) period = 0x6B00;

	paulaSetFreq(voc, Period2Freq(period));
}



/*-----------------------------------------------------------------------*/
void	paulaSetVolume(int voc, int vol)
/*-----------------------------------------------------------------------*/
{
	if (!ValidVoice(voc)) return;

	vol = ((((vol < 0) ? 0 : vol) > MAXVOL) ? MAXVOL : vol);

	MixChn[voc].Volume = (UBYTE)(vol&0xFF);
}


/*-----------------------------------------------------------------------*/
void	paulaSetSamplePos(int voc, SBYTE *Address)
/*-----------------------------------------------------------------------*/
{
	if (!ValidVoice(voc)) return;

	if (Address != NULL) MixChn[voc].SamplePos = Address;
}



/*-----------------------------------------------------------------------*/
void	paulaSetSampleLen(int voc, UWORD Length)
/*-----------------------------------------------------------------------*/
{
	if (!ValidVoice(voc)) return;

	if (Length > 0) 
		MixChn[voc].SampleLen = Length*2;
	else
		paulaStopVoice(voc);
}




/*-----------------------------------------------------------------------*/
void	paulaPlayVoice(int voc)
/*-----------------------------------------------------------------------*/
{
	if (!ValidVoice(voc)) return;

	MixChn[voc].SamplePos_i = MixChn[voc].SamplePos;
	MixChn[voc].SampleLen_i = MixChn[voc].SampleLen;
	MixChn[voc].PosIndex = 0;

	MixChn[voc].Active = (MixChn[voc].SampleLen_i > 0) ? TRUE : FALSE;
}


/*-----------------------------------------------------------------------*/
void	paulaStopVoice(int voc)
/*-----------------------------------------------------------------------*/
{
	if (!ValidVoice(voc)) return;

	MixChn[voc].Active = FALSE;
}



/*--------------------------------------------------------------------------------------*/
void	paulaVoice2(int voc, int md, SBYTE *address, ULONG loopSta, ULONG loopEnd)
/*--------------------------------------------------------------------------------------*/
{
	voc &= 3;
	if (md)
	{
		MixChn[voc].SamplePos_i = address;
		MixChn[voc].SampleLen_i = loopEnd + 1;

		MixChn[voc].SamplePos = address + loopSta;
		MixChn[voc].SampleLen = (loopEnd + 1) - loopSta;
	}
	else
	{
		MixChn[voc].SamplePos_i = address;
		MixChn[voc].SampleLen_i = loopEnd + 1;

		MixChn[voc].SamplePos = address;
		MixChn[voc].SampleLen = 0;
	}
	
	MixChn[voc].PosIndex = 0;
}


/*-----------------------------------------------------------------------*/
void	paulaWriteDmaCon(UWORD amigaWord)
/*-----------------------------------------------------------------------*/
{
int	i;

	// Set channel DMA.
	if (amigaWord & 0x8000)
	{
		for(i = 0; i < NUMCHN; i++)
		   if ( amigaWord & (1<<i) ) paulaPlayVoice(i);
	}
	// Clear channel DMA.
	else
	{
		for(i = 0; i < NUMCHN; i++)
		   if ( amigaWord & (1<<i) ) paulaStopVoice(i);
	}

}




// nbSample = mixingFrequency / userRate; (441)
// innerSampleCount = 0;
/*--------------------------------------------------------------*/
void	paulaUpdate(SWORD *dst, int nbSampleToCompute)
/*--------------------------------------------------------------*/
{
		if (Opened == FALSE) return;


		if ((dst == NULL) || (nbSampleToCompute <= 0)) return;


	// 1) Clear BUFFER

		ZeroMemory(dst, nbSampleToCompute * sizeof(SWORD));


		do
		{
			int nbs = nbSample - innerSampleCount;
			
			if (nbs > nbSampleToCompute) nbs = nbSampleToCompute;
			
			innerSampleCount += nbs;
			
			if (innerSampleCount >= nbSample)
			{
				if (UserFunction) UserFunction();
				innerSampleCount -= nbSample;
			}

		// 2) Mix Channels

			AddChannel(0, dst, nbs);
			AddChannel(1, dst, nbs);
			AddChannel(2, dst, nbs);
			AddChannel(3, dst, nbs);


		// 3) Filter BUFFER

			if (Filter) FirMemory(dst, nbs);
			
			nbSampleToCompute -= nbs;
			dst += nbs;

			Sleep(0);
		}
		while (nbSampleToCompute > 0);
}


/*-----------------------------------------------------------------------*/
void	paulaSoundProc(void *pSoundBuffer,long bufferLen)
/*-----------------------------------------------------------------------*/
{
		paulaUpdate((SWORD*)pSoundBuffer, bufferLen / sizeof(SWORD));
}


/*-----------------------------------------------------------------------*/
void	paulaOpen( PFUNC UserFunc, int UserFuncRate)
/*-----------------------------------------------------------------------*/
{
	Opened = FALSE;

	LocalMem = (SBYTE*)malloc(LOCALSIZE);
	if (LocalMem == NULL) return;

SBYTE *p = LocalMem;


	MixChn = (KMixChn*)p;		p += CHANSIZE;
	VolTab = (SWORD*)  p;		p += VOLTSIZE;
	LocalBuffer = (SWORD*) p;	p += MIXBSIZE;
	nullBytes = p;

	MixingFrequency = MIXFREQ;

	UserFunction = UserFunc;
	UserFunctionRate = UserFuncRate;
	

	nbSample = MixingFrequency / UserFunctionRate;
	
	InitVolTab();
	paulaReset();

	
	pServer = new CSoundServer;


	Opened = TRUE;
}


/*-----------------------------------------------------------------------*/
void	paulaSetRate(int rate)
/*-----------------------------------------------------------------------*/
{
	UserFunctionRate = rate;
	nbSample = MixingFrequency / UserFunctionRate;
}


/*-----------------------------------------------------------------------*/
void	paulaStart(void)
/*-----------------------------------------------------------------------*/
{
	pServer->open(paulaSoundProc,500);
}

/*-----------------------------------------------------------------------*/
void	paulaClose(void)
/*-----------------------------------------------------------------------*/
{
	Opened = FALSE;
	Filter = FALSE;

	delete pServer;

	free(LocalMem);
}
