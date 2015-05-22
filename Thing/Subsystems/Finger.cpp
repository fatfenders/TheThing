/*
 * Finger.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: williamcoon
 */

#include <Finger.h>
#define TICK_TIMEOUT 500 //TODO: Calculate timeout based on speed
#define MIN_SPEED 20
#define EASE_FACTOR_1 0.3
#define EASE_FACTOR_2 0.5
#define EASE_DIST_1 1
#define EASE_DIST_2 2

Finger::Finger(int controlPin, int reed_pin, String name)
	:	fingerMotor(controlPin),
		targetPos(0),
		startPos(0),
		currentPos(0),
		targetSpeed(0),
		direction(FWD),
		finished(true),
		lastTickTime(0),
		enabled(true),
		reed_pin(reed_pin),
		name(name)
{

}

void Finger::startMotion(int targetPosition, int motionSpeed){
	//int targetPosition: The desired counter value to move to
	//double motionSpeed: The speed to move (number between 0 and 100)
	if(enabled){
		finished = false;
		targetPos = targetPosition;
		targetSpeed = motionSpeed;
		startPos = currentPos;
		direction = (targetPos>startPos)?FWD:REV;
		int adjustedSpeed = (int)(targetSpeed*EASE_FACTOR_1);
		int setSpeed = adjustedSpeed > MIN_SPEED ? adjustedSpeed:MIN_SPEED;
		lastTickTime = millis();
		if(direction==FWD){
			fingerMotor.setSpeed(setSpeed);
		}else{
			fingerMotor.setSpeed(-setSpeed);
		}
	}else{
		Serial.print(name);
		Serial.println(" is disabled.");
	}
}

void Finger::stopMotion(){
	//Set motor speed to zero;
	fingerMotor.setSpeed(0);
	finished = true;
}

void Finger::disableMotor(){
	//Call this if we're not seeing any sensor "ticks" even though we are powering the motor.
	//This is to prevent destroying a finger in the case that there is a fault in the hall effect sensor
	fingerMotor.setSpeed(0);
	finished = true;
	enabled = false;
	Serial.print(name);
	Serial.println(" has been disabled due to bad hall reading");
}

void Finger::update(){
	if(!finished){
		int targetDist = currentPos - targetPos;
		targetDist = abs(targetDist);
		int elapsedDist = currentPos - startPos;
		elapsedDist = abs(elapsedDist);
		if((millis()-lastTickTime)>TICK_TIMEOUT){
			disableMotor();
		}
		else if((direction&&currentPos>=targetPos)|(!direction&&currentPos<=targetPos)){
			stopMotion();
		}
		else if(direction==REV&&!digitalRead(reed_pin)){
			stopMotion();
			Serial.print(name);
			Serial.println(" stopped motion due to reed limit");
			setHomePosition();
		}else if(targetDist == EASE_DIST_1){
			//Use Ease Factor one to slow the motor
			int adjustedSpeed = (int)(targetSpeed*EASE_FACTOR_1);
			int setSpeed = adjustedSpeed > MIN_SPEED ? adjustedSpeed:MIN_SPEED;
			if(direction==FWD){
				fingerMotor.setSpeed(setSpeed);
			}else{
				fingerMotor.setSpeed(-setSpeed);
			}
		}else if((targetDist==EASE_DIST_2)||(elapsedDist == EASE_DIST_2 - 1)){
			//Use Ease Factor 2 to slow the motor
			int adjustedSpeed = (int)(targetSpeed*EASE_FACTOR_2);
			int setSpeed = adjustedSpeed > MIN_SPEED ? adjustedSpeed:MIN_SPEED;
			if(direction==FWD){
				fingerMotor.setSpeed(setSpeed);
			}else{
				fingerMotor.setSpeed(-setSpeed);
			}
		}else{
			if(direction==FWD){
				fingerMotor.setSpeed(targetSpeed);
			}else{
				fingerMotor.setSpeed(-targetSpeed);
			}
		}
	}
}

bool Finger::isFinished(){
	return finished;
}

void Finger::init(){
	fingerMotor.startServo();
}

void Finger::incrementCount(){
	currentPos += (direction==FWD) ? 1:-1; //Increment or decrement based on fwd/rev direction
	lastTickTime = millis(); //Reset counter to detect sensor malfunction
}

void Finger::setHomePosition(){
	currentPos = 0;
}

