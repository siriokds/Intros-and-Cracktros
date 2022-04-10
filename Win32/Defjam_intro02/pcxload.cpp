void	pcx_to_bmp(char *source,char *destbmp)
{
char	*dest, *bdest;
short int xs,ys;
int pal[256],r,g,b,a;
unsigned char indx;
int	size;

        memcpy(&xs,(void*)&source[8],2);
        memcpy(&ys,(void*)&source[10],2);
		
		size = xs * ys;

        dest = (char*)malloc(size);
		bdest = dest + size;
	

        //__scompatta la pcx__

        source+=128;				 //salta l'header
        for (;;)
        {
                vl=*buf++;
                if ((vl&0xc0)==0xc0)
                {
                        vl&=0x3f;
                        va=*buf++;
                        for(i=0;i<vl;i++) *dest++=va;
                }
                else *dest++=vl;
                if (dest>bdest) break;
        }

        //__crea la palette__
        for (i=0;i<256;i++)
        {
                r=((*buf++));
                g=((*buf++));
                b=((*buf++));
                pal[i]=(r<<16)|(g<<8)|(b);	//ottiene palette rgb
        }

        for (i=0;i<ys;i++)
                for (j=0;j<xs;j++)
                        {
	                        indx=*dest++;
	                        r=((pal[indx])>>16)&0xff;
	                        g=((pal[indx])>>8)&0xff;
	                        b=((pal[indx]))&0xff;

				a=255;
	                        
				//correct alpha
				//if (indx==0) a=0;
	                        
				*ptr++=r;
	                        *ptr++=g;
	                        *ptr++=b;
	                        *ptr++=a;
                        }
	free(dest);
}

