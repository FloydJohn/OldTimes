#include <allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "C:\Users\Flavio\Desktop\kjhkjn\flavio_allegro\flavio_allegro.h"
//////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------->COSTANTI
#define Wx 800
#define Wy 600

////////////////////////////////////////// 
//////////////FUNZIONI////////////////////
//---------------------------------------------------------------------------------------------------------------------------->FUNZIONI

void init();
void deinit();
void stampa_block();
void stampa_curs(void);
void vis_v();
void scrivi();
void memorizza(int z);
void menu_block();
void c_menu_block();
void base();
void c_base();
void arancia();
void c_arancia();
void ponzio();
void c_ponzio();
void finec();



//---------------------------------------------------------------------------------------------------------------------------->BITMAP
BITMAP *buffer=NULL,*piano_pr=NULL,*blocchi=NULL,*ponzioim=NULL;
FILE *map;
//---------------------------------------------------------------------------------------------------------------------------->VARIABILI
int posx,posy;
int a;
int num_fin=30;
int spost=0;
int Gx=40,Gy=60;
int pon=FALSE;

int v=0; //velocita arance
int vb=0; //velocita basi

int leggi_v=FALSE;

int const databl[50][6]={{0,0,20,10,0,0},{21,0,26,30,0,10},{48,0,26,30,0,10},{0,11,20,10,0,0}};

struct quad{
       int vel;
       int tipo;
       int coo[4];
       int grand[2];
       }block[40*30][60];
//////////////////////////////////////////
//////////////MAIN////////////////////////
//////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------->MAIN
int main() {
    int i,j;
    int k,h;
	init();
	
	ponzioim=load_bitmap("ponzio.bmp",NULL);
    buffer=create_bitmap(Wx,Wy);
    piano_pr=create_bitmap(num_fin*Wx,Wy);
    blocchi=load_bitmap("blocchi.bmp",NULL);
    
    map=fopen("map.dat","r");
    if(map!=NULL){
    
    for(j=0;j<60;j++)                 
	   for(i=0;i<40*num_fin;i++)
	      fread(&block[i][j],sizeof(struct quad),1,map);
    }
    
    else map=fopen("map.dat","w");
    
    fclose(map);
	      
    i=0;
    j=0;
    stampa_block();
        
	while (!key[KEY_ESC]) {//------------------------------------------------------------------------------------------------>INIZIO CICLO
	
	
    a=labs(mouse_z%10);
    posx=mouse_x-mouse_x%(Wx/Gx);
    posy=mouse_y-mouse_y%(Wy/Gy);
    
    if(key[KEY_LEFT]&&spost>0)spost-=20;
    if(key[KEY_RIGHT]&&spost<20*40*29)spost+=20;
    
    
	if(key[KEY_C]&&key[KEY_A]&&key[KEY_N]){
        for(k=0;k<60;k++)                 
	       for(h=0;h<40*num_fin;h++)
              block[h][k].tipo=0;
              
        rectfill(piano_pr,0,0,Wx*num_fin,Wy,makecol(0,0,0));
    }
        
    if(key[KEY_I])spost=0;
    if(key[KEY_F])spost=20*40*29;
    if(key[KEY_S]&&key[KEY_1])spost=20*40*29/10;
    if(key[KEY_S]&&key[KEY_2])spost=20*40*29/10*2;
    if(key[KEY_S]&&key[KEY_3])spost=20*40*29/10*3;
    if(key[KEY_S]&&key[KEY_4])spost=20*40*29/10*4;
    if(key[KEY_S]&&key[KEY_5])spost=20*40*29/10*5;
    if(key[KEY_S]&&key[KEY_6])spost=20*40*29/10*6;
    if(key[KEY_S]&&key[KEY_7])spost=20*40*29/10*7;
    if(key[KEY_S]&&key[KEY_8])spost=20*40*29/10*8;
    if(key[KEY_S]&&key[KEY_9])spost=20*40*29/10*9;
    
    if(key[KEY_V]&&key[KEY_LCONTROL]){
        while(!key[KEY_ENTER]){
        textprintf_ex(screen, font, posx-30,posy-10 ,makecol(255,255,0), makecol(0,0,0),"scegli la velocita':");
        textprintf_ex(screen, font, posx+26,posy+30 ,makecol(255,255,0), makecol(0,0,0),"%3d",vb);
        if(key[KEY_UP])vb++;
        if(key[KEY_DOWN])vb--;
        rest(100);
        }
     }
     
    if(key[KEY_V])block[posx/20][posy/10].vel=vb;
        
    blit(piano_pr,buffer,spost,0,0,0,Wx,Wy);
    
    stampa_curs();
    if(mouse_b==1)
	menu_block();
	if(mouse_b==2)
	c_menu_block();
    
	
	if(key[KEY_V]&&key[KEY_LSHIFT])leggi_v=TRUE;
	if(key[KEY_V]&&key[KEY_RSHIFT])leggi_v=FALSE;
	if(leggi_v==TRUE)vis_v();
    blit(buffer,screen,0,0,0,0,Wx,Wy);
    
    
    rest(15);
	}//----------------------------------------------------------------------------------------------------------------------->FINE CICLO
	
    
    map=fopen("map.dat","r+");
    
	for(j=0;j<Gy;j++)
	   for(i=0;i<num_fin*Gx;i++)
          fwrite(&block[i][j],sizeof(struct quad),1,map);
	
	fclose(map);
	
	
	
	              
	/*for(j=0;j<30;j++){                   PROVA
       fprintf(map,"\n");
	   for(i=0;i<40;i++)
	      fprintf(map,"%d",block[i][j].tipo);
    }*/
//////////////////////////////////////////	
//////deinizzializzazione(FINE)///////////
	deinit();
	return 0;
}
END_OF_MAIN()
//////fine del main///////////////////////----------------------------------------------------------------------------------->FINE MAIN
//////////////////////////////////////////
//////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------------------->INIT()
void init() {
	
	allegro_init();
	
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, Wx, Wy, 0, 0);

	install_keyboard();
    install_timer();
	install_mouse();
	enable_hardware_cursor();
}

