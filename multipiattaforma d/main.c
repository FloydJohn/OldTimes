#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
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


void acq_block();
void nulla(int i,int j);
void base(int i,int j);
void arancia(int i,int j);
void aranciainv(int i,int j);
void ponziof(int i,int j);
void finec(int i,int j);

void stampa_block();
void limiti_collisione();
void mov_ponzio();
void agg_passato();
void stampa_ponzio();
void mov_blocchi();
void pro();
void iniz_pro();
//---------------------------------------------------------------------------------------------------------------------------->BITMAPS
BITMAP *ponzio=NULL,*ponzioinv=NULL,*buffer=NULL,*blocchi=NULL;
//---------------------------------------------------------------------------------------------------------------------------->VARIABILI
int const databl[50][6]={{0,0,20,10,0,0},{20,0,26,30,0,10},{47,0,26,30,0,10},{0,0,0,0,0,0},{0,0,0,0,0,0},{74,0,15,9,0,11}};
int limite_s,limite_g,azz;
int direzione=TRUE;
int conta_b;
//collisione
int a,b,c;
//----------
int bl_arancia[1000];  //numero del blocco in cui si trova un arancia

int num_nemici=0;
int y_morte=300;

int num_fin=30;
int spost=0;

int contacicli=0;

int morte=FALSE;
int chiuso=FALSE;

 struct quad{
       int vel;
       int tipo;
       int coo[4];
       int grand[2];
       }lettura[40*30][60],block[100000];
       
//////////////////////////////////////////
//////////////MAIN////////////////////////
//////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------->MAIN
int main(){
	init();
	acq_block();
	ponzio=load_bitmap("ponzio.bmp",NULL);
	ponzioinv=load_bitmap("ponzioinv.bmp",NULL);
	blocchi=load_bitmap("blocchi.bmp",NULL);
	
	buffer=create_bitmap(Wx*num_fin,Wy);
	gravita(0,0,1,0,'s');
	azz=FALSE;
	
	conta_b=conta_b+num_nemici*4;	
	
	
    iniz_pro(); 
    

	while (!key[KEY_ESC]&&chiuso==FALSE) {//------------------------------------------------------------------------------------------------>INIZIO CICLO	
	
    
    contacicli++;
	if(contacicli==100000)contacicli=0;
	
	
	mov_ponzio();
	
	pro();
    
    mov_blocchi();
    
    
    
    limiti_collisione();
 
    stampa_ponzio();
    
    
    stampa_block();
    
    
	spost=block[0].coo[0]-Wx/2+100;
	if(block[0].coo[0]<300)spost=0;
	if(block[0].coo[0]>Wx*num_fin-500)spost=Wx*(num_fin-1);
      
    blit(buffer,screen,spost,0,0,0,Wx,Wy);
    
    agg_passato();
    
    
	
	if(morte==TRUE)break;
    
	rest(15);		
	}//----------------------------------------------------------------------------------------------------------------------->FINE CICLO
	
//////////////////////////////////////////	
//////deinizzializzazione(FINE)///////////
	deinit();
	return 0;
}
END_OF_MAIN();
//////fine del main///////////////////////----------------------------------------------------------------------------------->FINE MAIN
//////////////////////////////////////////
//////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------------------->INIT()
void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, Wx, Wy, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}
    install_keyboard();
	install_timer();
	install_mouse();
}

//------------------------------------------------------------------------------------------------------------------------->DEINIT()
void deinit() {
	clear_keybuf();
	
	//----------------------------DESTROY_BITMAP
	destroy_bitmap(ponzio);
	destroy_bitmap(ponzioinv);
	destroy_bitmap(blocchi);
	destroy_bitmap(buffer);
	//||||||||||||||||||||||||||||||||||||||||||
	allegro_exit();
	
}
void acq_block(){
     
     int i,j;
     
     void(*menu[6])(int,int)={nulla,base,arancia,aranciainv,ponziof,finec};
     
     conta_b=50;
       
     FILE *levmap;
     
     levmap=fopen("map.dat","r");
     
     if(levmap!=NULL){
     
     for(j=0;j<60;j++)                 
	   for(i=0;i<40*num_fin;i++)
	      fread(&lettura[i][j],sizeof(struct quad),1,levmap);
	      
     for(j=0;j<60;j++)                 
	   for(i=0;i<40*num_fin;i++){
	      if(lettura[i][j].tipo==99)continue;
	      (*menu[lettura[i][j].tipo])(i,j);}
     
    } 
    
    else   chiuso=TRUE;  
    
	      
    fclose(levmap);
    }
    
