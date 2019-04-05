%include	'io.asm'

section	.bss
sinput:	resb	255	;reserve a 255 byte space in memory for the users input string
v$a:	resd	1
v$b:	resd	1

section	.text
global _start
_start:
	call	fmain
	mov	eax, 1		 ; 1 est le code de SYS_EXIT
	int	0x80		 ; exit
fmain:
	push	ebp		 ; sauvegarde la valeur de ebp
	mov	ebp, esp		 ; nouvelle valeur de ebp
	mov	eax, sinput
	call	readline
	mov	eax, sinput
	call	atoi
	mov	dword [v$a], eax		 ; affectation stocke valeur
	mov	eax, sinput
	call	readline
	mov	eax, sinput
	call	atoi
	mov	dword [v$b], eax		 ; affectation stocke valeur
	mov	ebx, dword [v$a]
	add	ebx, dword [v$b]
	mov	eax, ebx		 ; new reg for t
	call	iprintLF
	pop	ebp		 ; restaure la valeur de ebp
	ret
