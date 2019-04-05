%include	'io.asm'

section	.bss
sinput:	resb	255	;reserve a 255 byte space in memory for the users input string

section	.text
global _start
_start:
	call	fmain
	mov	eax, 1		 ; 1 est le code de SYS_EXIT
	int	0x80		 ; exit
fmax:
	push	ebp		 ; sauvegarde la valeur de ebp
	mov	ebp, esp		 ; nouvelle valeur de ebp
	mov	ebx, -1		 ; affectation stocke valeur
	mov	ecx, dword [ebp + 12]
	cmp	ecx, dword [ebp + 8]
	jl	e2		 ; saut
	mov	ebx, 0		 ; affectation stocke valeur
e2:
	cmp	ebx, 0
	je	e1		 ; saut
	mov	[ebp + 16], dword [ebp + 8]		 ; ecriture de la valeur de retour
	pop	ebp		 ; restaure la valeur de ebp
	ret
e1:
	mov	[ebp + 16], dword [ebp + 12]		 ; ecriture de la valeur de retour
	pop	ebp		 ; restaure la valeur de ebp
	ret
	pop	ebp		 ; restaure la valeur de ebp
	ret
fmain:
	push	ebp		 ; sauvegarde la valeur de ebp
	mov	ebp, esp		 ; nouvelle valeur de ebp
	sub	esp, 4	; allocation variable locale v$v_1
	sub	esp, 4	; allocation variable locale v$v_2
	mov	eax, sinput
	call	readline
	mov	eax, sinput
	call	atoi
	mov	dword [ebp - 4], eax		 ; affectation stocke valeur
	mov	eax, sinput
	call	readline
	mov	eax, sinput
	call	atoi
	mov	dword [ebp - 8], eax		 ; affectation stocke valeur
	sub	esp, 4		 ; allocation valeur de retour
	push	dword [ebp - 4]		 ; empile argument
	push	dword [ebp - 8]		 ; empile argument
	call	fmax
	add	esp, 8		; desallocation parametres
	pop	ebx		 ; récupère valeur de retour
	mov	ecx, -1		 ; affectation stocke valeur
	cmp	ebx, dword [ebp - 4]
	je	e5		 ; saut
	mov	ecx, 0		 ; affectation stocke valeur
e5:
	cmp	ecx, 0
	je	e3		 ; saut
	mov	eax, dword [ebp - 4]
	call	iprintLF
	jmp	e4
e3:
	mov	eax, dword [ebp - 8]
	call	iprintLF
e4:
	add	esp, 8	; desallocation variables locales
	pop	ebp		 ; restaure la valeur de ebp
	ret
