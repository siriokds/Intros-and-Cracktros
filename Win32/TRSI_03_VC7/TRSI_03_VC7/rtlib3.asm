SECTION .text

global      __mfloor@8
__mfloor@8:
        fld	qword [esp+4]
        push	ecx
        mov	ecx,esp
        fnstsw	[ecx]
        mov	ax,[ecx]
        and	ah,0F7h
        or	ah,004h
        mov	[ecx+2],ax
        fldcw	[ecx+2]
        frndint
        fldcw	[ecx]
        pop	ecx
        ret


global      __mlog@8
__mlog@8:
        fldln2
        fld	qword [esp+4]
        fyl2x
        ret

global      __msin@8
__msin@8:
	fld	qword [esp+4]
        fsin
        ret	8


global      __mcos@8
__mcos@8:
	fld	qword [esp+4]
        fcos
        ret	8


global      __mpow@16
__mpow@16:
	fld	qword [esp+12]
	fld	qword [esp+4]
	ftst
	fstsw	ax
	sahf
	jz	.zero
	fyl2x
	fld1
	fld	st1
	fprem
	f2xm1
	faddp	st1, st0
	fscale
.zero:	fstp	st1
	ret	16
    
    
    
    
; ---- float->int
global		__ftol
__ftol:
	sub	esp, byte 8
 	fistp	qword [esp]
 	pop	eax
 	pop	edx
 	ret
