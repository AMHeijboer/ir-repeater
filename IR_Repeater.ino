/*
  IR Repeater (IR_Repeater.ino)

  Sketch to repeat IR signals (make a IR repeater with a Arduino + IR reciever + IR sender)

  created 18 December 2019 in Maassluis, The Netherlands
  by A.M. Heijboer
  modified 18 December 2019
    
  This code is property of A.M. Heijboer. © A.M. Heijboer.

  Github: https://github.com/AMHeijboer/ir-repeater (project is forked!!)

  Note: the creation date is the date this project was forked on GitHub
*/


// MOVED TO PRIVATE GIT REPOSITORY!! This sketch was created as a quick test and has since been moved to a private Git Repository.



#include <IRremote.h>

// create variables for IR send/recieve PINs
// TestSetup01 - IR Receiver start PIN is 11 (~13) | IR Sender start PIN is 5 (~3)
  // send
  //int irSendData = 3; // DO NOT CHANGE! See 'IRsend irsend' other PIN than default 3 possibly not allowed
  int irSendVCC = 4;
  int irSendGround = 5;
  // recieve
  int irRecvData = 11;
  int irRecvVCC = 13;
  int irRecvGround = 12;

IRrecv irrecv(irRecvData);
//IRsend irsend(irSendData); // Defaults to pin 3 // Dous not work on other PIN? Other PIN not supported by library as it seems
IRsend irsend; // Defaults to pin 3

decode_results results;
unsigned int rawCodes[RAWBUF];

void setup(){
  // set VCC and ground PINs for IR sender and IR transceiver
    // declare PINs as output
    pinMode(irSendVCC, OUTPUT);     // send VCC
    pinMode(irSendGround, OUTPUT);  // send Ground
    pinMode(irRecvVCC, OUTPUT);     // recv VCC
    pinMode(irRecvGround, OUTPUT);  // recv Ground
    // give power to PINs
    digitalWrite(irSendVCC, HIGH);  // connect to VCC 5 volt
    digitalWrite(irRecvVCC, HIGH);  // connect to VCC 5 volt
    // give ground to PINs
    digitalWrite(irSendGround, LOW); // connect to Ground
    digitalWrite(irRecvGround, LOW); // connect to Ground

  // below the original setup (above I added myself)
  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.println("Info    : Infrared Decoder and Extender");
  Serial.println("Version : 1.0");
  Serial.println("---------------------------------------");
}

void loop(){
  // IR Extender
  if (irrecv.decode(&results)) {
    //Serial.println(results.value, HEX);
    int codeLen = results.rawlen - 1;
    for (int i = 1; i <= codeLen; i++) {
      if (i % 2) {
        // Mark
        rawCodes[i - 1] = results.rawbuf[i]*USECPERTICK - MARK_EXCESS;
        //Serial.print(" m");
      } 
      else {
        // Space
        rawCodes[i - 1] = results.rawbuf[i]*USECPERTICK + MARK_EXCESS;
        //Serial.print(" s");
      }
      //Serial.print(rawCodes[i - 1], DEC);
    }
    irsend.sendRaw(rawCodes, codeLen, 38);
    irrecv.enableIRIn();
    irrecv.resume(); // Receive the next value
    dump(&results);
  }
}

// Dump results
void dump(decode_results *results) {
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
  } 
  else if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");
  } 
  else if (results->decode_type == SONY) {
    Serial.print("Decoded SONY: ");
  } 
  else if (results->decode_type == RC5) {
    Serial.print("Decoded RC5: ");
  } 
  else if (results->decode_type == RC6) {
    Serial.print("Decoded RC6: ");
  }
  else if (results->decode_type == PANASONIC) {	
    Serial.print("Decoded PANASONIC - Address: ");
    //Serial.print(results->panasonicAddress,HEX);
    Serial.print(" Value: ");
  }
  else if (results->decode_type == JVC) {
    Serial.print("Decoded JVC: ");
  }
  Serial.print(results->value, HEX);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");

  for (int i = 0; i < count; i++) {
    if ((i % 2) == 1) {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    } 
    else {
      Serial.print(-(int)results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println("");
}
