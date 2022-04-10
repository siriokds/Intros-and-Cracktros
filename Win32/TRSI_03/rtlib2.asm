;		"Art" - a 64K demo by Farbrausch and Scoopex
;
;		RTLIB2.ASM - small math run time library replacement
;
;		Written by:
;   Ryg/FR, kb/FR - code

bits        32

section		.data
einhalb		dd 0.5

section		.text


; ---- sF64 sin(sF64 a)

global      _sin@8
_sin@8:
	fld		qword [esp+4]
	fsin
	ret		8

; ---- sF64 cos(sF64 a)

global      _cos@8
_cos@8:
	fld		qword [esp+4]
	fcos
	ret		8

    
; ---- float->int
global		__ftol
__ftol:
	sub		esp, byte 8
 	fistp	qword [esp]
 	pop		eax
 	pop		edx
 	ret


global __chkesp
__chkesp:
  ret
