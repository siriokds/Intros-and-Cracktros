#ifndef __TIMINGH__
#define __TIMINGH__

struct ticker_t
{
   void		Init(void);
   void		Update(void);
   float	Passed(void);			// in milliseconds
   float	FramePerSec(void)	{ return frameSec; }

protected:
   DWORD gtcNow;

   float	LastPassed;
   float	res; 
   float	frameCnt;
   float	frameTot;
   float	frameSec;
};



/*-------------------------------------------------------------*/
void ticker_t::Init(void)
/*-------------------------------------------------------------*/
{
   gtcNow = GetTickCount();

   LastPassed = 0.0f;
   frameCnt = 0.0f;
   frameTot = 0.0f;
   frameSec = 0.0f;
}


/*-------------------------------------------------------------*/
void ticker_t::Update(void)
/*-------------------------------------------------------------*/
{
   gtcNow = GetTickCount();
}


/*-------------------------------------------------------------*/
float ticker_t::Passed(void)
/*-------------------------------------------------------------*/
{
	LastPassed = ((float) (GetTickCount() - gtcNow));

	frameTot++;
	frameCnt += LastPassed;
	
	if (frameCnt > 1000.0f) 
	{
		frameSec = (frameTot * 1000) / frameCnt;
		frameTot = 0.0f;
		frameCnt -= 1000.0f;
	}

	Update();

	return LastPassed;
}


#endif


