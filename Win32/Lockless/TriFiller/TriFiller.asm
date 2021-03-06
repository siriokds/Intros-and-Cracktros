; TRIFILLER



;============
SECTION .bss
;============


struc	gdatas

	.minX		resd 1
	.maxX		resd 1
	.minY		resd 1
	.maxY		resd 1

	.buffer		resd 1
	.pitch		resd 1
	.col		resd 1

	.a_x		resd 1
	.a_y		resd 1
	.b_x		resd 1
	.b_y		resd 1
	.c_x		resd 1
	.c_y		resd 1

	.SLR		resd 1024
	
	.size
endstruc



;- - - - - - - - - - - - - - - - - - - - - - - 

global _TriDraw@4		; void	TriDraw(int *triSetting);
global _TriGet@4

gdata:	resb gdatas.size

;============
SECTION .code
;============

%macro	swapP	4
	mov	eax,[ebp + gdatas.%2]
	mov	ebx,[ebp + gdatas.%4]
	cmp	eax,ebx
	jle	short %%sk
	mov	[ebp + gdatas.%2],ebx
	mov	[ebp + gdatas.%4],eax
	mov	eax,[ebp + gdatas.%1]
	mov	ebx,[ebp + gdatas.%3]
	mov	[ebp + gdatas.%1],ebx
	mov	[ebp + gdatas.%3],eax
%%sk:
%endmacro




%macro	calcInc	5
	push	eax
	push	ecx
	push	edx
	mov	eax,[ebp + gdatas.%5]
	sub	eax,[ebp + gdatas.%3]
	jz	short %%ezero
	mov	ecx,eax
	mov	eax,[ebp + gdatas.%4]
	sub	eax,[ebp + gdatas.%2]
	xor	edx,edx
	shl	eax,16
	cdq
	idiv	ecx
%%ezero:
	mov	%1,eax
	pop	edx
	pop	ecx
	pop	eax
%endmacro


;---------------------------------------------
HLine:
;---------------------------------------------
	pushad

	mov	ebx,[esi + eax*8]
	mov	edx,[esi + eax*8 + 4]

	cmp	ebx,edx
	jle	short .noswp
	
	mov	ecx,ebx
	mov	ebx,edx
	mov	edx,ecx
.noswp:

	cmp	ebx,[ebp + gdatas.maxX]		; if ((x1 > maxX) || (x2 < minX)) return;
	jg	short .exit
	cmp	edx,[ebp + gdatas.minX]
	jl	short .exit
	
	
	mov	ecx,[ebp + gdatas.minX]
	cmp	ebx,ecx				; if (x1 < minX) x1 = minX;
	jg	short .naX1
	mov	ebx,ecx
.naX1:
	mov	ecx,[ebp + gdatas.maxX]
	cmp	edx,ecx				; if (x2 > maxX) x2 = maxX;
	jle	short .naX2
	mov	edx,ecx
.naX2:

        mov	ecx,edx
        sub	ecx,ebx
        inc	ecx
        jle	short .exit
        
	mov	eax,[ebp + gdatas.col]
	add	edi,ebx				; +X
	
	cmp	eax,-1
	je	.xoring
	
	push	ecx
	shr	ecx,2
	cld
	rep	stosd
	pop	ecx
	and	ecx,3
	cld
	rep	stosb
	jmp	short .exit
	
.xoring:
	dec	ecx
	jz	.exit
.xorng:
	cmp	byte [edi],63
	jne	short .zz
	mov	byte [edi],127
	jmp	short .cz
.zz:	
	cmp	byte [edi],127
	jne	short .cz
	mov	byte [edi],63
.cz:
	inc	edi
	dec	ecx
	jnz	short .xorng

.exit:
	popad
	ret


;---------------------------------------------
sortPoints:
;---------------------------------------------
	pushad

	swapP	a_x, a_y, b_x, b_y
	swapP	a_x, a_y, c_x, c_y
	swapP	b_x, b_y, c_x, c_y

	popad
	ret



;---------------------------------------------
;---------------------------------------------
doTria:
;---------------------------------------------
;---------------------------------------------

	calcInc	esi, a_x, a_y, c_x, c_y		; esi = incEnd
	
	mov	edx,[ebp + gdatas.a_x]
	shl	edx,16				; edx = EndX
	
	lea	edi,[ebp + gdatas.SLR]

;--------------
; CALC UP-TRIA
;--------------
;.Tria1:
	mov	eax,[ebp + gdatas.a_y]
	cmp	eax,[ebp + gdatas.b_y]
	jge	short .Tria2

	calcInc	ebx, a_x, a_y, b_x, b_y		; ebx = incSta

	mov	ecx,[ebp + gdatas.a_x]
	shl	ecx,16				; ecx = StaX

