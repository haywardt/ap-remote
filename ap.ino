int state;
#define apout 13  // autopilot out
#define apin  13  // autopilot in
#define bittime 208
#define encoderClk 12
#define encoderData 11
#define encoderVcc 9
#define encoderGnd 8



void setup() {
  // put your setup code here, to run once
  pinMode(apout,OUTPUT);
  pinMode(encoderClk,INPUT);
  pinMode(encoderData,INPUT);
  pinMode(encoderGnd,OUTPUT);
  digitalWrite(encoderGnd,LOW);
  pinMode(encoderVcc,OUTPUT);
  digitalWrite(encoderVcc,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(encoderClk)==0){
    if(digitalRead(encoderData)){
      sendMinusOne();
    }
    else{
      sendPlusOne();
    }
  delay(1);
  while(!digitalRead(encoderClk)){delay(1);}
  
  }
  // sendMinusOne();
  // sendPlusOne();
  

}

int diddle(int period) {  // toggle the ap output for period bit times. send without using hardware.
  if(period==0) {
    // period zero means reset the state and delay 20ms. Output floats high
    state = state | 1;
    delayMicroseconds(20 * bittime);
    }
    else {
      if(( state & 0x01) == 1 ) {
        // state is high so toggle low for period bit times
              // digitalWrite(apout,HIGH);
        pinMode(apout,INPUT_PULLUP); // write high through pullup resistor
        state = state & 0xFE; // clear low order bit
        delayMicroseconds(period * bittime);
      }
      else {
        // state is low so float high for period bit times
        pinMode(apout,OUTPUT); // take pin to ground
        digitalWrite(apout,LOW);  // low turns off external transistor low side switch allowing data to float
        // collision detection will go in here
        state = state | 0x01;
        delayMicroseconds(period * bittime);
        }
      }
    
    }
  
void sendPlusOne(){
  diddle(0);
  send8611();
   // send start,0x07,0b0,stop
  //  L HHHL LLLL L H
  diddle(1);
  diddle(3);
  diddle(6);
  diddle(1);
   
   // send start,0xF8,0b0,stop
  // L LLLH HHHH L H
  diddle(4);
  diddle(5);
  diddle(1);
  diddle(1);
   
  }
void sendMinusOne(){
  diddle(0);    // initialize port
  send8611();   // spoof z101 remote button
  
    // send start,0x05,0b0,stop

  diddle(1);     // L       0 1010 0000 0 1
  diddle(1);     // H
  diddle(1);     // L
  diddle(1);     // H
  diddle(6);     // L L L L L L
  diddle(1);     // H
  
// send start, 0xfa,0b0,stop

  diddle(2);     // L L     0 0101 1111 0 1
  diddle(1);     // H
  diddle(1);     // L
  diddle(5);     // H H H H H
  diddle(1);     // L
  diddle(1);     // H

}

void send8611(){
  // send start,0x86,0b0,stop
  diddle(2);     //  L L       0 0110 0001 1 1
  diddle(2);     //  H H
  diddle(4);     //  L L L L
  diddle(3);     //  H H H 

  // send start,0x11,0b0,stop

  diddle(1);     //  L        0 1000 1000 0 1
  diddle(1);     //  H
  diddle(3);     //  L L L
  diddle(1);     //  H
  diddle(4);     //  L L L L
  diddle(1);     //  H
}
