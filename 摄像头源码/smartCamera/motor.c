#include "motor.h"
#include <wiringPi.h>

void motor_turn_left()
{
  	//using wiringPi Pin Number
  	int pin = LEFTRIGHT;              
  	if (wiringPiSetup () == -1)
    	exit (1) ;               
 
  	pinMode(pin, OUTPUT);

	int i;
  	for (i = 0; i < 2; i++){                      
  		digitalWrite(pin, HIGH) ;         
     	delay(0.5);        
      	digitalWrite(pin, LOW);
      	delay(1);        
  	}         
}


void motor_turn_right()
{
  	//using wiringPi Pin Number
  	int pin = LEFTRIGHT;              
  	if (wiringPiSetup () == -1)
    	exit (1) ;               
 
  	pinMode(pin, OUTPUT);

	int i;
  	for (i = 0; i < 2; i++){                      
  		digitalWrite(pin, HIGH) ;         
     	delay(2);        
      	digitalWrite(pin, LOW);
      	delay(1);        
  	}         
}

void motor_turn_up()
{
  	//using wiringPi Pin Number
  	int pin = UPDOWN;              
  	if (wiringPiSetup () == -1)
    	exit (1) ;               
 
  	pinMode(pin, OUTPUT);

	int i;
  	for (i = 0; i < 2; i++){                      
  		digitalWrite(pin, HIGH) ;         
     	delay(0.5);        
      	digitalWrite(pin, LOW);
      	delay(1);        
  	}         
}

void motor_turn_down()
{
  	//using wiringPi Pin Number
  	int pin = UPDOWN;              
  	if (wiringPiSetup () == -1)
    	exit (1) ;               
 
  	pinMode(pin, OUTPUT);

	int i;
  	for (i = 0; i < 2; i++){                      
  		digitalWrite(pin, HIGH) ;         
     	delay(2);        
      	digitalWrite(pin, LOW);
      	delay(1);        
  	}         
}

