#include <allegro.h>
#include <math.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define screenX 800
#define screenY 600

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void init();
inline void deinit();

void prog_init();                     //Racchiude le operazioni da fare prima del ciclo principale

void move();                          //Gestisce i movimenti di ogni oggetto
    /**/void red_move();
    /**/void green_move();
    /**/void sparo();

void draw();                          //Disegna gli oggetti sul buffer
    /**/void d_green(int);
    /**/void d_red  (int);
    /**/void d_tower(int);

float P2P(float,float,float,float);   //Distanza point to point (valore assoluto)

float head_toX(float,float,float);    //In caso di spostamento in diagonale ad una data velocità, restituisce il valore dei cateti
float head_toY(float,float,float);    //(x e y) che servono per lo spostamento...  head_toX(distanzaX,distanzaY,velocità(inseguitore));

void agg_dati();

void increment_speed_counter();       //Funzioni per il clock del gioco
volatile long speed_counter = 0 ;

void  type_data();                    //Gestisce i dati dei tipi di obj

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ogg {
    public:
    //MOVE
    float  x;
    float  y;
    float  vel;         //velocità oggetto
    bool   team;        //TRUE=alleato, FALSE=nemico
    //RAD
    float  Mrad;        //follower-radius
    float  Srad;        //shooting-radius
    //PRO
    float  Xpro[30];    //coo. ipotetici proiettili
    float  Ypro[30];
    float  Xgen;        //punto di partenza proiettili
    float  Ygen;
    float  dirX[30];    //dir. iniziale proiettili
    float  dirY[30];
    float  Pvel    ;    //velocità proiettile
    bool  esist[30];    //esistenza proiettile

    int    type;        // type: 0-----> soldatini
                        //       1-----> omino rosso
                        //       2-----> tower
    };

ogg obj[10];





float Rvel   = 3.5;                       //Velocità del pallino

int   Xpos   = 0  , Ypos   = 0  ;         //Posizione punto in alto sx della mappa


BITMAP *buffer ;
BITMAP *mappaB ;
BITMAP *mappaV ;
BITMAP *tower ;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){
	init();

    prog_init();

	while (!key[KEY_ESC])
	{

      while(speed_counter > 0){        //Tutte le operazioni di logica vanno inserite nel ck del gioco tranne i disegni

         move();

         if( obj[4].x>=screenX/2&&obj[4].x<=screenX*3-screenX/2)Xpos=obj[4].x-screenX/2;     // Gestisce la camera puntata sull'omino rosso e evita di inquadrare fuori dalla mappa
         if( obj[4].y>=screenY/2&&obj[4].y<=screenY*3-screenY/2)Ypos=obj[4].y-screenY/2;

         speed_counter --;

      }

    draw();

    blit(buffer,screen,Xpos,Ypos,0,0,screenX,screenY);

    clear_bitmap(buffer);


	}

	deinit();
	return 0;
}
END_OF_MAIN()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void prog_init(){

int i;

buffer=create_bitmap(screenX*3,screenY*3);

tower  = load_bitmap("tower.bmp", NULL);

mappaB = load_bitmap("mappaB.bmp", NULL);

mappaV = load_bitmap("mappaV.bmp", NULL);

for(i=0;i<10;i++) obj[i].esist[0]=FALSE;

obj[0].type=0;
obj[1].type=0;
obj[2].type=0;
obj[3].type=0;
obj[4].type=1;
obj[5].type=2;
obj[6].type=2;
obj[7].type=2;
obj[8].type=99;
obj[9].type=99;

obj[0].x=500;
obj[1].x=1500;
obj[2].x=800;
obj[3].x=1100;
obj[5].x=1300;
obj[6].x=1500;
obj[7].x=1800;

obj[0].y=100;
obj[1].y=500;
obj[2].y=1200;
obj[3].y=800;
obj[5].y=1200;
obj[6].y=300;
obj[7].y=450;

obj[4].x   = 200;
obj[4].y   = 250;         //Coordinate pallino rosso


type_data();

}

END_OF_FUNCTION(prog_init);

