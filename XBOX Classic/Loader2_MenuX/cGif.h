#ifndef DEGIF_H
#define DEGIF_H

class	cGifImage
{
private:
	int	curr_size, clear, ending, newcodes, top_slot, slot;
	int	navail_bytes;
	int	nbits_left;
	unsigned char  	b1;
	unsigned char  	byte_buff[257];
	unsigned char  	*SRC, *pbytes, *stack, *suffix;
	unsigned int*  	prefix;
	unsigned char	palette[768];
	unsigned long 	code_mask[13];

	int		width;
	int		pitch;

	int		padcount;


	void		Clear();
	int		Mget_next_code(void);
	void	PutColorData(unsigned char *D, int col);


public:
		cGifImage(void);
		~cGifImage();

	int	GetSize(unsigned char *Src, int *width, int *height);
	int	LoadGifFromMemory(unsigned char *Src, unsigned char *Dst);
};




#endif