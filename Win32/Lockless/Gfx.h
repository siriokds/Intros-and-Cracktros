/*----------------------------------------------------------------*/
void 	copyROW16(char *src, char *dst)
/*----------------------------------------------------------------*/
{
	_asm
	{
		mov esi,src
		mov	edi,dst
		mov	eax,[esi]
		mov	[edi],eax
		mov	eax,[esi+4]
		mov	[edi+4],eax
		mov	eax,[esi+8]
		mov	[edi+8],eax
		mov	eax,[esi+12]
		mov	[edi+12],eax
	}
}



/*----------------------------------------------------------------*/
void 	fastSet(UBYTE *dst, int val, int todo)
/*----------------------------------------------------------------*/
{
	_asm
	{
		pushad
		mov		edi,dst
		mov		eax,val
		mov		ah,al
		push	ax
		shl		eax,16
		pop		ax
		mov		ecx,todo
		push	ecx
		shr		ecx,2
		cld
		rep		stosd
		pop		ecx
		and		ecx,3
		rep		stosb
		popad
	}
}


/*----------------------------------------------------------------*/
void 	fastCopy(UBYTE *dst, UBYTE *src, int todo)
/*----------------------------------------------------------------*/
{
	_asm
	{
		pushad
		mov		esi,src
		mov		edi,dst
		mov		ecx,todo
		push	ecx
		shr		ecx,2
		cld
		rep		movsd
		pop		ecx
		and		ecx,3
		rep		movsb
		popad
	}
}

/*----------------------------------------------------------------*/
void 	fastMove(UBYTE *dst, UBYTE *src, int todo)
/*----------------------------------------------------------------*/
{
	_asm
	{
		pushad

		mov		esi,src
		mov		edi,dst
		mov		ecx,todo
		rep		movsb

		popad
	}
}





/*----------------------------------------------------------------*/
int		getRand(void)
/*----------------------------------------------------------------*/
{
static	int	m_RND = 0x6A91A32B;

	m_RND = ((m_RND << 7) + (m_RND >> 25) + 0x6ECA756D) ^ 0x9E59A92B;
	return m_RND;
}