//------------------------------------------------------------------------------------------------------------------------->DEINIT()
void deinit() {
	clear_keybuf();
	
	//----------------------------DESTROY_BITMAP
	destroy_bitmap(buffer);
	destroy_bitmap(blocchi);
	destroy_bitmap(piano_pr);
	destroy_bitmap(ponzioim);
	//||||||||||||||||||||||||||||||||||||||||||
	allegro_exit();
	
	
}
     
void stampa_curs(void){
    if(a==3)masked_blit(ponzioim,buffer,0,0,posx,posy+3,27,37);
    else if(a==4)
    rectfill(buffer,posx,posy,posx+19,posy+9,makecol(255,0,254));
    else {
    masked_blit(blocchi,buffer,databl[a][0],databl[a][1],posx+databl[a][4],posy+databl[a][5],databl[a][2],databl[a][3]);
    rect(buffer,posx+databl[a][4],posy+databl[a][5],posx+databl[a][2]+databl[a][4],posy+databl[a][3]+databl[a][5],makecol(0,0,0));
    }
}
void scrivi(){
     masked_blit(blocchi,piano_pr,databl[a][0],databl[a][1],posx+databl[a][4]+spost,posy+databl[a][5],databl[a][2],databl[a][3]);
}
void memorizza(int z){
     block[posx/20+spost/20][posy/10].tipo=z+1;
     }
void cancella(){
     rectfill(piano_pr,posx+spost,posy,posx+20-1+spost,posy+10-1,makecol(0,0,0));
     block[posx/20+spost/20][posy/10].vel=0;
     }
void menu_block(){
     void(*menu[5])()={base,arancia,arancia,ponzio,finec};
     if(a<5)
     if(block[posx/20+spost/20][posy/10].tipo==0)
     (*menu[a])();
     }
void base(){
     scrivi();
     memorizza(a);
     }
