#include "cGif.h"

#define  MAX_CODES  4096


/*-----------------------------------------------------------------------------------*/
	cGifImage::cGifImage(void)
/*-----------------------------------------------------------------------------------*/
{
unsigned long 	codemask[13] = { 0L,
			0x0001L, 0x0003L, 0x0007L, 0x000FL, 0x001FL, 0x003FL,
			0x007FL, 0x00FFL, 0x01FFL, 0x03FFL, 0x07FFL, 0x0FFFL};


	for (int i = 0; i < 13; i++) code_mask[i] = codemask[i];


	stack  = (unsigned char*) new unsigned char[MAX_CODES + 1];
	suffix = (unsigned char*) new unsigned char[MAX_CODES + 1];
	prefix = (unsigned int*)  new unsigned int[sizeof(int) * (MAX_CODES + 1)];


	Clear();
}


/*-----------------------------------------------------------------------------------*/
	cGifImage::~cGifImage()
/*-----------------------------------------------------------------------------------*/
{
	if (stack)  delete [] stack;
	if (suffix) delete [] suffix;
	if (prefix) delete [] prefix;
}


/*-----------------------------------------------------------------------------------*/
void	cGifImage::Clear(void)
/*-----------------------------------------------------------------------------------*/
{
	navail_bytes = 0;
	nbits_left = 0;
	padcount = 0;
}


/*-----------------------------------------------------------------------------------*/
void	cGifImage::PutColorData(unsigned char *D, int col)
/*-----------------------------------------------------------------------------------*/
{
	 col = (col << 1) + col;

	 D[3] = 0xFF;
	 D[2] = palette[col + 0];
	 D[1] = palette[col + 1];
	 D[0] = palette[col + 2];
}



/*-----------------------------------------------------------------------------------*/
int	cGifImage::Mget_next_code(void)
/*-----------------------------------------------------------------------------------*/
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


/*-----------------------------------------------------------------------------------*/
int	cGifImage::GetSize(unsigned char *s, int *width, int *height)
/*-----------------------------------------------------------------------------------*/
{
	if (!s) return -1;
	if ( ((int*)s)[0] != 0x38464947 ) return -1;	// check for "GIF8"
	unsigned short *p = (unsigned short *)(s + 6);
	
	if (width)  *width  = (int)p[0];
	if (height) *height = (int)p[1];
	
	return 0;
}


/*-----------------------------------------------------------------------------------*/
int	cGifImage::LoadGifFromMemory(unsigned char *S, unsigned char *D)
/*-----------------------------------------------------------------------------------*/
{
   unsigned char* sp;
   int            code, fc, oc;
   int            i, c;
   unsigned char  size;

   Clear();

   SRC = S;
   SRC += 13;

   for (i = 0; i < 768; i++) palette[i] = (*SRC++);
   

   SRC += 10;
   size = *SRC++;

   if( size < 2 || 9 < size ) return 0;


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
		if ( c >= slot ) c = 0;
		oc = fc = c;
		PutColorData(D,c); D += 4;
      }
      else
      {
		code = c;
		if ( code >= slot ) { code = oc; *sp++ = fc; }
		while( code >= newcodes ) { *sp++ = *(suffix + code); code = *(prefix + code); }
	 
		*sp++ = code;
	 
		if ( slot < top_slot )
		{
			*(suffix + slot) = fc = code;
			*(prefix + slot++) = oc;
			oc = c;
		}
		if ( slot >= top_slot && curr_size < 12 ) { top_slot <<= 1; ++curr_size; }
		while( sp > stack ) { --sp; PutColorData(D,sp[0]);	D += 4; }
      }
   }

   return 1;
}

