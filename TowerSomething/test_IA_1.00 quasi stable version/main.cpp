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

void draw();                          //Disegna gli oggetti sul buffer
    /**/void sparo();

float P2P(float,float,float,float);   //Distanza point to point (valore assoluto)

float head_toX(float,float,float);    //In caso di spostamento in diagonale ad una data velocità, restituisce il valore dei cateti
float head_toY(float,float,float);    //(x e y) che servono per lo spostamento...  head_toX(distanzaX,distanzaY,velocità(inseguitore));


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Xgreen = 500, Ygreen = 300;         //Coordinate pallino verde
float Xred   = 200, Yred   = 250;         //Coordinate pallino rosso
float Xpro        , Ypro        ;

float Gvel   = 1.7, Rvel   = 3.5;         //Velocità dei pallini
float Pvel   = 7  ;

bool  bum  = FALSE;                       //Variabile che indica se deve avvenire lo sparo

BITMAP *buffer;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(){
	init();

    prog_init();

	while (!key[KEY_ESC])
	{

    rectfill  (buffer, 0, 0, screenX, screenY, makecol(0,0,0));

    move();

    draw();

    blit(buffer,screen,0,0,0,0,screenX,screenY);

    rest(10);
	}

	deinit();
	return 0;
}
END_OF_MAIN()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void prog_init(){

buffer=create_bitmap(800,600);
}
void move(){

red_move();

green_move();

sparo();

}
void red_move(){

if      (key[KEY_UP]) Yred-=Rvel;
else if (key[KEY_DOWN]) Yred+=Rvel;
else if (key[KEY_RIGHT]) Xred+=Rvel;
else if (key[KEY_LEFT]) Xred-=Rvel;

}
void green_move(){


if(P2P(Xgreen,Ygreen,Xred,Yred)<200){


    if(!(P2P(Xgreen,Ygreen,Xred,Yred)<80)){

    Xgreen+=head_toX(Xgreen-Xred,Ygreen-Yred,-fabs(Gvel));
    Ygreen+=head_toY(Xgreen-Xred,Ygreen-Yred,-fabs(Gvel));}
    }
}
void draw(){


//RED OBJ
circlefill(buffer, Xred, Yred, 10, makecol(255,0,0));
//circle    (buffer, Xred, Yred, 80, makecol(255,0,0));

//GREEN OBJ
circlefill(buffer, Xgreen, Ygreen, 10 , makecol(0,255,0));
circle    (buffer, Xgreen, Ygreen, 200, makecol(0,255,0));
circle    (buffer, Xgreen, Ygreen, 80 , makecol(0,255,0));

//}
}
void sparo(){

   static float Move_p_X=0, Move_p_Y=0;        //valore da aggiungere alle coordinate del proiettile per farlo muovere
   static float dirX, dirY;                    //memorizza la direzione iniziale del proiettile

   if(P2P(Xgreen,Ygreen,Xred,Yred)<80){

   Xpro=Xgreen;
   Ypro=Ygreen;

   Xpro+=Move_p_X;
   Ypro+=Move_p_Y;

   Move_p_X+=dirX;      //il valore si incrementa sempre più perchè le coordinate del proiettile, finita la funzione tornano ad essere la posizione di partenza
   Move_p_Y+=dirY;

   if(Xpro==Xgreen&&Ypro==Ygreen){             //solo se il proiettile è in partenza calcola la direzione
   dirX=head_toX(Xgreen-Xred,Ygreen-Yred,-fabs(Gvel))*0.5;
   dirY=head_toY(Xgreen-Xred,Ygreen-Yred,-fabs(Gvel))*0.5;}

   line( buffer, Xpro, Ypro, Xpro-6*dirX, Ypro-6*dirY, makecol(0,255,0));

   if( P2P(Xpro,Ypro,Xgreen,Ygreen)>80 ||        //il proiettile verrà cancellato se tocca il pallino rosso o se esce dagli 80p
       P2P(Xpro,Ypro,Xred  ,Yred  )<10){

      Xpro=Xgreen;
      Move_p_X=0;
      Ypro=Ygreen;
      Move_p_Y=0;

      }
   }

   else{

      Xpro=Xgreen;
      Move_p_X=0;
      Ypro=Ygreen;
      Move_p_Y=0;

   }
}
float P2P(float x1,float y1,float x2,float y2){

return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

}
float head_toX(float x,float y,float v){
int z;

z=sqrt(x*x+y*y);
if(z==0)z=1;

return (v*x/z);

}
float head_toY(float x,float y,float v){
int z1;

z1=sqrt(x*x+y*y);
if(z1==0)z1=1;

return (v*y/z1);

}
inline void init(){
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, screenX, screenY, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}
    install_keyboard();
	install_timer();
	install_mouse();
}
inline void deinit(){
	clear_keybuf();
	destroy_bitmap(buffer);
	allegro_exit();

}
