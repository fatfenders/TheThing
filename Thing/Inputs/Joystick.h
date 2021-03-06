/*
 * Joystick.h
 *
 *  Created on: May 26, 2016
 *      Author: williamcoon
 */

#ifndef INPUTS_JOYSTICK_H_
#define INPUTS_JOYSTICK_H_

#include "Arduino.h"

#define POT_ZERO 512 //Value pot should read when joy at 0
#define DEAD_ZONE 50
#define MAX_SPEED 100
#define TURN_DAMP 0.75 //Value for slowing down turning

class Joystick {
public:
	Joystick(int xAxisPin, int yAxisPin);
	virtual ~Joystick();
	void init();
	void readJoystick();
	int getLeftSpeed();
	int getRightSpeed();

private:
	double mapValue(int rawVal, bool squared=true);
	int xAxisPin;
	int yAxisPin;
	int leftSpeed;
	int rightSpeed;
	double getX();
	double getY();
};

#endif /* INPUTS_JOYSTICK_H_ */
