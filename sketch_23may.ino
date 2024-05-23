//* Obstacle Avoidance *//

//This code uses the HC-SR04 Ultasonic Sensor to measure distance.//
//Here it is paired with a SG90 Servo Motor to help it detect     //
//obstacles in a field of 180°. This in turn helps the Elegoo Car //
//to follow an object and eventualy another Elegoo Car.           //
//Code available on: //
//  //


//Include the Servo Motor Library
#include <Servo.h>  //This library contains the codes necessary to use the SG90 Servo Moror

//Create the Servo Object to control the Servo Motor
Servo myservo;

// Standby pin
int STBY = 3;

// Pins on the Arduino that belong to the motors' of the wheels // 

//Motor A  (Motors on the right side of the car)
int PWMA = 5;    // Pin 5 controls how fast Motor A is spinning
int AIN1 = 8;    // Pin 8 controls in which direction (forward or backward) Motor A is spinning

//Motor B  (Motors on the left side of the car)
int PWMB = 6;    // Pin 6 controls how fast Motor B is spinning
int BIN1 = 7;    // Pin 7 controls in which direction (forward or backward) Motor B is spinning
 
// Pins for the Ultrasonic Sensor
int trigPin = 13;           //Pin 13, the Trigger pin, is the one driven to send out the ultrasonic pulses
int echoPin = 12;           //Pin 12, the Echo in, is the one that produces a pulse when the reflected signal is received.

// Precise Variables
//float duration, distance;   //With float, the duration and distace variables become precise to the decimal

// Available Speeds //    These will be the the Y variables when a command is called
int MaxSPEED= 250;   
int MinSPEED= 50;    
int NormSPEED= 100;    
int DontMOVE= 0;  

// Available Times //     These will be the the X variables when a command is called)
int Stop = 0; 
int Time = 1000;  
int IRResponseTime = 0; 



void Forward(int x,int y){  //Forward command  
  //motor A    
  analogWrite (PWMA,y);     //Motor A is powered at speed y
  digitalWrite (AIN1,1);    //The comand of 1(HIGH) makes motor A move forward

  //Motor B      
  analogWrite (PWMB, y);    //Motor A is powered at speed y
  digitalWrite (BIN1,1);    //The comand of 1(HIGH) makes motor B move forward
} 

void Backward(int x,int y){ //Backward command  
  //motor A    
  analogWrite (PWMA,y);     //Motor A is powered at speed y
  digitalWrite (AIN1,0);    //The comand of 0(LOW) makes motor A move backward

  //Motor B
  analogWrite (PWMB, y);    //Motor B is powered at speed y
  digitalWrite (BIN1,0);    //The comand of 0(LOW) makes motor B move backward
}   

void RightForward(int x,int y){ //Right command  
  //motor A    
  analogWrite (PWMA,y/2);     //Motor A is powered at speed y
  digitalWrite (AIN1,1);    //The comand of 1(HIGH) makes motor A move forward

  //Motor B    
  analogWrite(PWMB, y);     //Motor B is powered at speed y
  digitalWrite (BIN1,1);    //The comand of 0(LOW) makes motor B move backward

  // In the case of the Right command, Motor A is moving forward at full speed and Motor B is moving backwards at full speed
  // causing the Car to spin to the Right in the same spot.
}   

void LeftForward(int x,int y){ //Left command  
  //motor A    
  analogWrite (PWMA,y);     //Motor A is powered at speed y
  digitalWrite (AIN1,1);    //The comand of 0(LOW) makes motor A move backward

  //Motor B    
  analogWrite(PWMB, y/2);     //Motor B is powered at speed y
  digitalWrite (BIN1,1);    //The comand of 1(HIGH) makes motor B move forward

  // In the case of the Left command, Motor A is moving backward at full speed and Motor B is moving forwards at full speed
  // causing the Car to spin to the Left in the same spot.
}   

//void Right(int NormSPEED){ //Right command  
  //motor A    
  //analogWrite (PWMA, NormSPEED);     
  //digitalWrite (AIN1,1);    

  //Motor B    
  //analogWrite(PWMB, NormSPEED);    
  //digitalWrite (BIN1,0);
//}

void Standby(){ //Standby command  
  //motor A     
  analogWrite (PWMA,0);     //Motor A is powered at speed 0

  //Motor B       
  analogWrite (PWMB,0);     //Motor B is powered at speed 0
}       

