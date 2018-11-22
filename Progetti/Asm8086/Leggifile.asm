
include emu8086.inc

org 100h

;Apro il file

MOV AL, 0 ;READ

MOV AH, 3Dh

LEA DX, filename  ;Metto il nome del file in dx

INT 21h

JC  error


;STAMPO IL PRIMO CARATTERE

 
MOV BX, AX 

MOV CX, 1


leggi:

 
MOV DX, 1000h ;Indirizzo di memoria dove sara' caricato il primo carattere

MOV AH, 3Fh

INT 21h 

CMP CX, AX

JNE continua

MOV AH, 02h

MOV DL, [1000h]

INT 21h ;Stampo il numero

JMP leggi

 

continua:


MOV AH, 3Eh

INT 21h

ret
             
error:

print 'Error, fuck you'

ret             
             
filename db "testo.txt",0
