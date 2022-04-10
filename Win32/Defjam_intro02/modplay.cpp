#include <windows.h>
#include "Types.h"
#include "Paula.h"


#define	PERIODSIZ	134
#define NONOTE		132
#define KEYOFF		133
#define NOVOL		255



void	MOD_UpdateNote(void);
void	MOD_UpdateEffect(int tick);



//-----------------------------------------------------------------
// TYPES
//-----------------------------------------------------------------

typedef	struct		// INSTRUMENT
{
	ULONG	Start;			// Sample Pos			4
	ULONG	LoopSta;		// Loop Start			4
	ULONG	LoopEnd;		// Loop Size			4
	UBYTE	Mode;			// Loop Flag			1
	UBYTE	Volume;			// Volume (0 - 40h)		1
	UWORD	C2Spd;			// Finetune			2
} tsInstr;

typedef struct			// Song type - contains info on song
{
	UBYTE *order;		// pattern playing orders
	tsInstr *instr;

	SWORD songlength;	// song length
	SWORD numpats;		// number of physical patterns
	SBYTE numinstr;		// number of instruments
	SBYTE restart;		// restart position
	UBYTE playing;		// if song is playing.  0 = no, 1 = yes
	UBYTE speed, bpm;	// speed & bpm, figure it out.
	int   tick;
	SBYTE *module;
	SBYTE row;
	SBYTE ord;
	UBYTE patdelay;		// # of notes to delay pattern for in EEx
} Song;





typedef	struct		// CHANNEL
{
// general variables
 UBYTE	mute;		// toggle whether to mute channel or not
 SBYTE	volume;		// current volume of each channel
 SBYTE	lastins;	// instrument # for each channel (to remember)
 UBYTE	lastnot;	// last note set in channel (to remember)

 SWORD	freq;		// amiga frequency of each channel
 SWORD	panval;		// pan values for each channel

 UWORD	lastper;	// last period set in channel (to remember)
 UBYTE	restart;	// flag whether to play sample or not
 UBYTE	filler;

 ULONG	start;		// where to start in sample

// effect variables
 ULONG	soffset;	// amount to sample offset by

 SWORD	porto;		// note to port to value (signed word)
 UBYTE  portsp;		// porta speed
 SBYTE	vibpos;		// vibrato position

 UBYTE  vibspe;		// vibrato speed
 UBYTE  vibdep;		// vibrato depth
 SBYTE	trempos;	// tremolo position
 UBYTE	tremspe;	// tremolo speed

 UBYTE	tremdep;	// tremolo depth
 UBYTE	patlooprow;
 UBYTE	patloopno;	// pattern loop variables for effect  E6x
 UBYTE	wavecon;	// waveform type for vibrato and tremolo (4bits each)

} tsChannel;




typedef struct		// Single note type - contains info on 1 note
{
	UBYTE note;		// note to play at
	UBYTE number;	// sample being played
	UBYTE effect;	// effect number
	UBYTE eparm;	// effect parameter

} Note;




int filler[] = { 0,0 };

char PLAYER_ID[] = "****************DC ModPlayer 1.0- SiRioKD 2oo1 -****************";



// SINE TABLE FOR TREMOLO AND VIBRATO 
// (from protracker so 100% compatible)
UBYTE Sintab[32] = 
{
	   0, 24, 49, 74, 97,120,141,161,
	 180,197,212,224,235,244,250,253,
	 255,253,250,244,235,224,212,197,
	 180,161,141,120, 97, 74, 49, 24
};

// AMIGA TYPE PERIOD TABLE, FOR 11 OCTAVES 
// (octave 9 and 10 are only for .MOD's that use these sort 
// of periods when loading, i.e dope.mod)

UWORD Periodtab[PERIODSIZ] = 
{
  27392,25856,24384,23040,21696,20480,19328,18240,17216,16256,15360,14496,//0   0
  13696,12928,12192,11520,10848,10240, 9664, 9120, 8608, 8128, 7680, 7248,//1   12

   6848, 6464, 6096, 5760, 5424, 5120, 4832, 4560, 4304, 4064, 3840, 3624,//2   24
   3424, 3232, 3048, 2880, 2712, 2560, 2416, 2280, 2152, 2032, 1920, 1812,//3   36
   1712, 1616, 1524, 1440, 1356, 1280, 1208, 1140, 1076, 1016,  960,  906,//4   48

    856,  808,  762,  720,  678,  640,  604,  570,  538,  508,  480,  453,//5   60
    428,  404,  381,  360,  339,  320,  302,  285,  269,  254,  240,  226,//6   72
    214,  202,  190,  180,  170,  160,  151,  143,  135,  127,  120,  113,//7   84

    107,  101,   95,   90,   85,   80,   75,   71,   67,   63,   60,   56,//8   96
     53,   50,   47,   45,   42,   40,   37,   35,   33,   31,   30,   28,//9   108

     26,   25,   23,   22,   21,   20,   18,   17,   16,   15,   15,   14,//10
      0,    0       // <- these last 2 are for no note (132)
					// and keyoff (133)
};