void nulla(int i,int j){
     }
void base(int i,int j){
     block[conta_b].tipo=1;
     block[conta_b].coo[0]=i*20;
     block[conta_b].coo[1]=j*10;
     block[conta_b].grand[0]=20; 
     block[conta_b].grand[1]=10; 
     block[conta_b].vel=lettura[i][j].vel;
     conta_b++;
     }
void arancia(int i,int j){
     num_nemici++;
     block[conta_b].tipo=2;
     block[conta_b].coo[0]=i*20;
     block[conta_b].coo[1]=j*10+databl[2][5];
     block[conta_b].grand[0]=26; 
     block[conta_b].grand[1]=30; 
     block[conta_b].vel=lettura[i][j].vel;
     conta_b++;
     }
void aranciainv(int i,int j){
     num_nemici++;
     block[conta_b].tipo=3;
     block[conta_b].coo[0]=i*20;
     block[conta_b].coo[1]=j*10+databl[2][5];
     block[conta_b].grand[0]=26; 
     block[conta_b].grand[1]=30; 
     block[conta_b].vel=lettura[i][j].vel;
     conta_b++;
     }
void ponziof(int i,int j){
     block[0].tipo=4;
     block[0].coo[0]=i*20;
     block[0].coo[1]=j*10;
     block[0].grand[0]=27; 
     block[0].grand[1]=37; 
     }
void finec(int i,int j){
     block[conta_b].tipo=5;
     block[conta_b].coo[0]=i*20;
     block[conta_b].coo[1]=j*10;
     block[conta_b].grand[0]=20; 
     block[conta_b].grand[1]=10; 
     block[conta_b].vel=lettura[i][j].vel;
     conta_b++;
     }
void stampa_block(){
     int i;
     for(i=50;i<conta_b;i++){
     if(block[i].tipo==-1)continue;
     if(block[i].tipo==2||block[i].tipo==3){
     if(block[i].vel<0)block[i].tipo=2;
     if(block[i].vel>0)block[i].tipo=3;
     }
     masked_blit(blocchi,buffer,databl[block[i].tipo-1][0],databl[block[i].tipo-1][1],block[i].coo[0],block[i].coo[1],databl[block[i].tipo-1][2],databl[block[i].tipo-1][3]);
     }
     }
