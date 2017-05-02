/*
  Arduino Yún Bridge example

  This example for the Arduino Yún shows how to use the
  Bridge library to access the digital and analog pins
  on the board through REST calls. It demonstrates how
  you can create your own API when using REST style
  calls through the browser.

  Possible commands created in this shetch:

  "/arduino/digital/13"     -> digitalRead(13)
  "/arduino/digital/13/1"   -> digitalWrite(13, HIGH)
  "/arduino/analog/2/123"   -> analogWrite(2, 123)
  "/arduino/analog/2"       -> analogRead(2)
  "/arduino/mode/13/input"  -> pinMode(13, INPUT)
  "/arduino/mode/13/output" -> pinMode(13, OUTPUT)

  This example code is part of the public domain

  http://www.arduino.cc/en/Tutorial/Bridge

*/

#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>

// Listen to the default port 5555, the Yún webserver
// will forward there all the HTTP requests you send
BridgeServer server;

int treshold = 300; // treshold hold our treshold value above which we turn on the light.

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

int ledPin = A0;
int speakerPin = 4; //Speaker or Piezo buzzer on pin 11

void setup() {
  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  pinMode(ledPin, OUTPUT); // Setting both pins to outputs
  pinMode(speakerPin, OUTPUT); 

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  
  song();
  
  // Get clients coming from server
  BridgeClient client = server.accept();

  // There is a new client?
  if (client) {
    // Process request
    process(client);

    // Close connection and free resources.
    client.stop();


   
  }

   checkNoiseLevel();

  delay(50); // Poll every 50ms
}

void process(BridgeClient client) {
  // read the command
  String command = client.readStringUntil('/');

  // is "digital" command?
  if (command == "digital") {
    digitalCommand(client);
  }

  // is "analog" command?
  if (command == "analog") {
    analogCommand(client);
  }

  // is "mode" command?
  if (command == "mode") {
    modeCommand(client);
  }

  if (command == "treshold"){
    tresholdCommand(client);
  }
}

void tresholdCommand (BridgeClient client) {
  treshold = client.parseInt();

  // Send feedback to client
  client.print(F("treshold set to"));

  client.println(treshold);

  
}


void digitalCommand(BridgeClient client) {
  int pin, value;

  // Read pin number
  pin = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/digital/13/1"
  if (client.read() == '/') {
    value = client.parseInt();
    digitalWrite(pin, value);
  } else {
    value = digitalRead(pin);
  }

  // Send feedback to client
  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" set to "));
  client.println(value);

  // Update datastore key with the current pin value
  String key = "D";
  key += pin;
  Bridge.put(key, String(value));
}

void analogCommand(BridgeClient client) {
  int pin, value;

  // Read pin number
  pin = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/analog/5/120"
  if (client.read() == '/') {
    // Read value and execute command
    value = client.parseInt();
    analogWrite(pin, value);

    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" set to analog "));
    client.println(value);

    // Update datastore key with the current pin value
    String key = "D";
    key += pin;
    Bridge.put(key, String(value));
  } else {
    // Read analog pin
    value = analogRead(pin);

    // Send feedback to client
    client.print(F("Pin A"));
    client.print(pin);
    client.print(F(" reads analog "));
    client.println(value);

    // Update datastore key with the current pin value
    String key = "A";
    key += pin;
    Bridge.put(key, String(value));
  }
}

void modeCommand(BridgeClient client) {
  int pin;

  // Read pin number
  pin = client.parseInt();

  // If the next character is not a '/' we have a malformed URL
  if (client.read() != '/') {
    client.println(F("error"));
    return;
  }

  String mode = client.readStringUntil('\r');

  if (mode == "input") {
    pinMode(pin, INPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as INPUT!"));
    return;
  }

  if (mode == "output") {
    pinMode(pin, OUTPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as OUTPUT!"));
    return;
  }

  client.print(F("error: invalid mode "));
  client.print(mode);
}

void checkNoiseLevel ()
{
  int sensorValue = analogRead(A3);
  if (sensorValue > treshold){
      digitalWrite(13, HIGH);
  }
  else{
    digitalWrite (13, LOW);
  } 
}
  
void beep (unsigned char speakerPin, int frequencyInHertz, long timeInMilliseconds)  //code for working out the rate at which each note plays and the frequency.
{
  digitalWrite(ledPin, HIGH);
  int x;     
  long delayAmount = (long)(1000000/frequencyInHertz);
  long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2));
  for (x=0;x<loopTime;x++)   
  {   
    digitalWrite(speakerPin,HIGH);
    delayMicroseconds(delayAmount);
    digitalWrite(speakerPin,LOW);
    delayMicroseconds(delayAmount);
  }   
  digitalWrite(ledPin, LOW);
  delay(20);
}       