void  type_data(){

int i;

for(i=0;i<10;i++){
   if(obj[i].type==0){          //racchiude tutte le info comuni al tipo 0

   obj[i].Xgen=obj[i].x;
   obj[i].Ygen=obj[i].y;
   obj[i].vel =1.7;
   obj[i].Mrad=250;
   obj[i].Srad=100;
   obj[i].esist[0]=FALSE;
   obj[i].Pvel= 1.2;
   obj[i].team = FALSE;
   }

   if(obj[i].type==1){


   obj[i].Xgen=obj[i].x;
   obj[i].Ygen=obj[i].y;
   obj[i].vel =3.5;
   obj[i].Mrad=0;
   obj[i].Srad=-1;
   obj[i].esist[0]=FALSE;
   obj[i].Pvel= 2;
   obj[i].team = TRUE;
   }

   if(obj[i].type==2){


   obj[i].Xgen=obj[i].x+25;
   obj[i].Ygen=obj[i].y+35;
   obj[i].vel =0;
   obj[i].Mrad=0;
   obj[i].Srad=270;
   obj[i].esist[0]=FALSE;
   obj[i].Pvel= 5.7;
   obj[i].team = FALSE;
   }

}

}

END_OF_FUNCTION(type_data);

void move(){


red_move();

green_move();

agg_dati();

sparo();



}

END_OF_FUNCTION(move);

void red_move(){

if      (key[KEY_UP])    obj[4].y-=Rvel;
else if (key[KEY_DOWN])  obj[4].y+=Rvel;
else if (key[KEY_RIGHT]) obj[4].x+=Rvel;
else if (key[KEY_LEFT])  obj[4].x-=Rvel;

}

END_OF_FUNCTION(red_move);

void green_move(){

int i;

for(i=0;i<10;i++){

if(obj[i].type==0)

if(P2P(obj[i].x,obj[i].y,obj[4].x,obj[4].y)<obj[i].Mrad){


    if(!(P2P(obj[i].x,obj[i].y,obj[4].x,obj[4].y)<obj[i].Srad)){

    obj[i].x+=head_toX(obj[i].x-obj[4].x,obj[i].y-obj[4].y,-fabs(obj[i].vel));
    obj[i].y+=head_toY(obj[i].x-obj[4].x,obj[i].y-obj[4].y,-fabs(obj[i].vel));}
    }
}
}

END_OF_FUNCTION(green_move);

void draw(){

int i,k;

for(i=0;i<3;i++)                                      //stampa lo sfondo
   for(k=0;k<3;k++){
      if((k+i)%2==0)
      draw_sprite(buffer, mappaB, 800*k , 600*i);
      else
      draw_sprite(buffer, mappaV, 800*k , 600*i);
   }

int h;

for(h=0;h<10;h++){            //disegna i proiettili
if(obj[h].esist[0])
line( buffer, obj[h].Xpro[0], obj[h].Ypro[0], obj[h].Xpro[0]-obj[h].dirX[0], obj[h].Ypro[0]-obj[h].dirY[0], makecol(0,255,0));
}

int j;                          //disegna gli oggetti

for(j=0;j<10;j++){

if(obj[j].type==0)
d_green(j);

if(obj[j].type==1)
d_red(j);

if(obj[j].type==2)
d_tower(j);

}

}

END_OF_FUNCTION(draw);

void d_green(int j){

    circlefill(buffer, obj[j].x, obj[j].y, 10 , makecol(0,255,0));

    if(key[KEY_R]){                             // Premendo R si potrà vedere il raggio d'azione
       circle    (buffer, obj[j].x, obj[j].y, obj[j].Mrad, makecol(0,255,0));
       circle    (buffer, obj[j].x, obj[j].y, obj[j].Srad, makecol(0,255,0));
       }
}
END_OF_FUNCTION(d_green());

void d_red(int j){

    circlefill(buffer, obj[j].x, obj[j].y, 10, makecol(255,0,0));

    //circle  (buffer, Xred, Yred, obj[i].Srad, makecol(255,0,0));
}
END_OF_FUNCTION(d_red());

void d_tower(int j){

    masked_blit(tower, buffer, 0 , 0 , obj[j].x, obj[j].y, 50,80 );

    if(key[KEY_R]){                             // Premendo R si potrà vedere il raggio d'azione
       circle    (buffer, obj[j].Xgen, obj[j].Ygen, obj[j].Mrad, makecol(0,255,0));
       circle    (buffer, obj[j].Xgen, obj[j].Ygen, obj[j].Srad, makecol(0,255,0));
       }

}
END_OF_FUNCTION(d_tower());