//void Ultrasonic(){  //Ultrasonic command
float Ultrasonic(){  //Ultrasonic command
  // Write a pulse to the HC-SR04 Trigger Pin 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the response from the HC-SR04 Echo Pin
  float duration = pulseIn(echoPin, HIGH);

  // Determine distance from duration  
  // Use 343 meters per second as speed of sound
 float distance = (duration / 2) * 0.0343; 
  return distance;
}



void setup(){

  Serial.begin(9600);

  // Turn the Motors pins into Outputs
  pinMode(AIN1, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(PWMB, OUTPUT);
  digitalWrite(STBY, 1);

  // Declare the pupose of each Ultrasonic pin
  pinMode(trigPin, OUTPUT);  // trig pin outputs sonic signals
  pinMode(echoPin, INPUT);   // trig pin recieves sonic signals that bounced back

  // Attaches the servo on pin 10 to the Servo Object
  myservo.attach(10);
}



void loop() {
 float distance = Ultrasonic();
  
  if (distance >= 25){   //if false that Mommy is less than 25 cm in Front of the Duck, run next line of code.
    myservo.write(0);     ////Servo turn Right
    delay(200);            ///for 0.2 seconds
    distance = Ultrasonic();         ////Run Ultrasonic command
    delay(200);            ///for 0.2 seconds

    if (distance >= 25){   //if false that Mommy is less than 25 cm to the Right of the Duck, run next line of code.
      myservo.write(180);   ////Servo turn Left
      delay(200);            ///for 0.2 seconds
      distance = Ultrasonic();         ////Run Ultrasonic command
      delay(200);            ///for 0.2 seconds

      if (distance >= 25){   //if false that Mommy is less than 25 cm to the Left of the Duck, run next line of code.
        myservo.write(90);   ////Servo turn Forward
        delay(200);            ///for 0.2 seconds
        distance = Ultrasonic();         ////Run Ultrasonic command
        delay(200);            ///for 0.2 seconds

        //if (distance >= 25){   //if false that Mommy is less than 25 cm in Front of the Duck, run next line of code.
      
         // myservo.write(90);   ////Servo turn Forward
         // delay(200);            ///for 0.2 seconds
         // distance = Ultrasonic();         ////Run Ultrasonic command
         // delay(200);            ///for 0.2 seconds
        //}

        //else{                  //if true that Mommy is less than 25 cm to the right of the car, run next line of code.
          //Standby();   ////Duck's Body turns Right, at 100 speed
          //delay(2000);            ///for 0.5 seconds
          //myservo.write(90);    ////Servo reajusts by turning Forward
          //delay(200);            ///for 0.2 seconds
          //distance = Ultrasonic();         ////Run Ultrasonic command
          //delay(100);            ///for 0.1 seconds
        //}
      }

      else{                  //if true that Mommy is less than 25 cm to the right of the car, run next line of code.
        LeftForward(0,NormSPEED);   ////Duck's Body turns Right, at 100 speed
        delay(200);            ///for 0.5 seconds
        myservo.write(90);    ////Servo reajusts by turning Forward
        delay(200);            ///for 0.2 seconds
        distance = Ultrasonic();         ////Run Ultrasonic command
        delay(100);            ///for 0.1 seconds
      }
    }
    
    else{                  //if true that Mommy is less than 25 cm to the right of the car, run next line of code.
      RightForward(0,NormSPEED);   ////Duck's Body turns Right, at 100 speed
      delay(200);            ///for 0.5 seconds
      myservo.write(90);    ////Servo reajusts by turning Forward
      delay(200);            ///for 0.2 seconds
      distance = Ultrasonic();         ////Run Ultrasonic command
      delay(100);            ///for 0.1 seconds
    }
  }
  
  else{                  //if true that Mommy is less than 25 cm in front of the Duck, run next line of code.
    Forward(0,NormSPEED); ////Duck goes Forward, at 100 speed
    delay(200);            ///for 0.2 seconds
    myservo.write(90);    ////Servo reajusts by turning Forward
    delay(200);            ///for 0.2 seconds
    distance = Ultrasonic();         ////Run Ultrasonic command
    delay(200);            ///for 0.1 seconds
  }
}
