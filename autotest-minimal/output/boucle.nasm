%include	'io.asm'

section	.bss
sinput:	resb	255	;reserve a 255 byte space in memory for the users input string
v$i:	resd	1
v$carre:	resd	1

section	.text
global _start
_start:
	call	fmain
	mov	eax, 1		 ; 1 est le code de SYS_EXIT
	int	0x80		 ; exit
fmain:
	push	ebp		 ; sauvegarde la valeur de ebp
	mov	ebp, esp		 ; nouvelle valeur de ebp
	mov	dword [v$i], 0		 ; affectation stocke valeur
e0:
	mov	ebx, -1		 ; affectation stocke valeur
	cmp	dword [v$i], 10
	jl	e2		 ; saut
	mov	ebx, 0		 ; affectation stocke valeur
e2:
	cmp	ebx, 0
	je	e1		 ; saut
	mov	ebx, dword [v$i]
	imul	ebx, dword [v$i]
	mov	dword [v$carre], ebx		 ; affectation stocke valeur
	mov	eax, dword [v$carre]
	call	iprintLF
	mov	ebx, dword [v$i]
	add	ebx, 1
	mov	dword [v$i], ebx		 ; affectation stocke valeur
	jmp	e0
e1:
	pop	ebp		 ; restaure la valeur de ebp
	ret
