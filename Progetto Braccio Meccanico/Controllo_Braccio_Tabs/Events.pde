void handleButtonEvents(GButton button, GEvent event) {
  if (button == simBut) {
      if (!sim_started) {
        sim_create(this);
        println("SIMULATION STARTED FOR FIRST TIME");
        frame.setSize(1200,625);
      }
      else if(sim_activated && sim_started) {
       sim.setVisible(false);
       sim_activated=false;
       frame.setSize(695,625);
      }
      else if(!sim_activated && sim_started) {
       sim.setVisible(true);
       sim_activated=true;
       frame.setSize(1200,625);
      }
     println("CHANGED SIMULATION VISIBILITY TO : "+sim_activated);
  }
 
  if (button == serBut) {
      if (!ser_started) {
        ser_init();
      }
      else if(ser_activated && ser_started) ser_activated=false;
      else if(!ser_activated && ser_started) ser_activated=true;
      
      println("CHANGED SERIAL STATE TO : "+ser_activated);
  }
       
    
  
  if (button == keyBut) {
      if(!key_activated) key_activated=true;
      else key_activated=false;
      println("CHANGED KEY STATE TO : "+key_activated);
  }
  
   if (button == save_posBut) {
      String prova_passi[] = new String[6];
      int[] zeros = {0,0,0,0,0,0}; 
      for(int i=0;i<6;i++) {
            prova_passi[i] = seleziona_passi[i].getText();
            if (prova_passi[i] == "" && motori_selezionati[i]) println("Per favore, inserisci un numero di passi valido per il motore "+(i+1));
            else {
            int passi=0;
            if (prova_passi[i] == "") passi = 0;
            else passi = Integer.parseInt(seleziona_passi[i].getText());
            if(current_line >= current_program.size() / 6) {
            seleziona_passi[i].setText("0");
            muovi(i,passi);
            current_program.set((current_line-1)*6+i,passi);
            }
            println((current_line)+" > SAVED POSITION! MOT = "+(i+1)+" & PASSI = "+passi);
            seleziona_passi[i].setTextEditEnabled(false);
            motChooseOpt[i].setSelected(false);
            motori_selezionati[i]=false;
            } 
         }
         updateProgramShow();
       current_line++;
       for(int l=0;l<6;l++) current_program.add(0);
    }
  
  if (button == serOkBut) {
      ser = new Serial(this, Serial.list()[serialChosen], 9600);
      ser_started = true;
      ser_activated=true;
      println("SERIAL STARTED FOR FIRST TIME, PORT : "+ Serial.list()[serialChosen]);
      println("CHANGED SERIAL STATE TO : "+ser_activated);
      serChoose.forceClose();
  }
  if (button == serCancelBut) {
      ser_started = false;
      ser_activated=false;
      serChoose.forceClose();
  }
  
    if (button == btnInput) {
    program_name = G4P.selectInput("Input Dialog", "txt,aap", "Arm Program Files");
    String[] program_buffer = new String[0];
    program_buffer = loadStrings(program_name);
    
    for(int i=0;i<program_buffer.length;i++)
        current_program.add(Integer.parseInt(program_buffer[i]));
    
  }
  else if (button == btnOutput) {
    String ext = "txt";
    program_name = G4P.selectOutput("Output Dialog", "txt,aap", "Arm Program Files");
    println("SAVED AS "+program_name);
    String[] buffer = new String[current_program.size()];
    for(int i=0;i<current_program.size();i++) buffer[i] = String.valueOf(current_program.get(i));
    if(program_name.toLowerCase().contains(ext.toLowerCase()))  saveStrings(program_name, buffer);
    else saveStrings(program_name+".txt", buffer);
    
    for(int i=0;i<current_program.size();i++)
    println(i+" > "+current_program.get(i));
    
    
  }
  else if (button == btnPlay && current_program.size() > 6) {
    println("Play program, length = "+current_program.size());
    currentlyPlaying = true;
    azzera_motori();
  }
  
  else if (button == pointForwardBut) {
    current_line++;
    if(current_line > current_program.size()/6) current_line--;
    updatePassiText();
  }
  
  else if (button == pointBackwardBut) {
    current_line--;
    if(current_line < 1) current_line++;
    updatePassiText();
  }
  
  
}

void handleToggleControlEvents(GToggleControl option, GEvent event) {

  for(int i=0;i<6;i++) if(option == motChooseOpt[i]){ 
    motori_selezionati[i] = option.isSelected(); 
    seleziona_passi[i].setTextEditEnabled (option.isSelected());
    }
  if(!ser_started&&ser_activated) {
  int porte = Serial.list().length;
  for(int i=0;i<porte;i++) if(option == serChooseOpt[i])  serialChosen = i; 
  }
}

void keyPressed(){
  if (keyCode==UP)
    alfa1+=0.02;
  if (keyCode==DOWN)
    alfa1-=0.02;
  if (keyCode==LEFT)
    alfa2+=0.02;
  if (keyCode==RIGHT)
    alfa2-=0.02;
  if (key=='+')
    zoom--;
  if (key=='-')
    zoom++;
    
  if (key==ENTER)
    handleButtonEvents(save_posBut,GEvent.PRESSED);
  
  if(zoom<80)zoom=90;  
  if(alfa1<0)alfa1=0;
  if(alfa1>PI)alfa1=PI;
  
  if (key_activated){
   if (key=='z') muovi(1,1);
   if (key=='x') muovi(1,-1);
   if (key=='c') muovi(2,1);
   if (key=='v') muovi(2,-1);
   if (key=='b') muovi(3,1);
   if (key=='n') muovi(3,-1);
   if (key=='a') muovi(4,1);
   if (key=='s') muovi(4,-1);
   if (key=='d') muovi(5,1);
   if (key=='f') muovi(5,-1);
   if (key=='g') muovi(6,1);
   if (key=='h') muovi(6,-1);
  }
}

public void handleTextEvents(GEditableTextControl textcontrol, GEvent event) { /* code */ }
