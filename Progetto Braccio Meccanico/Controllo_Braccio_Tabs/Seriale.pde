GWindow sim,serChoose;
Serial ser;
GToggleGroup serialChoose = new GToggleGroup();
GOption[] serChooseOpt;
GButton serOkBut,serCancelBut;
int serialChosen;
int connectionTimeout = CTO;
int step = 0;

void serialEvent(Serial myPort) {
  int inByte = myPort.read();
  println("REC : "+inByte);
}   

void ser_init() {
  int porte = Serial.list().length;
  if(porte != 0) {
  serChoose = new GWindow(this, "Configurazione porta seriale", width/2, height/2, 160, 20+20*porte, false, JAVA2D);
  serChoose.setActionOnClose(GWindow.KEEP_OPEN);
  serChoose.setResizable (false);
  serChooseOpt = new GOption[porte];
  for(int i=0;i<porte;i++) {
    serChooseOpt[i] = new GOption(serChoose.papplet, 10, i*20, 80, 20, Serial.list()[i]);
    serialChoose.addControl(serChooseOpt[i]);
  }
  serOkBut = new GButton(serChoose.papplet, 10, 20*porte, 50, 20, "OK");
  serCancelBut = new GButton(serChoose.papplet, 80, 20*porte, 80, 20, "CANCEL");
  }
  else println("No suitable serial ports found.");
  ser_started = false;
}

void serialTX(ArrayList <Integer> program, int position){
  int[] txData = new int[6];
  int rec;
  int time = 0;
  int cont = 0;
  for (int i = 0; i < 6 ; i++) txData[i] = program.get(i + (position * 6));
  
  
  if (cont == 0) println("Richiesta fallita.");
  else {
     rec = ser.read();
     if (rec == 'K') {
          println("Risposta ricevuta! Inizio trasmissione");
          for(int i = 0; i < 6 ; i++) ser.write(txData[i]);
          ser.write('?');
          while (ser.available() == 0 && time < 100) time++;
          if (time >= 100) println("Connection timed out.");
          else {
            rec = ser.read();
            if (rec == 19){
               ser.write('@');
               println("Trasmissione avvenuta con successo! Inviato char di start.");
            }
            
          }
       }
     else println("Non ho ricevuto la risposta desiderata, ma rec = "+rec);
    }
}

void serUpdate() {
  
 if(connectionTimeout > 0)  {
   int rec = 0;
   int sentData = 0;
  switch(step) {
    case 0 : //Il PC invia carattere di handshake 'k' al PIC. Se risponde, passa a step successivo
      if (connectionTimeout%10 == 0) ser.write('k'); //Controlla se il dispositivo è pronto a ricevere, ogni 10 cicli
      if (ser.available() > 0) step++;
      connectionTimeout --;
      break;
    case 1 :
      rec = ser.read();
      if (rec == 'K') step++; //Se riceve carattere di handshake 'K' continua
      else serError(step,rec);   //Altrimenti chiama funzione di errore
      break;
    case 2 : //Se sono arrivato qui, il PIC è pronto per ricevere i dati. Chiamo quindi la funzione che manipola ed invia i dati
      sentData = serTXData();
      break;
    case 3 : //Richiesta dei byte ricevuti dal PIC
      if (connectionTimeout%10 == 0) ser.write('?');
      if (ser.available() > 0) step++;
      connectionTimeout --;
      break;
    case 4 : 
      rec = ser.read();
      if (rec == sentData + 2) step++;
      else serError(step,rec);
      break;
    case 5 :
      ser.write('@');
      println("Serial > Communication successful. Start data trasmitted.");
      step++;
      break;
    case 6 :
      if (ser.available() > 0) step++;
      break;
    case 7 :
      rec = ser.read();
      if (rec == 'R') {
        connectionTimeout = CTO;
        step = 0;
        println("Movement completed, ready to restart");
      }
      else serError(step, rec);
      break;
    }
  }  
}


int serTXData() {
  int buffer;
  int chars = 0;
  String buffer2;
  for (int i = 0; i < 6 ; i++) { 
   ser.write('p');
   chars++;
   buffer = current_program.get(i + (current_line*6) );
   buffer2 = Integer.toString(buffer);
   ser.write(buffer2);
   chars += buffer2.length();
   ser.write('v');
   ser.write(0x80);
   chars += 2;
 }
 println("Sent "+chars+" data.");
 return chars;
}

void serError(int s, int r) {
  println("-----------------------------------------------------------ERROR---------------------------------------------------");
  println("Type = Serial");
  println("Error = At trasmission step "+s+" character received is not expected. Found rec = "+r);
  println("---------------------------------------------------------ERROR END-------------------------------------------------");
}
