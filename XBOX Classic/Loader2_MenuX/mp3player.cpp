#include "mp3player.h"


char*	mp3file = 0;
int		mp3len = 0;
short	mp3_buffer[65536];


void	mp3_setmastervol(int vol)
{
		DOut_SetVol(vol);
}

void	mp3_setmastervolperc(int vol)
{
		DOut_SetVolPerc(vol);
}

void*	mp3_callback(int todo)
{
	MP3_Work(mp3_buffer, todo);

	return (void*)mp3_buffer;
}



void	mp3_run(void)
{
		DOut_RunOnce();
}



void	mp3_init(char *fname, int VMode50)
{
FILE *fp = fopen(fname, "rb");

	if (!fp) return;

	fseek(fp,0,2); mp3len = ftell(fp); fseek(fp,0,0);

	if (mp3file != 0) delete [] mp3file;
	mp3file = new char[mp3len+256];

	fread(mp3file,1,mp3len,fp);
	fclose(fp);


	MP3_Init();
	MP3_Open(mp3file, mp3len);

	DOut_Open(mp3_callback, VMode50);

}

void	mp3_start(void)
{
	DOut_Start();
}
