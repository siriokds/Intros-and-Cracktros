#include <dsound.h>
#include "DOut.h"


#define DOUT_SAMPLESIZE		(sizeof(SWORD)*2)
#define DOUT_BLOCKS			4

static	HRESULT				hr;
static	LPDIRECTSOUND		lpDSOUND = 0;

static	LPDIRECTSOUNDBUFFER	pdsbLoop;
static	WAVEFORMATEX		wfx;
static	DSBUFFERDESC		dsbd;

static  int                 g_MixFreq;
static  int                 g_SamplesToDo;

static	DWORD				nBufferLenBytes, nBuffer;
static	void*				(*UserFunction)(int);

static	int					Opened = 0;

static  int					bux = 0;
static  SWORD				lastbux = -1;
static	int					dout_perc = 255;
static	int					dout_vol  = 255;



/*--------------------------------------------------------------*/
void	DOut_SetVol(int vol)
/*--------------------------------------------------------------*/
{
	if (pdsbLoop == 0) return;
	
	vol = (vol < 0) ? 0 : ((vol > 255) ? 255 : vol);

	dout_vol = vol;

	float gain = float(vol) / 255.0f;
	gain = gain * float(dout_perc) / 255.0f;

	if (gain < 0.00001f) gain = 0.00001f;

	LONG volume = (LONG)(100.0 * 20.0 * log10(gain));

	if (volume < DSBVOLUME_MIN) { volume = DSBVOLUME_MIN; }
	else if (volume > DSBVOLUME_MAX) { volume = DSBVOLUME_MAX; }

	pdsbLoop->SetVolume(volume);
}


/*--------------------------------------------------------------*/
void	DOut_SetVolPerc(int vol)
/*--------------------------------------------------------------*/
{
	vol = (vol < 0) ? 0 : ((vol > 255) ? 255 : vol);
	dout_perc = vol;

	DOut_SetVol(dout_vol);
}


/*--------------------------------------------------------------*/
void	DOut_BufferCopy(void *src)
/*--------------------------------------------------------------*/
{
void*	lpWrite1;
DWORD	dwLen1;
int		pos;

	pos = (nBuffer * nBufferLenBytes);

	hr = pdsbLoop->Lock( pos, nBufferLenBytes, &lpWrite1,&dwLen1,NULL,0,0);

	if (hr == DS_OK)
    {
		if (src) CopyMemory(lpWrite1,src,nBufferLenBytes);
        else     ZeroMemory(lpWrite1,nBufferLenBytes);
	}

	pdsbLoop->Unlock(lpWrite1,dwLen1,NULL,0);

}



/*--------------------------------------------------------------*/
void	DOut_BufferZero(void)
/*--------------------------------------------------------------*/
{
void*	lpWrite1;
DWORD	dwLen1;

	hr = pdsbLoop->Lock( 0, nBufferLenBytes*DOUT_BLOCKS, &lpWrite1,&dwLen1,NULL,0,0);

	if (hr == DS_OK)
    {
        ZeroMemory(lpWrite1,nBufferLenBytes*DOUT_BLOCKS);
	}

	pdsbLoop->Unlock(lpWrite1,dwLen1,NULL,0);

}



/*------------------------------------------------------------*/
void	DOut_RunOnce(void)
/*------------------------------------------------------------*/
{
DWORD	playpos;


	pdsbLoop->GetCurrentPosition(&playpos,NULL);
	bux = (playpos / nBufferLenBytes);

	if (bux != lastbux)
    {
		lastbux = bux;

		nBuffer = (nBuffer + 1) % DOUT_BLOCKS;

		void* ptr = (*UserFunction)( g_SamplesToDo );
		if (ptr) DOut_BufferCopy( ptr );
	}
}


/*--------------------------------------------------------------*/
void    DOut_DSClose(void)
/*--------------------------------------------------------------*/
{
	if (lpDSOUND)	lpDSOUND->Release();
        lpDSOUND = 0;
}






/*--------------------------------------------------------------*/
int	DOut_Open(void* (*UserF)(int), int VMode50 )
/*--------------------------------------------------------------*/
{
int	nRet = 0;

	Opened = 0;
	lpDSOUND = 0;
	nBuffer = DOUT_BLOCKS-1;


	nRet = DirectSoundCreate(NULL,&lpDSOUND,NULL);
	if (nRet < 0 || lpDSOUND == NULL) { return 1; }
	

	UserFunction = UserF;

	g_MixFreq = 44100;
    g_SamplesToDo = (VMode50 != 0) ? ((g_MixFreq*10)/50) : ((g_MixFreq*10)/60);


    nBufferLenBytes = g_SamplesToDo * DOUT_SAMPLESIZE;


	//----------------------------------------------------------------
	// Secondary Buffer
	//----------------------------------------------------------------

	memset(&wfx, 0, sizeof(wfx));
//	wfx.cbSize = sizeof(wfx);
	wfx.cbSize = 0;
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = g_MixFreq;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = 4;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;



	DSMIXBINVOLUMEPAIR dsmbvp[8] = {
	{DSMIXBIN_FRONT_LEFT, DSBVOLUME_MAX},   // left channel
	{DSMIXBIN_FRONT_RIGHT, DSBVOLUME_MAX},  // right channel
	{DSMIXBIN_FRONT_CENTER, DSBVOLUME_MAX}, // left channel
	{DSMIXBIN_FRONT_CENTER, DSBVOLUME_MAX}, // right channel
	{DSMIXBIN_BACK_LEFT, DSBVOLUME_MAX},    // left channel
	{DSMIXBIN_BACK_RIGHT, DSBVOLUME_MAX},   // right channel
	{DSMIXBIN_LOW_FREQUENCY, DSBVOLUME_MAX},    // left channel
	{DSMIXBIN_LOW_FREQUENCY, DSBVOLUME_MAX}};   // right channel
		
	DSMIXBINS dsmb;
	dsmb.dwMixBinCount = 8;
	dsmb.lpMixBinVolumePairs = dsmbvp;

	memset(&dsbd, 0, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);

 	dsbd.dwFlags = 0;
	dsbd.dwBufferBytes = nBufferLenBytes * DOUT_BLOCKS;		// 4 MiniBlocks (2 = double buffered)
	dsbd.lpwfxFormat = &wfx;
	dsbd.lpMixBins = &dsmb;

	nRet = lpDSOUND->CreateSoundBuffer(&dsbd, &pdsbLoop, NULL);
	if (nRet < 0 || pdsbLoop == NULL) { DOut_DSClose(); return 1; }

	DOut_BufferZero();

	Opened = 1;

	return 0;
}



/*--------------------------------------------------------------*/
int	 DOut_Start(void)
/*--------------------------------------------------------------*/
{
	if (Opened == 0) return 1;

	nBuffer = DOUT_BLOCKS-3;

	pdsbLoop->SetCurrentPosition(0);
	pdsbLoop->Play(0, 0, DSBPLAY_LOOPING);

	return 0;
}



/*--------------------------------------------------------------*/
void 	DOut_Close(void)
/*--------------------------------------------------------------*/
{
	if (Opened == 0) return;

	pdsbLoop->Stop();
	
	DOut_DSClose();
}

