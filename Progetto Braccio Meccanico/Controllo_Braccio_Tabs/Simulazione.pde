float passo = 0.01;
int sim_buf[] = {0,0,0,0,0,0,0};
float motori[] = {0.0000,-PI/4,0.0000,0.0000,0.0000,0.0000};

float zoom=300;
float Ydist=0;             //distanza dall'asse y
float alfa1=0;
float alfa2=HALF_PI;
float x=0;
float y=0;
float z=0;

int playStep = 0;

boolean currentlyPlaying = false;

void azzera_motori() {
  for(int i=0;i<6;i++)
   motori[i] = 0.0000;
   motori[1] = -PI/4;
   
} 

void playUpdate() {
   println("arr");
   for(int i=0;i<6;i++) {
   muovi(i,current_program.get(i+(playStep*6)));
   println( "Sto simulando il movimento del motore "+i+" con passo "+current_program.get(i+(playStep*6)) );
   }
   playStep++;
   if(playStep*6 == current_program.size()) {
         currentlyPlaying = false;
         playStep = 0;
     } 
} 


void sim_create(PApplet mainWin) {
    sim = new GWindow(mainWin, "Finestra di simulazione", 0, 0, 500, 597, true, P3D);
    sim.addDrawHandler(mainWin, "simDraw");
    sim.setActionOnClose(GWindow.KEEP_OPEN);
    sim.setResizable (false);
    sim_started=true;
    sim_activated=true;
    sim.setLocation(800,100);
} 


void simDraw(GWinApplet a, GWinData data) {
  mainX = frame.getX();
  mainY = frame.getY();
  sim.setLocation(mainX+697,mainY+25);
  a.camera(x, -y, z, 0, 0, 0, 0.0, 1.0, 0.0);

  a.background(0);
  a.fill(204, 102, 0);
  a.rotateX(HALF_PI);
  a.box(40, 100, 40);    //Base della ruspa
  
  a.translate(0,-20,30);
  a.box(25,50,20);       //Base per i motori 
  
  a.translate(0,0,10);
  a.sphere(10);          //Rotazione completa e flessione
  a.rotateZ(motori[0]);

  a.rotateX((PI/2)+motori[1]);
  a.translate(0,30,0);
  a.box(20,60,5);        //Tratto "Spalla-Gomito"
  
  a.translate(0,30,0);
  a.sphere(10);          //Flessione secondo tratto rispetto a primo tratto
  a.rotateX((-PI/4)+motori[2]);

  a.translate(0,30,0);
  a.box(20,60,5);        //Tratto "Gomito-Polso"
  
  a.translate(0,30,0);
  a.rotateX(motori[3]);
  a.rotateY(motori[4]);
  a.sphere(7);           //Rotazione "Polso", controllo presa, flesssione polso rispetto a secondo tratto
 
  
  a.rotateX(PI/6);
  a.rotateZ(-PI/9);
  a.rotateY(PI/3);
  a.translate(2,10,-1);
  a.rotateX(motori[5]);   
  a.box(3,20,2);         //Primo "Dito"
  a.rotateX(-motori[5]);   
  a.translate(-2,-10,1);
  a.rotateY(-PI/3);
  a.rotateZ(PI/9);
  a.rotateX(-PI/6);

  a.rotateX(PI/6);
  a.rotateZ(PI/9);
  a.rotateY(-PI/3);
  a.translate(-2,10,-1);
  a.rotateX(motori[5]); 
  a.box(3,20,2);         //Secondo "Dito"
  a.rotateX(-motori[5]); 
  a.translate(2,-10,1);
  a.rotateY(PI/3);
  a.rotateZ(-PI/9);
  a.rotateX(-PI/6);


  a.rotateX(-PI/11);
  a.translate(0,10,-0.3);  
  a.rotateX(-motori[5]); 
  a.box(3,20,2);        //Terzo "Dito"
  a.rotateX(motori[5]); 
  a.translate(0,-10,0);
  a.rotateX(PI/10);
  
}


void trigonometry(){
  //ANGOLAZIONE VERTICALE
  z=zoom*cos(alfa1);
  y=zoom*sin(alfa1);
  Ydist=z;
  //ANGOLAZIONE ORIZZONTALE 
  x=cos(alfa2)*Ydist;
  z=sin(alfa2)*Ydist;
}

void update_sim() {
  for(int i=0;i<6;i++){
    if(sim_buf[i] > 0){
       motori[i] = motori[i]+passo;
       sim_buf[i]--;
     } 
    else if(sim_buf[i] < 0){
       motori[i] = motori[i]-passo;
       sim_buf[i]++;
     } 
  } 
} 
