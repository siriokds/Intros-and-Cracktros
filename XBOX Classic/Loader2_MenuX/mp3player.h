#ifndef _MP3_PLAYER_H__
#define _MP3_PLAYER_H__

#include "dout.h"
#include "mp3.h"
#include <stdio.h>

void	mp3_init(char *fname, int VMode50);
void	mp3_start(void);
void	mp3_run(void);
void	mp3_setmastervol(int vol);
void	mp3_setmastervolperc(int vol);


#endif	// _MP3_PLAYER_H__
