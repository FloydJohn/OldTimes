GButton simBut,serBut,keyBut, save_posBut;
GButton pointForwardBut, pointBackwardBut;
GButton btnOutput,btnInput, btnPlay;
GTextArea program_show;
GTextField punto_corrente;
GCheckbox[] motChooseOpt = new GCheckbox[6];
GToggleGroup motChoice = new GToggleGroup();
GTextField[] seleziona_passi = new GTextField[6];
boolean[] motori_selezionati = new boolean[6];
boolean sim_started=false,sim_activated=false,ser_started=false,ser_activated=false,key_activated=false;

int motorChosen;



void crea_GUI(PApplet mainWin) {
  simBut=new GButton(mainWin, 10, 10, 30, 30,"SIM");
  simBut.setLocalColorScheme(GCScheme.GREEN_SCHEME);
  serBut=new GButton(mainWin, 50, 10, 30, 30,"SER");
  serBut.setLocalColorScheme(GCScheme.BLUE_SCHEME);
  keyBut=new GButton(mainWin, 90, 10, 30, 30,"KEY");
  keyBut.setLocalColorScheme(GCScheme.RED_SCHEME);
  save_posBut=new GButton(mainWin, 12, 220, 168, 70,"SAVE POSITION");
  save_posBut.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  for(int i=0;i<6;i++) {
     motChooseOpt[i] = new GCheckbox(mainWin, 10, i*20+80, 80, 20, "Motore "+(i+1));
     motChoice.addControl(motChooseOpt[i]);
  }
  for(int i=0;i<6;i++) {
     seleziona_passi[i] = new GTextField(mainWin, 120, i*20+80, 60, 20);
     seleziona_passi[i].tag = "passi"+i;
     seleziona_passi[i].setDefaultText("0");
     seleziona_passi[i].setTextEditEnabled (false);
  }
  
  btnOutput = new GButton(mainWin, 12, 300, 50, 20, "Output");
  btnInput = new GButton(mainWin, 72, 300, 50, 20, "Input");
  btnPlay = new GButton(mainWin, 132, 300, 50, 20, "Play");
  btnPlay.setLocalColorScheme(GCScheme.RED_SCHEME);
  
  program_show = new GTextArea(mainWin, 400, 20, 250, 560,G4P.SCROLLBAR_VERTICAL);
  program_show.tag = "prg_shw";
  program_show.setDefaultText("Program still empty.");
  program_show.setTextEditEnabled (false);
  
  punto_corrente = new GTextField(mainWin, 10, 50, 30, 20);
  punto_corrente.tag = "point";
  punto_corrente.setDefaultText(String.valueOf(0));
  punto_corrente.setTextEditEnabled(false);
  
  pointForwardBut = new GButton(mainWin, 50, 50, 20, 20, "F");
  pointBackwardBut = new GButton(mainWin, 80, 50, 20, 20, "B");
  
  
  for(int l=0;l<6;l++) current_program.add(0);
}  


void updatePassiText() {
  for(int i=0;i<6;i++) seleziona_passi[i].setText(String.valueOf(current_program.get(i+6*(current_line-1))));
}

void updateProgramShow() {
  String[] buffer = new String[current_program.size()];
  for(int i=0;i<(current_program.size());i++) buffer[i] = String.valueOf(current_program.get(i));
  program_show.setText(buffer);
} 