void arancia(){
     
     scrivi();
     while(!key[KEY_ENTER]){
     textprintf_ex(screen, font, posx-30,posy-10 ,makecol(0,255,0), makecol(0,0,0),"scegli la velocita':");
     textprintf_ex(screen, font, posx+26,posy+30 ,makecol(0,255,0), makecol(0,0,0),"%3d",v);
     if(key[KEY_UP])v++;
     if(key[KEY_DOWN])v--;
     rest(100);
     }
     block[posx/20+spost/20][posy/10].vel=v;
     memorizza(a);
     posy+=10;
     memorizza(99-1);
     posy+=10;
     memorizza(99-1);
     posy+=10;
     memorizza(99-1);
     posx+=20;
     memorizza(99-1);
     posy-=10;
     memorizza(99-1);
     posy-=10;
     memorizza(99-1);
     posy-=10;
     memorizza(99-1);
     posx-=20;
     }
void ponzio(){
     if(pon==FALSE){
     masked_blit(ponzioim,piano_pr,0,0,posx+spost,posy+3,27,37);
     memorizza(a);
     posy+=10;
     memorizza(99-1);
     posy+=10;
     memorizza(99-1);
     posy+=10;
     memorizza(99-1);
     posx+=20;
     memorizza(99-1);
     posy-=10;
     memorizza(99-1);
     posy-=10;
     memorizza(99-1);
     posy-=10;
     memorizza(99-1);
     posx-=20;
     pon=TRUE;
     }
     }
void finec(){
     rectfill(piano_pr,posx+spost,posy,posx+19+spost,posy+9,makecol(255,0,254));
     memorizza(a);
     }

void c_menu_block(){
     void(*menu[5])()={c_base,c_arancia,c_arancia,c_ponzio,c_base};
     if(a<5)
     (*menu[a])();
     }
void c_base(){
     if(block[posx/20+spost/20][posy/10].tipo==1||block[posx/20+spost/20][posy/10].tipo==5||block[posx/20+spost/20][posy/10].tipo==0){
     cancella();
     memorizza(-1);
     }
     }
void c_arancia(){
     if(block[posx/20+spost/20][posy/10].tipo==2||block[posx/20+spost/20][posy/10].tipo==3){
     block[posx/20+spost/20][posy/10].vel=0;
     cancella();
     memorizza(-1);
     posy+=10;
     cancella();
     memorizza(-1);
     posy+=10;
     cancella();
     memorizza(-1);
     posy+=10;
     cancella();
     memorizza(-1);
     posx+=20;
     cancella();
     memorizza(-1);
     posy-=10;
     cancella();
     memorizza(-1);
     posy-=10;
     cancella();
     memorizza(-1);
     posy-=10;
     cancella();
     memorizza(-1);
     posx-=20;
     }
}
void c_ponzio(){
     if(block[posx/20+spost/20][posy/10].tipo==4){
     cancella();
     memorizza(-1);
     posy+=10;
     cancella();
     memorizza(-1);
     posy+=10;
     cancella();
     memorizza(-1);
     posy+=10;
     cancella();
     memorizza(-1);
     posx+=20;
     cancella();
     memorizza(-1);
     posy-=10;
     cancella();
     memorizza(-1);
     posy-=10;
     cancella();
     memorizza(-1);
     posy-=10;
     cancella();
     memorizza(-1);
     posx-=20;
     pon=FALSE;
     }
     }
void stampa_block(){
     
     int i,j;
     
     for(j=0;j<60;j++)                 
	   for(i=0;i<40*num_fin;i++){
     if(block[i][j].tipo==-1)continue;
     if(block[i][j].tipo==99)continue;
     if(block[i][j].tipo==2||block[i][j].tipo==3){
     if(block[i][j].vel<0)block[i][j].tipo=2;
     if(block[i][j].vel>0)block[i][j].tipo=3;
     }
     masked_blit(blocchi,piano_pr,databl[block[i][j].tipo-1][0],databl[block[i][j].tipo-1][1],i*20+databl[block[i][j].tipo-1][4],j*10+databl[block[i][j].tipo-1][5],databl[block[i][j].tipo-1][2],databl[block[i][j].tipo-1][3]);
     }
     }
     
void vis_v(){
     textprintf_ex(buffer, font, posx-5,posy-8 ,makecol(255,255,0), makecol(0,0,0),"v=%d",block[posx/20][posy/10].vel);
     }
     
