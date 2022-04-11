#ifndef MP3LIB_H_
#define MP3LIB_H_


/* decoder level: */
void MP3_Init();
int  MP3_Open(char *filename,int buffsize);
void MP3_SeekFrame(int num,int dir);
void MP3_SeekForward(int num);
int  MP3_DecodeFrame(unsigned char *hova,short single);
void MP3_Work(short *dst, int todo);
void MP3_Close();

/* public variables: */
extern int MP3_eof;        // set if EOF reached
extern int MP3_pause;      // lock playing

/* informational: */
extern int MP3_filesize;   // filesize
extern int MP3_frames;     // current frame no
extern int MP3_fpos;       // current file pos
extern int MP3_framesize;  // current framesize in bytes (including header)
extern int MP3_bitrate;    // current bitrate (kbits)
extern int MP3_samplerate; // current sampling freq (Hz)


#endif