int FineHz[16] =
{
	8363, 8413, 8463, 8529, 8581, 8651, 8723, 8757,
	7895, 7941, 7985, 8046, 8107, 8169, 8232, 8280
};




//-----------------------------------------------------------------
// VARIABLES
//-----------------------------------------------------------------

// PLAY TIME VARIABLES

//extern unsigned int jps;
int   TPS;

SBYTE gvol = 60;	// master volume


 // PATTERN AND SONG DATA VARIABLES
Song		FM;				// the song header information
SBYTE		*patbuff[256];		// pattern data pointers
tsChannel	Chans[4];
Note		*current;		// current - a global 'note'






//-----------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------



/*-----------------------------------------------------------------------*/
int 	FinetoHz(UBYTE ft) 
/*-----------------------------------------------------------------------*/
{
	return FineHz[ft & 15];
}

/*-----------------------------------------------------------------------*/
static int fTPS(int x) 
/*-----------------------------------------------------------------------*/
{
	return x * 50 * 4 / 125;
}



/*---------------------------------------------------------------*/
void	MOD_ZeroMemory(char *dst, int size)
/*---------------------------------------------------------------*/
{
	while (size--) *dst++ = 0;
}

/*---------------------------------------------------------------*/
void	MOD_CopyMemory(char *dst, char *src, int size)
/*---------------------------------------------------------------*/
{
	while (size--) *dst++ = *src++;
}


/*-----------------------------------------------------------------------*/
void	doporta(UBYTE track)
/*-----------------------------------------------------------------------*/
{
	// slide pitch down if it needs too.
	if (Chans[track].freq < Chans[track].porto)
	{
		Chans[track].freq += ((SWORD)Chans[track].portsp << 2);
		if (Chans[track].freq > Chans[track].porto)
			Chans[track].freq = Chans[track].porto;
	}

	// slide pitch up if it needs too.
	if (Chans[track].freq > Chans[track].porto)
	{
		Chans[track].freq -= ((SWORD)Chans[track].portsp << 2);
		if (Chans[track].freq < Chans[track].porto)
			Chans[track].freq = Chans[track].porto;
	}

	 //
	 // if (glissando[track]) {
	 //}
	 //
	paulaSetPeriod(track,Chans[track].freq);
}




/*-----------------------------------------------------------------------*/
void	dovibrato(UBYTE track) 
/*-----------------------------------------------------------------------*/
{
UWORD delta = 0;
SBYTE temp;

	temp = (Chans[track].vibpos & 31);

	switch(Chans[track].wavecon & 3)
	{
		case 0: delta = Sintab[temp];             // sine
				break;
		case 1: temp <<= 3;	                  // ramp down
				if (Chans[track].vibpos<0) temp=255-temp;
				delta=temp;
				break;
		case 2: delta = 255;                      // square
				break;
		case 3: delta = Sintab[temp];             // random
				break;
	};

	delta *= Chans[track].vibdep;
	delta >>=6;
//	delta <<=1;   // we use 4*periods so make vibrato 4 times bigger


	if (Chans[track].vibpos >= 0)
			paulaSetPeriod(track, Chans[track].freq + delta);
	else	        paulaSetPeriod(track, Chans[track].freq - delta);

	Chans[track].vibpos += Chans[track].vibspe;
	if (Chans[track].vibpos > 31) Chans[track].vibpos -= 64;
}