void sparo(){
   static int cont[10]={0,0,0,0,0,0,0,0,0,0};
   int i;

   for(i=0;i<10;i++){

       if(!(obj[i].esist[0])  &&                              // Se è la prima volta E la distanza è valida

          (P2P(obj[i].Xgen,obj[i].Ygen,obj[4].x,obj[4].y)<obj[i].Srad) ){

          obj[i].esist[0]=TRUE;

          obj[i].Xpro[0]=obj[i].Xgen;                  // le coo. del pro. sono quelle del nemico
          obj[i].Ypro[0]=obj[i].Ygen;

          obj[i].dirX[0]=head_toX(obj[i].Xgen-obj[4].x,obj[i].Ygen-obj[4].y,-fabs(obj[i].Pvel));   //calcola la direzione
          obj[i].dirY[0]=head_toY(obj[i].Xgen-obj[4].x,obj[i].Ygen-obj[4].y,-fabs(obj[i].Pvel));

          cont[i]++;
       }

       if(cont[i]>0 && (cont[i] <= obj[i].Srad/P2P(obj[i].dirX[0],obj[i].dirY[0],0,0))){   // muove il pro fino a che non oltrepassa il raggio

          obj[i].Xpro[0]+= obj[i].dirX[0];      // alle coo. del pro. viena aggiunto uno spostamento pari a dir
          obj[i].Ypro[0]+= obj[i].dirY[0];
          cont[i]++;

       }

       if(cont[i] >= obj[i].Srad/P2P(obj[i].dirX[0],obj[i].dirY[0],0,0)) {  //se le volte che è stato incrementato per l'incremento superano la dist massima


          cont[i]=0;
          obj[i].esist[0]=FALSE;

          obj[i].Xpro[0]=obj[i].Xgen;
          obj[i].Ypro[0]=obj[i].Ygen;
          obj[i].dirX[0]=0;
          obj[i].dirY[0]=0;

       }

       if(P2P(obj[i].Xpro[0],obj[i].Ypro[0],obj[4].x  ,obj[4].y  )<10){        //il proiettile verrà cancellato se tocca il pallino rosso

          cont[i]=0;
          obj[i].esist[0]=FALSE;

          obj[i].Xpro[0]=obj[i].Xgen;
          obj[i].Ypro[0]=obj[i].Ygen;
          obj[i].dirX[0]=0;
          obj[i].dirY[0]=0;

       }
}


}

END_OF_FUNCTION(sparo);

float P2P(float x1,float y1,float x2,float y2){

return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

}

END_OF_FUNCTION(P2P);

float head_toX(float x,float y,float v){
int z;

z=sqrt(x*x+y*y);
if(z==0)z=1;

return (v*x/z);

}

END_OF_FUNCTION(head_toX);

float head_toY(float x,float y,float v){
int z1;

z1=sqrt(x*x+y*y);
if(z1==0)z1=1;

return (v*y/z1);

}

END_OF_FUNCTION(head_toY);

void agg_dati(){
  int i;

  for(i=0;i<10;i++)
  if(obj[i].type==1||obj[i].type==0){
  obj[i].Xgen=obj[i].x;
  obj[i].Ygen=obj[i].y;}


}

inline void init(){
	int depth, res;
	allegro_init();

	install_keyboard();

	install_mouse();

	install_timer();
	LOCK_VARIABLE(speed_counter);            //Used to set the timer - which regulates the game's
    LOCK_FUNCTION(increment_speed_counter);  //speed.
    install_int_ex(increment_speed_counter, BPS_TO_TIMER(60));//Set our BPS

	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, screenX, screenY, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}

}

END_OF_FUNCTION(init);

inline void deinit(){
	clear_keybuf();
	destroy_bitmap(buffer);
	destroy_bitmap(mappaB);
	destroy_bitmap(mappaV);
	destroy_bitmap(tower);
	allegro_exit();

}

END_OF_FUNCTION(deinit);

void increment_speed_counter(){       // A function to increment the speed counter
  speed_counter++;                    // This will just increment the speed counter by one. :)
}

END_OF_FUNCTION(increment_speed_counter);