void limiti_collisione(){
    int d,c,a;
    
    for(d=0;d<conta_b;d++){
     if(block[d].tipo==4||block[d].tipo==2||block[d].tipo==3){
     for(c=50;c<conta_b;c++){
                      
     if (d==c)continue;   
     if(block[d].tipo==-1)continue;  
     if(block[d].tipo==4&&block[c].tipo==5)continue;
     
     
    a=collision(block[d].coo[0],block[d].coo[1],block[d].coo[0]+block[d].grand[0],block[d].coo[1]+block[d].grand[1],
                block[c].coo[0],block[c].coo[1],block[c].coo[0]+block[c].grand[0],block[c].coo[1]+block[c].grand[1]);
    
    
    //su
    if(a==TRUE&&block[d].coo[3]+block[d].grand[1]<=block[c].coo[1]+block[c].grand[1]/2&&block[d].coo[0]+block[d].grand[0]>=block[c].coo[0]&&block[d].coo[0]<=block[c].coo[0]+block[c].grand[0]){
       if(block[d].tipo==4){
       limite_s=TRUE;
       limite_g=TRUE;}
       if(block[c].tipo==2||block[c].tipo==3||block[c].tipo==1)
          block[d].coo[0]+=block[c].vel;
       block[d].coo[1]=ferma_al_limite(block[d].coo[1],block[d].grand[1],-10000 ,block[c].coo[1]-1,'y');
       }
       
       
    //sx
    if(a==TRUE&&block[d].coo[2]+block[d].grand[0]<=block[c].coo[0]+block[c].grand[0]/2&&block[d].coo[1]+block[d].grand[1]>=block[c].coo[1]&&block[d].coo[1]<block[d].coo[1]+block[c].grand[1]){
       block[d].coo[0]=ferma_al_limite(block[d].coo[0],block[d].grand[0],-1000 ,block[c].coo[0]-2,'x');
       if(block[d].tipo!=4){
       block[d].vel=block[d].vel-block[d].vel*2;
       block[c].vel=block[c].vel-block[c].vel*2;}
       }

    //dx
	if(a==TRUE&&block[d].coo[2]>=block[c].coo[0]+block[c].grand[0]-block[c].grand[0]/2&&block[d].coo[1]+block[d].grand[1]>=block[c].coo[1]&&block[d].coo[1]<block[d].coo[1]+block[c].grand[1]){
       block[d].coo[0]=ferma_al_limite(block[d].coo[0],block[d].grand[0],block[c].coo[0]+block[c].grand[0]+2,1000000,'x');
       if(block[d].tipo!=4){
       block[d].vel=block[d].vel-block[d].vel*2;
       block[c].vel=block[c].vel-block[c].vel*2;}
       }


    //giu
    if(a==TRUE&&block[d].coo[3]>=block[c].coo[1]+block[c].grand[1]-block[c].grand[1]/2&&block[d].coo[0]+block[d].grand[0]>=block[c].coo[0]&&block[d].coo[0]<=block[c].coo[0]+block[c].grand[0]){
       block[d].coo[1]=ferma_al_limite(block[d].coo[1],block[d].grand[1],block[c].coo[1]+block[c].grand[1]+1,1000000,'y');
       if(block[d].tipo==4)azz=TRUE;}
    }
    }
    }
}
void mov_ponzio(){
    rectfill(buffer,block[0].coo[0],block[0].coo[1],block[0].coo[0]+block[0].grand[0],block[0].coo[1]+block[0].grand[1],makecol(0,0,0));
    
    
	block[0].coo[0]=lettura_mov(block[0].coo[0],5,'x');
	
	block[0].coo[1]=salto(block[0].coo[1],limite_s,12,azz);
	azz=FALSE;
	
	block[0].coo[1]=gravita(block[0].coo[1],limite_g,3,0,'n');
	
	block[0].coo[1]=ferma_al_limite(block[0].coo[1],block[0].grand[1],0,Wy+y_morte,'y');
	
	if(segna_limite(block[0].coo[1],block[0].grand[1],-10000,Wy+y_morte-70,'y')==TRUE)morte=TRUE;
	
	if(segna_limite(block[0].coo[1],block[0].grand[1],0,Wy+y_morte,'y')==TRUE)azz=TRUE;
	
	block[0].coo[0]=ferma_al_limite(block[0].coo[0],block[0].grand[0],0,Wx*num_fin,'x');
	
	limite_g=segna_limite(block[0].coo[1],block[0].grand[1],-1000,Wy+y_morte,'y');
	limite_s=segna_limite(block[0].coo[1],block[0].grand[1],-1000,Wy+y_morte,'y');
	
}

void agg_passato(){
    int i;
    for(i=0;i<conta_b;i++){     
    if(block[i].tipo==-1)continue;
    block[i].coo[2]=block[i].coo[0];
	block[i].coo[3]=block[i].coo[1];
    }
	
    }
void stampa_ponzio(){
     if(key[KEY_RIGHT])direzione=TRUE;
    else if(key[KEY_LEFT])direzione=FALSE;
    
	if(direzione==TRUE)
	masked_blit(ponzio,buffer,0,0,block[0].coo[0],block[0].coo[1],block[0].grand[0],block[0].grand[1]);
    
    if(direzione==FALSE)
    masked_blit(ponzioinv,buffer,0,0,block[0].coo[0],block[0].coo[1],block[0].grand[0],block[0].grand[1]);
}
void mov_blocchi(){
     
     int i;
     for(i=50;i<conta_b;i++){
     if(block[i].tipo==-1)continue;
     if(block[i].vel!=0){
        rectfill(buffer,block[i].coo[0],block[i].coo[1],block[i].coo[0]+block[i].grand[0],block[i].coo[1]+block[i].grand[1],makecol(0,0,0));
        block[i].coo[0]+=block[i].vel;
     }
     }
}
     
void pro(){
        block[conta_b-num_nemici*4+1].tipo=6;
        block[conta_b-num_nemici*4+1].coo[0]=-50;
        block[conta_b-num_nemici*4+1].coo[1]=100;
        block[conta_b-num_nemici*4+1].vel=100;
     }
void iniz_pro(){
     int i,j=0;  
     
     for(i=0;i<conta_b-num_nemici*4;i++){
           if(block[i].tipo==2||block[i].tipo==3){
               bl_arancia[j]=i;
               j++;}
           }  
              
     for(i=conta_b-num_nemici/4;i<conta_b;i++)
         block[i].tipo=-1;
     }
     
     
     
