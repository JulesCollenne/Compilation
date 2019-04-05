%include	'io.asm'

section	.bss
sinput:	resb	255	;reserve a 255 byte space in memory for the users input string
v$a:	resd	1

section	.text
global _start
_start:
	call	fmain
	mov	eax, 1		 ; 1 est le code de SYS_EXIT
	int	0x80		 ; exit
fmain:
	push	ebp		 ; sauvegarde la valeur de ebp
	mov	ebp, esp		 ; nouvelle valeur de ebp
	mov	dword [v$a], 123		 ; affectation stocke valeur
	pop	ebp		 ; restaure la valeur de ebp
	ret
