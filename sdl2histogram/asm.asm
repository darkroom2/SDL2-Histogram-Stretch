section .data
width: dd 0
height: dd 0
;min: dd 255
;max: dd 0

section .text
bits 32
global _function
_function:
;eax -height
;ecx -width
;edx -pixels
	push ebp
	mov ebp, esp
	mov [width], ecx
	mov [height], eax
	mov ecx, 0
iLoop:
	cmp ecx, [width]
	je done
	mov ebx, 0
jLoop:
	;do stuff there
	mov byte[edx], 55
	add edx, 1

	cmp ebx, [height]
	je jLoopDone
	inc ebx
	jmp jLoop
jLoopDone:
	inc ecx
	jmp iLoop
done:
	leave
	ret