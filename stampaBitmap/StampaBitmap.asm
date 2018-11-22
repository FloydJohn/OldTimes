;Racheli Alessandro

include emu8086.inc
include lib.inc

org  100h

graphic_mode

open_file BX, filename 

MOV AX, 1  

MOV CX, 1000h

leggi:       

CMP AX, 1

JNE cont

read_file BX, CX, 1, AX  

INC CX      

JMP leggi 

cont:    

;Salvo start, height, width   

MOV BX, 100AH

MOV CX, [BX]

MOV bmp_start, CX  

   

MOV BX, 1012H
             
MOV CX, [BX]
MOV DX, [BX+4]   

MOV bmp_width, CX
MOV bmp_height, DX


MOV BX, bmp_start

ADD BX, 1000h

MOV CX, 0

SUB DX, 1

INIZIO:

MOV AL, [BX]

CONTINUA:

CMP AL, 10000000b ;Il pixel in questione e' bianco?

JB LAB1          ;Se si, vado avanti e non stampo niente

color_pixel DX, CX, 0Fh ;Altrimenti stampo un pixel bianco

LAB1:  

SHL AL, 1  ;Ora leggo il BIT successivo

INC CX   ;Aumento la mia posizione sulla colonna

INC i2  ;Aumento il conto dei bit gia' letti in questa locazione

CMP i2, 8  ;Ne ho gia' letti 8 ?

JE leggi_nuovo ;Se si, allora ne carico altri 8

ret_leggi_nuovo:

CMP CX, bmp_width ;Bona, ora sono arrivato alla fine di sta cazzo di riga o no?

JNE CONTINUA ;No? Allora altro pixel!

;Ho finito la riga! E ora? 

XOR CX, CX ;Azzeriamo la colonna...

;Per adesso non ho implementato dimensioni diverse da multipli di 4, lo faro'!

CMP DX, 0 ;Ho finito anche le righe? 

JE fine ;Ho finito!

DEC DX ;Non ho finito, saliamo di una riga...

JMP CONTINUA ;Continuiamo!

fine:  

get_ch

text_mode           

RET                               


leggi_nuovo:;Ho finito di stampare gli 8 bit identificati dalla cella di memoria

     INC BX ;Leggo nella posizione successiva
     
     MOV AL, [BX] ;Carico altri 8 bit in AL 
     
     MOV i2, 0    ;Azzero il contatore di bit
     
     JMP ret_leggi_nuovo          ;Sono pronto per stampare altri 8 bit
     
      
    

filename db "img.bmp",0           

bmp_height DD ?
bmp_width DD ?
bmp_start DW ?

i DB 0 ;counter
i2 DB 0;2o counter

