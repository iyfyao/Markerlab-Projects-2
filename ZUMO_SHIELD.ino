#include <Wire.h>
#include <ZumoShield.h>
#include <ZumoMotors.h>
#include <Pushbutton.h> 
void calibrateTheUltrasonic();
void LineDetection();

// MY NOTES
// INFRARED SENSOR used to detect the white band - maximum 5 mm
//CAPTEUR ULTRRASON = pin 13
//CONTROLE DES MOTEURS = digital pin 7-10  (7 motor 1 direction, 8 motor 2 direction)  (9 motor 1 speed, 10 motor 2 speed)
//COURANT FOUNIT PAR LA BATTERIE = gnd(borne -) et vin(borne +)

#define LED_PIN 13
#define QTR_THRESHOLD  1500
const int TRIG_PIN = 4; //Initialize ultrasonic sensor 
const int ECHO_PIN = 1; //Initialize ultrasonic sensor 

#define REVERSE_SPEED     400 // Initialize motor speed  
#define REVERSE_DURATION  200

#define TURN_SPEED        200
#define TURN_DURATION     300 

#define FORWARD_SPEED     200
#define FORWARD_DURATION  200

#define line_detectors 6
unsigned int sensor_values[line_detectors];

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

long durationTime;
long cm;  


void setup() {
  pinMode(LED_PIN, HIGH); //Set light to on
  Serial.begin(9600); //Monitor
  button.waitForButton();
}

void loop() {
  //
  digitalWrite(LED_PIN, HIGH);
  motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED); 
  calibrateTheUltrasonic();
  LineDetection();
}

// Default configuration-The Pulse is High, Put it down or initialize it 
void calibrateTheUltrasonic(){
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  pinMode(ECHO_PIN, INPUT);
  durationTime = pulseIn(ECHO_PIN, HIGH);
  cm = durationTime/ 29 / 2;
}

void LineDetection(){
 sensors.read(sensor_values);

 if (sensor_values[0] < QTR_THRESHOLD)  //Left most sensor + right
 {
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(2*REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    delay(FORWARD_DURATION);
 }
 else if (sensor_values[0] && sensor_values[5] < QTR_THRESHOLD)  //combination + leftmost sensor + right
 {
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(2*REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    delay(FORWARD_DURATION);
 }
 else if (sensor_values[5] < QTR_THRESHOLD) // right most sensor + left 
 {
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(2*REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    delay(FORWARD_DURATION);
 }
 else
 {
    if(cm < 8)
    {        
    digitalWrite(LED_PIN, LOW);   

    // double speed
    motors.setSpeeds(FORWARD_SPEED*2, FORWARD_SPEED*2);
    delay(FORWARD_DURATION);
    }
    else
    {
      digitalWrite(LED_PIN, LOW); 
      
      // turn to randomize direction
      motors.setSpeeds(FORWARD_SPEED*0.5, FORWARD_SPEED);

      delay(FORWARD_DURATION);
    }
 }

}
