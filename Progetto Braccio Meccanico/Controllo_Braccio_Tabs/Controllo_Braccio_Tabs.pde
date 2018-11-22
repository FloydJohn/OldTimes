import g4p_controls.*;
import processing.serial.*;

int  CTO = 100;

int mainX,mainY;
String program_name="empty";
ArrayList <Integer> current_program = new ArrayList <Integer>();
int current_line = 1;

void setup() {
  size(695, 600, JAVA2D);
  crea_GUI(this);
}

void draw() {
  background(255);
  trigonometry();
  if(sim_activated) update_sim();
  if(currentlyPlaying == true) playUpdate();
  if(ser_activated) serUpdate();
  punto_corrente.setText(String.valueOf(current_line));
}

void muovi(int mot, int passi) {
  println("muovo il motore "+mot+" di "+passi+" passi.");
  if(ser_activated) serialTX(current_program, current_line);
  if(sim_activated) sim_buf[mot] += passi;
}