/*-----------------------------------------------------------------------*/
void	dotremolo(UBYTE track)
/*-----------------------------------------------------------------------*/
{
UWORD delta = 0;
UBYTE temp;

	temp = (Chans[track].trempos & 31);

	switch((Chans[track].wavecon >> 4) & 3)
	{
		case 0: delta = Sintab[temp];             // sine
				break;
		case 1: temp <<= 3;                     // ramp down
				if(Chans[track].vibpos < 0) temp=255-temp;
				delta=temp;
				break;
		case 2: delta = 255;                      // square
				break;
		case 3: delta = Sintab[temp];             // random (just use sine)
				break;
	};

	delta *= Chans[track].tremdep;
	delta >>= 6;

	if (Chans[track].trempos >= 0) 
	{
		if (Chans[track].volume + delta > 64)
                        delta = 64 - Chans[track].volume;

		paulaSetVolume(track, (Chans[track].volume + delta) * gvol / 64);
	}
	else
        {
		if ((SWORD)(Chans[track].volume - delta) < 0)
			delta = Chans[track].volume;

		paulaSetVolume(track, (Chans[track].volume - delta) * gvol / 64);
	}

	Chans[track].trempos += Chans[track].tremspe;
	if (Chans[track].trempos > 31) Chans[track].trempos -= 64;
}








/*-----------------------------------------------------------------------*/
void	MOD_UpdateNote(void)
/*-----------------------------------------------------------------------*/
{
UBYTE track, eparmx, eparmy;
UBYTE breakflag = 0, jumpflag = 0, lastvoice = 0;
tsChannel *p;



	current = (Note *)( patbuff[FM.order[FM.ord]] + (sizeof(Note) * FM.row * 4) );

	for (track = 0; track < 4; track++) 
	{
		p = &Chans[track];	
	  	eparmx = current -> eparm >> 4;	        // get effect param x
	  	eparmy = current -> eparm & 0xF;	// get effect param y

	  	// if an INSTRUMENT NUMBER is given
	  	if (current->number) 
	  	{
			// remember the Instrument #
			p->lastins = current->number;

			if (!(current -> effect == 0xE && eparmx == 0xD))
				p->volume = FM.instr[p->lastins].Volume;
	  	}

		// if a NOTE is given

		if ( current->note != NONOTE )
		{
			// get period according to relative note, c2spd and finetune
                        p->lastper = 8363L * Periodtab[current->note] / FM.instr[p->lastins].C2Spd;

			p->lastnot = current->note;			// now remember the note
			p->restart = 1;                     		// retrigger sample
			p->start = FM.instr[p->lastins].Start;		// store sample start
			lastvoice = track;                      	// last voiced used

			// retrigger tremolo and vibrato waveforms

			if ((p->wavecon & 0xF) < 4) p->vibpos = 0;
			if ((p->wavecon >> 4) < 4)  p->trempos = 0;

			// frequency only changes if there are no portamento effects

			if (current->effect != 0x3 && current->effect != 0x5 &&	current->effect != 0x16)
				p->freq = p->lastper;
		}
                else  p->restart = 0;      // else dont retrigger a sample

		// process keyoff note
		if (current->note == KEYOFF) p->volume = 0;

		// TICK 0 EFFECTS

		switch (current -> effect)
		{
			case 0x0: break;                // dont waste my time in here!!!
			case 0x1: break;                // not processed on tick 0
			case 0x2: break;                // not processed on tick 0

		// Set Volume
			case 0xC: p->volume = current -> eparm;
					  if (p->volume > 64) p->volume = 64;
					  break;

		// Porta to Note
			case 0x3: if (current->eparm)
						  p->portsp = current->eparm;

		// Porta + Volume Slide
			case 0x5: p->porto = p->lastper;
					  p->restart = 0;
					  break;

		// Vibrato
			case 0x4: if (eparmx) p->vibspe = eparmx;
					  if (eparmy) p->vibdep = eparmy;

		// Vibrato + Volume Slide
			case 0x6: break;		// not processed on tick 0

		// Tremolo
			case 0x7: if (eparmx) p->tremspe = eparmx;
					  if (eparmy) p->tremdep = eparmy;
					  break;

		// Pan
			case 0x8: p->panval = current -> eparm;
//					  GUSSetBalance(track, panval[track]);
					  break;

		// Sample Offset
			case 0x9: if (current->eparm)
                          p->soffset = current->eparm << 8;

                      if (p->soffset > FM.instr[p->lastins].LoopEnd)
                              p->soffset = FM.instr[p->lastins].LoopEnd;
                                        
                      p->start += p->soffset;
        			  break;

		// Volume Slide
			case 0xA: break;	// not processed on tick 0

		// Jump To Pattern
			// --- 00 B00 : --- 00 D63 , should put us at FM.ord=0, FM.row=63
			case 0xB: FM.ord = current->eparm;
					  FM.row = -1;
					  if (FM.ord >= FM.songlength) FM.ord=0;
					  jumpflag = 1;
					  break;

		// Pattern Break
			case 0xD: FM.row = (eparmx*10) + eparmy -1;
					  if (FM.row > 63) FM.row = -1;
					  if (!breakflag && !jumpflag) FM.ord++;
					  if (FM.ord >= FM.songlength) FM.ord=0;
					  breakflag = 1;
					  break;

		// Set Speed
			case 0xF: 
				if (current->eparm < 0x20) 
					FM.speed = current->eparm;
				else 		    
				{
					FM.bpm = current->eparm; 
					TPS = fTPS(FM.bpm);
				}	  			
				break;

		// extended PT effects
			case 0xE: 
				switch (eparmx) 
				{
					case 0x1: p->freq -= (eparmy << 2);
						  break;
						  
					case 0x2: p->freq += (eparmy << 2);
						  break;
						  
					case 0x3: break;
					
					case 0x4: p->wavecon &= 0xF0;
						  p->wavecon |= eparmy;		
						  break;

					case 0x5: FM.instr[p->lastins].C2Spd = FinetoHz(eparmy);
						  break;

					case 0x6: if (eparmy == 0) p->patlooprow = FM.row;
						  else 
						  {
							  if (!p->patloopno) p->patloopno = eparmy;
							  else 		     p->patloopno--;

							  if (p->patloopno)  FM.row = p->patlooprow-1;
						  } 
						  break;

					case 0x7:	p->wavecon &= 0xF;
							p->wavecon |= (eparmy<<4);
							break;
							
					case 0x8:	p->panval = (eparmy << 4);
//							GUSSetBalance(track, panval[track]);
							break;
							
					case 0x9:	break;

					case 0xA:	p->volume += eparmy;
							if (p->volume > 64) p->volume=64;
							break;

					case 0xB:	p->volume -= eparmy;
							if (p->volume < 0) p->volume = 0;
							break;
							
					case 0xC:	break;  // not processed on tick 0
					case 0xD:	p->restart = 0;	goto nofreq;
					case 0xE:	FM.patdelay = eparmy; break;
				};
				break;

		};

		if (current->effect != 7) paulaSetVolume(track, p->volume * gvol / 64);
		if (p->freq)		  paulaSetPeriod(track, p->freq);
nofreq:
		current++;
	}

	// now play the samples
	for (track=0; track <= lastvoice; track++) 
	{
		p = &Chans[track];

		if (p->restart)
		{
			paulaVoice2(
				track,
				FM.instr[p->lastins].Mode,
				FM.module + p->start,
				FM.instr[p->lastins].LoopSta,
				FM.instr[p->lastins].LoopEnd
			);

		}
	}
}



















