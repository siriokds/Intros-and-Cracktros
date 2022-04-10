#include "degif.h"

#define  MAX_CODES  4096

	int	curr_size, clear, ending, newcodes, top_slot, slot;
	int	navail_bytes;
	int	nbits_left;
	unsigned char  	b1;
	unsigned char  	byte_buff[257];
	unsigned char  	*SRC, *pbytes, *stack, *suffix;
	unsigned int*  	prefix;

	unsigned long 	code_mask[13] = { 0L,
				0x0001L, 0x0003L, 0x0007L, 0x000FL, 0x001FL, 0x003FL,
				0x007FL, 0x00FFL, 0x01FFL, 0x03FFL, 0x07FFL, 0x0FFFL};

	int		width;
	int		pitch;

	int		padcount;


	unsigned char tempBuffer[32768*3];



/*---------------------------------------------------------------------------------------*/
int	Mget_next_code(void)
/*---------------------------------------------------------------------------------------*/
{
int  i;
static unsigned long ret;

   if( ! nbits_left )
   {
      if( navail_bytes <= 0 )
      {
	 pbytes = byte_buff;
	 navail_bytes = *SRC++;				//*********
	 if( navail_bytes )
	    for( i = 0; i < navail_bytes; ++i )
	       *(byte_buff + i) = *SRC++;		//*********
      }
      b1 = *pbytes++;
      nbits_left = 8;
      --navail_bytes;
   }
   ret = b1 >> (8 - nbits_left);
   while( curr_size > nbits_left )
   {
      if( navail_bytes <= 0 )
      {
	 pbytes = byte_buff;
	 navail_bytes = *SRC++;                     //*********
	 if( navail_bytes )
	    for( i = 0; i < navail_bytes; ++i )
	       *(byte_buff + i) = *SRC++;           //*********
      }
      b1 = *pbytes++;
      ret |= b1 << nbits_left;
      nbits_left += 8;
      --navail_bytes;
   }
   nbits_left -= curr_size;

   return (int) (ret & *(code_mask + curr_size));
}



/*---------------------------------------------------------------------------------------*/
int	degif(unsigned char *S, unsigned char *D, unsigned char *P)
/*---------------------------------------------------------------------------------------*/
{
unsigned char* sp;
int            code, fc, oc;
int            i, c;
unsigned char  size;

//unsigned char *p = (unsigned char*)0x8003d040;	// use video frame buffer as temp buffers!!!

unsigned char *p = tempBuffer;

   stack = p;  p += 32768;
   suffix = p; p += 32768;
   prefix = (unsigned int *)p;

	navail_bytes = 0;
	nbits_left = 0;
	padcount = 0;

   SRC = S;
   SRC += 13;

   for (i = 0; i < 768; i++) P[i] = (*SRC++);
   

   SRC += 10;
   size = *SRC++;

   if (size < 2 || 9 < size) return 0;


   curr_size	= size + 1;
   top_slot 	= 1 << curr_size;
   clear 	= 1 << size;
   ending 	= clear + 1;
   slot 	= newcodes = ending + 1;
   navail_bytes = nbits_left = 0;
   oc 		= fc = 0;
   sp 		= stack;

   while( (c = Mget_next_code()) != ending )
   {
      if( c == clear )
      {
	 curr_size = size + 1;
	 slot = newcodes;
	 top_slot = 1 << curr_size;
	 while( (c = Mget_next_code()) == clear );
	 if( c == ending ) break;
	 if( c >= slot )   c = 0;
	 oc = fc = c;
	 *D++ = c;	//***
      }
      else
      {
	 code = c;
	 if( code >= slot )
	 {
	    code = oc;
	    *sp++ = fc;
	 }
	 while( code >= newcodes )
	 {
	    *sp++ = *(suffix + code);
	    code = *(prefix + code);
	 }
	 *sp++ = code;
	 if( slot < top_slot )
	 {
	    *(suffix + slot) = fc = code;
	    *(prefix + slot++) = oc;
	    oc = c;
	 }
	 if( slot >= top_slot && curr_size < 12 )
	 {
	    top_slot <<= 1;
	    ++curr_size;
	 }
	 while( sp > stack )
	 {
	    --sp;
	 *D++ = *sp;	//**
	 }
      }
   }
   return 1;
}

