//* Auto-Following Command *//

//This code uses the HC-SR04 Ultasonic Sensor to measure distance.//
//Here it is paired with a SG90 Servo Motor to help it detect     //
//subjects in a field of 180°. This in turn helps the Elegoo Car  //
//to follow an object and eventualy another Elegoo Car.           //
//Code available on: //
// https://github.com/NiceNicoN/Robotics-2/blob/main/Auto-Follow_Mode.ino //


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

// Available Times //     These will be the the X variables when a command is called
int Stop = 0; 
int Time = 1000;  
int IRResponseTime = 0; 



void Forward(int x,int y){  //Forward command  
  //motor A    
  analogWrite  (PWMA,y);    //Motor A is powered at speed y
  digitalWrite (AIN1,1);    //The comand of 1(HIGH) makes motor A move forward

  //Motor B      
  analogWrite  (PWMB,y);    //Motor B is powered at speed y
  digitalWrite (BIN1,1);    //The comand of 1(HIGH) makes motor B move forward
} 

void Backward(int x,int y){ //Backward command  
  //motor A    
  analogWrite  (PWMA,y);    //Motor A is powered at speed y
  digitalWrite (AIN1,0);    //The comand of 0(LOW) makes motor A move backward

  //Motor B
  analogWrite (PWMB, y);    //Motor B is powered at speed y
  digitalWrite (BIN1,0);    //The comand of 0(LOW) makes motor B move backward
}   

void RightForward(int x,int y){ //Right command  
  //motor A    
  analogWrite (PWMA,y/2);       //Motor A is powered at half of speed y
  digitalWrite  (AIN1,1);       //The comand of 1(HIGH) makes motor A move forward

  //Motor B    
  analogWrite (PWMB, y);        //Motor B is powered at speed y
  digitalWrite (BIN1,1);        //The comand of 1(HIGH) makes motor B move forward

  // In the case of the Right command, Motor A is moving forward at half-speed and Motor B is moving forwards at full speed
  // causing the Duck to go in circles to the Right.
}   

void LeftForward(int x,int y){  //Left command  
  //motor A    
  analogWrite  (PWMA,y);        //Motor A is powered at speed y
  digitalWrite (AIN1,1);        //The comand of 1(HIGH) makes motor A move forward
  //Motor B    
  analogWrite(PWMB,y/2);        //Motor B is powered at half of speed y
  digitalWrite (BIN1,1);        //The comand of 1(HIGH) makes motor B move forward

  // In the case of the Left command, Motor A is moving forward at full speed and Motor B is moving forwards at half-speed
  // causing the Duck to go in circles to the Left.
}   

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
  
  if (distance >= 30){      /////if true that Mommy is more than or = 30 cm in FRONT of the Duck, run next line of code.
    myservo.write(0);         ///Servo turn RIGHT
    delay(200);                //for 0.2 seconds
    distance = Ultrasonic();  ///Run Ultrasonic command
    delay(200);                //for 0.2 seconds

    if (distance >= 30){      /////if true that Mommy is more than or = 30 cm to the RIGHT of the Duck, run next line of code.
      myservo.write(180);       ///Servo turn LEFT
      delay(200);                //for 0.2 seconds
      distance = Ultrasonic();  ///Run Ultrasonic command
      delay(200);                //for 0.2 seconds

      if (distance >= 30){      /////if true that Mommy is more than or = 30 cm to the LEFT of the Duck, run next line of code.
        Standby();                ///Run Duck in /Standby/ command
        delay(200);                //for 0.2 seconds
        myservo.write(90);        ///Servo turn FORWARD
        delay(200);                //for 0.2 seconds
        distance = Ultrasonic();  ///Run Ultrasonic command
        delay(200);                //for 0.2 seconds
      }

      else if (distance > 6 && distance < 30){      /////otherwise, if true that Mommy is more than 6 cm and less than 30 cm to the LEFT of the Duck, run next line of code.
        LeftForward(0,NormSPEED);                     ///Duck's Body turns LEFT, at 100 speed
        delay(200);                                    //for 0.5 seconds
        myservo.write(90);                            ///Servo reajusts by turning FORWARD
        delay(200);                                    //for 0.2 seconds
        distance = Ultrasonic();                      ///Run Ultrasonic command
        delay(100);                                    //for 0.1 seconds
      }

      else if (distance <= 6){      /////otherwise, otherwise, if true that Mommy is less than or = 6 cm to the LEFT of the Duck, run next line of code.
        Backward(0,NormSPEED);        ///Duck runs BACKWARDS, at 100 speed
        delay(200);                    //for 0.2 seconds
        myservo.write(90);            ///Servo reajusts by turning FORWARD
        delay(200);                    //for 0.2 seconds
        distance = Ultrasonic();      ///Run Ultrasonic command
        delay(200);                    //for 0.2 seconds
      }
    }
    
    else if (distance > 6 && distance < 30){      /////otherwise, if true that Mommy is more than 6 cm and less than 30 cm to the RIGHT of the Duck, run next line of code.
      RightForward(0,NormSPEED);                     ///Duck's Body turns RIGHT, at 100 speed
      delay(200);                                     //for 0.2 seconds
      myservo.write(90);                             ///Servo reajusts by turning FORWARD
      delay(200);                                     //for 0.2 seconds
      distance = Ultrasonic();                       ///Run Ultrasonic command
      delay(100);                                     //for 0.1 seconds
    }

    else if (distance <= 6){      /////otherwise, otherwise, if true that Mommy is less than or = 6 cm to the RIGHT of the Duck, run next line of code.
    Backward(0,NormSPEED);          ///Duck runs BACKWARDS, at 100 speed
    delay(200);                      //for 0.2 seconds
    myservo.write(90);              ///Servo reajusts by turning FORWARD
    delay(200);                      //for 0.2 seconds
    distance = Ultrasonic();        ///Run Ultrasonic command
    delay(200);                      //for 0.2 seconds
    }
  }

  else if (distance > 6 && distance < 30){      /////otherwise, if true that Mommy is more than 6 cm and less than 30 cm in FRONT of the Duck, run next line of code.
    Forward(0,NormSPEED);                          ///Duck's Body runs FORWARD, at 100 speed
    delay(200);                                     //for 0.2 seconds
    myservo.write(90);                             ///Servo reajusts by turning FORWARD
    delay(200);                                     //for 0.2 seconds
    distance = Ultrasonic();                       ///Run Ultrasonic command
    delay(100);                                     //for 0.1 seconds
  }
 
  else if (distance <= 6){      /////otherwise, otherwise, if true that Mommy is less than or = 6 cm in FRONT of the Duck, run next line of code.
    Backward(0,NormSPEED);        ///Duck goes BACKWARDS, at 100 speed
    delay(200);                    //for 0.2 seconds
    myservo.write(90);            ///Servo reajusts by turning FORWARD
    delay(200);                    //for 0.2 seconds
    distance = Ultrasonic();      ///Run Ultrasonic command
    delay(200);                    //for 0.2 seconds
  }
}