/*-----------------------------------------------------------------------*/
void	MOD_UpdateEffect(int tick)
/*-----------------------------------------------------------------------*/
{
UBYTE track, effect, eparmx, eparmy;

	// rewind back 1 row (as we just incremented the row 
	// in modhandler() function)

	current -= 4;

	for (track=0; track < 4; track++)
	{
		// no freq?, so dont do anything!
		if (!Chans[track].freq) { paulaStopVoice(track); goto skip; }
		
		// grab the effect number
		effect = current -> effect;

		// grab the effect parameter x
		eparmx = current -> eparm >> 4;

		// grab the effect parameter y
		eparmy = current -> eparm & 0xF;


		switch(effect) 
		{
		// Arpeggio
		case 0x0: if (current -> eparm > 0) switch (tick % 3)
				  {
					case 0: paulaSetPeriod(track, Chans[track].freq);
							break;
					case 1: paulaSetPeriod(track, Periodtab[Chans[track].lastnot+eparmx]);
							break;
					case 2: paulaSetPeriod(track, Periodtab[Chans[track].lastnot+eparmy]);
							break;
				  };
				  break;

		// Porta Up
		case 0x1: Chans[track].freq -= (current -> eparm << 2); // subtract freq
				  if (Chans[track].freq < 56) Chans[track].freq = 56;  // stop at B#8
				  paulaSetPeriod(track, Chans[track].freq);
				  break;

		// Porta Down
		case 0x2: Chans[track].freq += (current -> eparm << 2);
				  paulaSetPeriod(track, Chans[track].freq);
				  break;

		// Porta to Note
		case 0x3: doporta(track);
				  break;

		// Vibrato
		case 0x4: dovibrato(track);
				  break;

		// Porta to Note + Volume Slide
		case 0x5: doporta(track);
				  Chans[track].volume += eparmx - eparmy;
				  if (Chans[track].volume < 0) Chans[track].volume = 0;
				  if (Chans[track].volume > 64) Chans[track].volume= 64;
				  paulaSetVolume(track, Chans[track].volume * gvol / 64);
				  break;

		// Vibrato + Volume Slide
		case 0x6: dovibrato(track);
				  Chans[track].volume += eparmx - eparmy;
				  if (Chans[track].volume < 0) Chans[track].volume = 0;
				  if (Chans[track].volume > 64) Chans[track].volume= 64;
				  paulaSetVolume(track, Chans[track].volume * gvol / 64);
				  break;

		// Tremolo
		case 0x7: dotremolo(track);
				  break;

		// Volume Slide
		case 0xA: Chans[track].volume += eparmx - eparmy;
				  if (Chans[track].volume < 0) Chans[track].volume = 0;
				  if (Chans[track].volume > 64) Chans[track].volume= 64;
				  paulaSetVolume(track, Chans[track].volume * gvol / 64);
				  break;

		// extended PT effects
		case 0xE: 
			switch(eparmx) 
			{
				// Note Cut - 0 volume after x ticks
				case 0xC:
					if (tick==eparmy) 
					{
						Chans[track].volume = 0;
						paulaSetVolume(track, Chans[track].volume);
					}
					break;

				// Retrig Note - retrigger every x ticks
				case 0x9: 
					if (!eparmy) break; // divide by 0 bugfix
					if (!(tick % eparmy)) 
					{
						paulaVoice2(track,
							FM.instr[Chans[track].lastins].Mode,
							FM.module + FM.instr[Chans[track].lastins].Start,
							FM.instr[Chans[track].lastins].LoopSta,
							FM.instr[Chans[track].lastins].LoopEnd
						);
					}
					break;

				// Note Delay - wait x ticks then play - quite more complex
				case 0xD:
					if (tick==eparmy)	// than it seems.
					{             
						if (current->number) 
							Chans[track].volume =
							  FM.instr[Chans[track].lastins].Volume;

						paulaSetPeriod(track, Chans[track].freq);
						paulaSetVolume(track, Chans[track].volume * gvol / 64);

						paulaVoice2(
							track,
							FM.instr[Chans[track].lastins].Mode,
							FM.module + FM.instr[Chans[track].lastins].Start,
							FM.instr[Chans[track].lastins].LoopSta,
							FM.instr[Chans[track].lastins].LoopEnd
						);
					}
					break;
			  };
			  break;
		};
skip:
		current++;
	}
}








