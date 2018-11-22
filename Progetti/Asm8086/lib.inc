;Library for general purposes
;Author  : Racheli Alessandro
;Version : 1.1
graphic_mode macro
	PUSH AX
	MOV AL, 13H
	MOV AH, 0H
	INT 10H
	POP AX
	ENDM
	
text_mode macro
	PUSH AX
	MOV AL, 03H
	MOV AH, 0H
	INT 10H
	POP AX
	ENDM
	
open_file MACRO HANDLE, FILENAME
	PUSH DX
	MOV AL, 0
	MOV AH, 3DH
	LEA DX, FILENAME
	INT 21H
	POP DX
	MOV HANDLE, AX
	ENDM
	
read_file MACRO HANDLE, DEST, BYTES, EFF_READ
	PUSH BX
	PUSH CX
	PUSH DX
	MOV DX, DEST
	MOV BX, HANDLE
	MOV CX, BYTES
	MOV AH, 3FH
	INT 21H
	POP DX
	POP CX
	POP BX
	MOV EFF_READ, AX
	ENDM
	
a_capo  macro
		PUSH AX
		PUSH DX
		MOV AH, 02H
		MOV DL, 0DH
		INT 21H
		MOV DL, 0AH
		INT 21H
		POP DX
		POP AX
endm

get_ch  macro
		PUSH CX
		MOV CH, AH
		MOV AH, 01H
		INT 21H
		MOV AH, CH
		POP CX
endm
		
prt_al  macro
		PUSH AX
		PUSH DX
		MOV AH, 02H
		MOV DL, AL
		INT 21H
		POP DX
		POP AX
endm

prt_ch  macro
		PUSH AX
		MOV AH, 02H
		INT 21H
		POP AX
endm

play_b  macro
		PUSH AX
		PUSH DX
		MOV AH, 02H
		MOV DL, 07H
		INT 21H
		POP DX
		POP AX
endm

COLOR_PIXEL  macro RIG, COL, COLOR
		PUSH AX
		PUSH CX
		PUSH DX
		MOV AH, 0CH
		MOV CX, COL
		MOV DX, RIG
		MOV AL, COLOR
		INT 10H
		POP DX
		POP CX
		POP AX
endm