void song()  //here is where all the notes for the song are played.
{       
  beep(speakerPin, NOTE_D5, 100);  //   beep( -PIN OF SPEAKER-, -THE NOTE WANTING TO BE PLAYED-, -DURATION OF THE NOTE IN MILISECONDS- )
  delay(80);
  beep(speakerPin, NOTE_F5, 100);
  delay(80);
  beep(speakerPin, NOTE_D6, 200);
  delay(250);
 
  beep(speakerPin, NOTE_D5, 100);
  delay(80);
  beep(speakerPin, NOTE_F5, 100);
  delay(80);
  beep(speakerPin, NOTE_D6, 200);
  delay(250);
 
  beep(speakerPin, NOTE_E6, 200);
  delay(200);
  beep(speakerPin, NOTE_F6, 100);
  delay(100);
  beep(speakerPin, NOTE_E6, 100);
  delay(80);
  beep(speakerPin, NOTE_F6, 100);
  delay(80);
  beep(speakerPin, NOTE_E6, 100);
  delay(80);
  beep(speakerPin, NOTE_C6, 100);
  delay(80);
  beep(speakerPin, NOTE_A5, 100);
  delay(300);
 
  beep(speakerPin, NOTE_A5, 200);
  delay(100);
  beep(speakerPin, NOTE_D5, 200);
  delay(100);
  beep(speakerPin, NOTE_F5, 100);
  delay(100);
  beep(speakerPin, NOTE_G5, 100);
  delay(100);
  beep(speakerPin, NOTE_A5, 100);
  delay(500);
 
  beep(speakerPin, NOTE_A5, 200);
  delay(100);
  beep(speakerPin, NOTE_D5, 200);
  delay(100);
  beep(speakerPin, NOTE_F5, 100);
  delay(100);
  beep(speakerPin, NOTE_G5, 100);
  delay(100);
  beep(speakerPin, NOTE_E5, 100);
  delay(500);
 
 
 
  beep(speakerPin, NOTE_D5, 100);
  delay(80);
  beep(speakerPin, NOTE_F5, 100);
  delay(80);
  beep(speakerPin, NOTE_D6, 200);
  delay(250);
 
  beep(speakerPin, NOTE_D5, 100);
  delay(80);
  beep(speakerPin, NOTE_F5, 100);
  delay(80);
  beep(speakerPin, NOTE_D6, 200);
  delay(250);
 
  beep(speakerPin, NOTE_E6, 200);
  delay(200);
  beep(speakerPin, NOTE_F6, 100);
  delay(100);
  beep(speakerPin, NOTE_E6, 100);
  delay(80);
  beep(speakerPin, NOTE_F6, 100);
  delay(80);
  beep(speakerPin, NOTE_E6, 100);
  delay(80);
  beep(speakerPin, NOTE_C6, 100);
  delay(80);
  beep(speakerPin, NOTE_A5, 100);
  delay(300);
 
  beep(speakerPin, NOTE_A5, 200);
  delay(100);
  beep(speakerPin, NOTE_D5, 200);
  delay(100);
  beep(speakerPin, NOTE_F5, 100);
  delay(100);
  beep(speakerPin, NOTE_G5, 100);
  delay(100);
  beep(speakerPin, NOTE_A5, 300);
  delay(100);
  beep(speakerPin, NOTE_A5, 200);
  delay(100);
  beep(speakerPin, NOTE_D5, 300);
  delay(2000);
 
}