/*---------------------------------------------------------------*/
int		MOD_PLAY_MUSIC(void)
/*---------------------------------------------------------------*/
{
	if (!FM.playing) return 0;

	TPS = fTPS(FM.bpm);
 
 	FM.tick++;
	if (FM.tick >= FM.speed) 
	{
		FM.tick = 0;
		if (FM.row >= 64)
		{ 
		  FM.row = 0;
		  FM.ord++;
		  if (FM.ord >= FM.songlength) FM.ord = FM.restart;
		}

		if (!FM.patdelay) { MOD_UpdateNote(); FM.row++; }
		else FM.patdelay--;
	}
	else
    {
                MOD_UpdateEffect(FM.tick);
    }

	return 0;
}



/*-----------------------------------------------------------------------*/
void	MOD_InitMusic(SBYTE *module)
/*-----------------------------------------------------------------------*/
{
int	i;

	FM.module = module;

	FM.speed = 6;
	FM.bpm = 125;

	FM.row = 0;
	FM.ord = 0;
	FM.patdelay = 0;
	FM.playing = 0;

	FM.restart = module[0x06];
	FM.numpats = module[0x07];
	FM.numinstr = module[0x0E];
	FM.songlength = module[0x0F];
	FM.order = (UBYTE*)module + 0x10;
	FM.instr = (tsInstr*)(module + 0x90);

	for (i = 0; i < FM.numpats; i++)
		patbuff[i] = (module + 0x290 + (i * 1024));

	paulaOpen(MOD_PLAY_MUSIC, 50);
}





/*-----------------------------------------------------------------------*/
void	MOD_StartMusic(void)
/*-----------------------------------------------------------------------*/
{
int	i;

	for (i = 0; i < 4; i++)
		MOD_ZeroMemory((char*)(&Chans[i]),sizeof(tsChannel));

	FM.row = 0;
	FM.ord = 0;
	FM.patdelay = 0;				// notes to delay pattern for EEx

	TPS = fTPS(FM.bpm);

	FM.playing = 1;
	paulaStart();
}


/*-----------------------------------------------------------------------*/
void	MOD_EndMusic(void)
/*-----------------------------------------------------------------------*/
{
	paulaClose();
}