.T1_calc:
	push	eax
	
	cmp	eax,[ebp + gdatas.minY]
	jl	short .T1_inc
	cmp	eax,[ebp + gdatas.maxY]
	jg	short .T1_inc

	push	ecx
	sar	ecx,16
	mov	[edi + eax*8],ecx			; SL[i] = StaX >> 16;
	pop	ecx

	push	edx
	sar	edx,16
	mov	[edi + eax*8 + 4],edx			; SR[i] = EndX >> 16;
	pop	edx

.T1_inc:
	add	ecx,ebx
	add	edx,esi

	pop	eax
	inc	eax
	cmp	eax,[ebp + gdatas.b_y]
	jl	short .T1_calc


;--------------
; CALC DN-TRIA
;--------------
.Tria2:
	mov	eax,[ebp + gdatas.b_y]
	cmp	eax,[ebp + gdatas.c_y]
	jge	short .drawTria

	calcInc	ebx, b_x, b_y, c_x, c_y		; ebx = incSta

	mov	ecx,[ebp + gdatas.b_x]
	shl	ecx,16				; ecx = StaX

.T2_calc:
	push	eax
	
	cmp	eax,[ebp + gdatas.minY]
	jl	short .T2_inc
	cmp	eax,[ebp + gdatas.maxY]
	jg	short .T2_inc

	push	ecx
	sar	ecx,16
	mov	[edi + eax*8],ecx			; SL[i] = StaX >> 16;
	pop	ecx

	push	edx
	sar	edx,16
	mov	[edi + eax*8 + 4],edx			; SR[i] = EndX >> 16;
	pop	edx
.T2_inc:
	add	ecx,ebx
	add	edx,esi

	pop	eax
	inc	eax
	cmp	eax,[ebp + gdatas.c_y]
	jl	short .T2_calc


;---------------------------------------------------
.drawTria:
	mov	esi,edi
	mov	edi,[ebp + gdatas.buffer]
	
	mov	eax,[ebp + gdatas.a_y]
	mov	ecx,[ebp + gdatas.minY]
	cmp	eax,ecx
	jge	short .nAJ
	mov	eax,ecx
.nAJ:

	mov	ebx,[ebp + gdatas.c_y]
	mov	ecx,[ebp + gdatas.maxY]
	cmp	ebx,ecx
	jle	short .nAJB
	mov	ebx,ecx
.nAJB:

	mov	edx,[ebp + gdatas.pitch]

	push	eax
	push	edx
	mov	ecx,edx
	xor	edx,edx
	mul	ecx
	add	edi,eax
	pop	edx
	pop	eax

	
.nextRow:
	call	HLine
	
	add	edi,edx
	
	inc	eax
	cmp	eax,ebx
	jl	short .nextRow
.exit:
	ret











;---------------------------------------------
_TriGet@4:
;---------------------------------------------
	push	ebp
	mov	ebp,esp
	pushad

	lea	esi,[gdata]
	mov	edi,[ebp + 8]
	mov	ecx,4+3+6+1024
	cld
	rep	movsd

	popad
	pop	ebp
	xor	eax,eax
	ret     4h


;---------------------------------------------
_TriDraw@4:
;---------------------------------------------
	push	ebp
	mov	ebp,esp
	pushad

	mov	esi, [ebp + 8]
	lea	ebp, [gdata]

	mov	eax,[esi]
	mov	[ebp + gdatas.minX],eax
	mov	eax,[esi + 4]
	mov	[ebp + gdatas.maxX],eax
	mov	eax,[esi + 8]
	mov	[ebp + gdatas.minY],eax
	mov	eax,[esi + 12]
	mov	[ebp + gdatas.maxY],eax
	
	mov	eax,[esi + 16]
	mov	[ebp + gdatas.buffer],eax
	mov	eax,[esi + 20]
	mov	[ebp + gdatas.pitch],eax
	
	mov	eax,[esi + 24]
	cmp	eax,-1
	je	.nosetcol
	mov	ah,al
	push	ax
	shl	eax,16
	pop	ax
.nosetcol:
	mov	[ebp + gdatas.col],eax

	mov	eax,[esi + 28]
	mov	[ebp + gdatas.a_x],eax
	mov	eax,[esi + 32]
	mov	[ebp + gdatas.a_y],eax
	mov	eax,[esi + 36]
	mov	[ebp + gdatas.b_x],eax
	mov	eax,[esi + 40]
	mov	[ebp + gdatas.b_y],eax
	mov	eax,[esi + 44]
	mov	[ebp + gdatas.c_x],eax
	mov	eax,[esi + 48]
	mov	[ebp + gdatas.c_y],eax
	
;-------

	call	sortPoints
	
	mov	eax,[ebp + gdatas.a_y]
	cmp	eax,[ebp + gdatas.maxY]
	jg	short .end
	
	mov	eax,[ebp + gdatas.c_y]
	cmp	eax,[ebp + gdatas.minY]
	jl	short .end

	call	doTria

.end:
	popad
	pop	ebp
	xor	eax,eax
	ret     4h




