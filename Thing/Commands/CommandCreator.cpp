/*
 * CommandCreator.cpp
 *
 *  Created on: Apr 27, 2015
 *      Author: williamcoon
 */

#include "CommandCreator.h"

#define THUMB_EXTENDED 5
#define INDEX_EXTENDED 25
#define MIDDLE_EXTENDED 20
#define RING_EXTENDED 23
#define PINKY_EXTENDED 25

#define THUMB_HOME -14
#define INDEX_HOME -48
#define MIDDLE_HOME -45
#define RING_HOME -40
#define PINKY_HOME -40
const char* PARAM_START = "(";
const char* PARAM_END = ")";
CommandCenter* CommandCreator::commandCenter = CommandCenter::getInstance();

int driveSpeed = 50;

CommandCreator::CommandCreator() {
	commandHash = StringHashTable();
	//Serial commands
	commandHash.put("print", printSomething);
	commandHash.put("moveFinger", moveFinger);
	commandHash.put("moveFingerSpeed", moveFingerWithSpeed);
	commandHash.put("moveHand", moveHand);
	commandHash.put("drive", drive);
	commandHash.put("bnf", backAndForth);
	commandHash.put("calibrate", calibrateVictor);
	commandHash.put("findHome", findHome);
	commandHash.put("wiggle", wiggleFingers);
	commandHash.put("testParallel", testParallel);
	commandHash.put("reset", resetFingers);
	commandHash.put("moveWrist", moveWrist);
	commandHash.put("crawl", crawlForward);
	commandHash.put("beckon", beckon);
	commandHash.put("resetRFID", resetRFID);
	commandHash.put("foldUp", foldUp);

	//RFID tags
	commandHash.put("78007C960290", pointerFinger);
	commandHash.put("78007C878407", crawlForward);
	commandHash.put("770097BB510A", crawlForward);
	commandHash.put("78007C8FC348", resetFingers);
	commandHash.put("78007C555E0F", resetFingers);
	commandHash.put("78007C9FD348", resetFingers);
	commandHash.put("77000EB416DB", middleFinger);
	commandHash.put("78007C70C4B0", peaceSign);
	commandHash.put("78007C8771F2", hangTenSign);
	commandHash.put("78007C5E84DE", metalSign);
	commandHash.put("78007C567123", metalSign);
	commandHash.put("78007C9F3EA5", highFive);
	commandHash.put("770097E2F2F0", shocker);
	commandHash.put("7700979ECAB4", shocker);
	commandHash.put("78007C8844C8", foldUp);
	commandHash.put("770097AAC18B", beckon);
	commandHash.put("78007C375063", tickle);
	commandHash.put("770097B74B1C", tickle);
	commandHash.put("78008009C435", count);
	commandHash.put("770097A5C88D", count);
	commandHash.put("770097EEE5EB", reverseMiddle);
	commandHash.put("78007C756011", reverseMiddle);
	commandHash.put("770097E49692", flap);
	commandHash.put("78007C9ABB25", crazy);
	commandHash.put("6C008F0A638A", crazy);
	commandHash.put("770097EE000E", love);
	commandHash.put("78007C4CBBF3", love);

}

CommandCreator::~CommandCreator() {
	// TODO Auto-generated destructor stub
}

bool CommandCreator::createCommand(String commandString){
	Parameters *params = NULL;
	int paramStart = commandString.indexOf(PARAM_START);
	int paramEnd = commandString.indexOf(PARAM_END);
	if(paramStart>0){
		//Get parameters
		String paramString = commandString.substring(paramStart+1, paramEnd);
		commandString.remove(paramStart);
		params = parseParameters(paramString);
	}
	Serial.print("New command: ");
	Serial.println(commandString);
	fptr cmdPtr = commandHash.get(commandString);
	if(cmdPtr==NULL){
		Serial.println("Unrecognized Command");
		delete params;
		params = NULL;
		return false;
	}else{
		bool success = cmdPtr(params);
		delete params;
		params = NULL;
		return success;
	}
}

/*
 * parseParameters(String)
 * Parse a string of the format "param1,param2,param3,..." and create
 * a parameters object from them
 */
Parameters* CommandCreator::parseParameters(String paramString){
	Parameters *params = new Parameters();
	int commaIndex = paramString.indexOf(",");
	int paramCount = 0;
	do{
		String parameter = "";
		commaIndex = paramString.indexOf(",");
		if(commaIndex>0){
			parameter = paramString.substring(0,commaIndex);
			paramString.remove(0, commaIndex+1);
		}else{
			parameter = paramString;
		}
		params->setValue(paramCount, parameter);
		paramCount++;
	}while(commaIndex>=0);
	return params;
}

/********************************************
 * Gesture Commands
 ********************************************/

/*
 * pointerFinger(NULL)
 * extends just the index finger
 */
bool CommandCreator::pointerFinger(Parameters *params){
	Serial.println("Pointer Finger");
	CommandBase *handCommand = new MoveHand(0,0,0,INDEX_EXTENDED,0);
	commandCenter->addCommand(handCommand);
	return true;
}

bool CommandCreator::oneFingerCrawl(Parameters *params){
	Serial.println("One Finger Crawl");
	Serial.println("One Finger Crawl"); //Add print statement to fix freeze on upload issue
	for(int i=0;i<3;i++){
		CommandBase *fingerUp = new MoveHand(0,0,0,INDEX_EXTENDED,0);
		commandCenter->addCommand(fingerUp);
		CommandBase *wristUp = new MoveWrist(100,true,1000);
		commandCenter->addCommand(wristUp);
		CommandBase *fist = new MoveHand(0,0,0,0,0);
		fist->setParallel(true);
		commandCenter->addCommand(fist);
//		CommandBase *drive = new Drive(driveSpeed, driveSpeed, 1);
//		commandCenter->addCommand(drive);
	}
	return true;
}

/*
 * crawlForward(NULL)
 * Crawl forward. TODO: test this out and make it actually work
 */
bool CommandCreator::crawlForward(Parameters *params){
	Serial.println("Crawl Forward");
	CommandBase *wiggleCommand;
	//CommandBase *drive;
	CommandBase *handUp;
	CommandBase *wristUp;
	wristUp = new MoveWrist(100,false,2000);
	wristUp->setParallel(true);
	commandCenter->addCommand(wristUp);
	handUp = new MoveHand(18,18,18,18,THUMB_EXTENDED, 0);
	commandCenter->addCommand(handUp);
	for(int i=0;i<3;i++){
		wiggleCommand = new WiggleFingers(18, 0,THUMB_EXTENDED,0,300,3000,1);
		commandCenter->addCommand(wiggleCommand);
		wristUp = new MoveWrist(100,false,2000);
		wristUp->setParallel(true);
		commandCenter->addCommand(wristUp);
		handUp = new MoveHand(18,18,18,18,THUMB_EXTENDED, 0);
		commandCenter->addCommand(handUp);
	}
	resetFingers(NULL);
	return true;
}

/*
 * middleFinger(NULL)
 * I'm sure you can guess what this does.
 */
bool CommandCreator::middleFinger(Parameters *params){
	Serial.println("Middle Finger");
	CommandBase *handCommand = new MoveHand(0,0,MIDDLE_EXTENDED,0,0);
	commandCenter->addCommand(handCommand);
	return true;
}

/*
 * shocker(NULL)
 * I'm sure you can guess what this does.
 */
bool CommandCreator::shocker(Parameters *params){
	Serial.println("Shocker");
	CommandBase *handCommand = new MoveHand(PINKY_EXTENDED,0,MIDDLE_EXTENDED,INDEX_EXTENDED,0);
	commandCenter->addCommand(handCommand);
	return true;
}

/*
 * HangTenFinger(NULL)
 * Extend thumb and pinky
 */
bool CommandCreator::hangTenSign(Parameters *params){
	Serial.println("Hang Ten Sign");
	CommandBase *wristUp = new MoveWrist(100,false,2000);
	wristUp->setParallel(true);
	commandCenter->addCommand(wristUp);
	CommandBase *handCommand = new MoveHand(PINKY_EXTENDED,0,0,0,THUMB_EXTENDED,3000);
	commandCenter->addCommand(handCommand);
	wristUp = new MoveWrist(100,false,2000);
	commandCenter->addCommand(wristUp);
	return true;
}

/*
 * peaceSign(NULL)
 * Makes a peace sign
 */

bool CommandCreator::peaceSign(Parameters *params){
	Serial.println("Peace");
	CommandBase *handCommand = new MoveHand(0,0,MIDDLE_EXTENDED,INDEX_EXTENDED,0,0);
	commandCenter->addCommand(handCommand);
	CommandBase *wristUp = new MoveWrist(100,false,2000);
	commandCenter->addCommand(wristUp);
	return true;
}

/*
 * metalSign(NULL)
 * Makes the metal sign
 */
bool CommandCreator::metalSign(Parameters *params){
	Serial.println("Metal");
	CommandBase *wristUp = new MoveWrist(100,false,2000);
	commandCenter->addCommand(wristUp);
	wristUp->setParallel(true);
	CommandBase *handCommand = new MoveHand(PINKY_EXTENDED,0,0,INDEX_EXTENDED,0,3000);
	commandCenter->addCommand(handCommand);
	wristUp = new MoveWrist(100,false,2000);
	commandCenter->addCommand(wristUp);
	return true;
}

/*
 * highFive(NULL)
 * Puts all fingers out for a "High Five"
 */
bool CommandCreator::highFive(Parameters *params){
	Serial.println("High Five");
	CommandBase *handCommand = new MoveHand(PINKY_EXTENDED,RING_EXTENDED,MIDDLE_EXTENDED,INDEX_EXTENDED,THUMB_EXTENDED);
	commandCenter->addCommand(handCommand);
	return true;
}

/*
 * fist(NULL)
 * Puts all fingers in a fist like sign
 */
bool CommandCreator::fist(Parameters *params){
	Serial.println("High Five");
	CommandBase *handCommand = new MoveHand(0,0,0,0,0);
	commandCenter->addCommand(handCommand);
	return true;
}

/*
 * beckon(NULL)
 * summons the undead
 */

bool CommandCreator::beckon(Parameters *params){
	Serial.println("Beckon");
	CommandBase *handUp = new MoveHand(-PINKY_HOME, -RING_HOME, -MIDDLE_HOME, -INDEX_HOME, -THUMB_HOME);
	commandCenter->addCommand(handUp);
	for(int i = 0; i<2; i++){
		CommandBase *extendIndex = new MoveFinger(3,INDEX_EXTENDED);
		commandCenter->addCommand(extendIndex);
		CommandBase *retractIndex = new MoveFinger(3, -INDEX_HOME);
		commandCenter->addCommand(retractIndex);
	}
	return true;
}

bool CommandCreator::tickle(Parameters *params){
	Serial.println("Tickle");
	CommandBase *handUp = new MoveHand(-PINKY_HOME, -RING_HOME, -MIDDLE_HOME, -INDEX_HOME, -THUMB_HOME);
	commandCenter->addCommand(handUp);
	for(int i = 0; i<2; i++){
		CommandBase *extendIndex = new MoveFinger(3,INDEX_EXTENDED);
		extendIndex->setParallel(true);
		commandCenter->addCommand(extendIndex);
		CommandBase *extendMiddle = new MoveFinger(2,MIDDLE_EXTENDED);
		commandCenter->addCommand(extendMiddle);
		CommandBase *retractIndex = new MoveFinger(3, -INDEX_HOME);
		commandCenter->addCommand(retractIndex);
		retractIndex->setParallel(true);
		CommandBase *retractMiddle = new MoveFinger(2, -MIDDLE_HOME);
		commandCenter->addCommand(retractMiddle);
	}
	return true;
}

bool CommandCreator::count(Parameters *params){
	Serial.println("Count");
	CommandBase *closeHand = new MoveHand(0,0,0,0,0);
	commandCenter->addCommand(closeHand);
	CommandBase *moveFinger = new MoveFinger(3, INDEX_EXTENDED);
	commandCenter->addCommand(moveFinger);
	CommandBase *moveFinger1 = new MoveFinger(2, MIDDLE_EXTENDED);
	commandCenter->addCommand(moveFinger1);
	CommandBase *moveFinger2 = new MoveFinger(1, RING_EXTENDED);
	commandCenter->addCommand(moveFinger2);
	CommandBase *moveFinger3 = new MoveFinger(0, PINKY_EXTENDED);
	commandCenter->addCommand(moveFinger3);
	CommandBase *moveFinger4 = new MoveFinger(4, THUMB_EXTENDED);
	commandCenter->addCommand(moveFinger4);
	CommandBase *moveWrist = new MoveWrist(100,false,2000);
	commandCenter->addCommand(moveWrist);
	closeHand = new MoveHand(0,0,0,0,0);
	commandCenter->addCommand(closeHand);
	return true;
}

bool CommandCreator::reverseMiddle(Parameters *params){
	Serial.println("Reverse Middle");
	CommandBase *handUp = new MoveHand(-PINKY_HOME, -RING_HOME, -MIDDLE_HOME, -INDEX_HOME, -THUMB_HOME);
	handUp->setParallel(true);
	commandCenter->addCommand(handUp);
	CommandBase *wristUp = new MoveWrist(100,false,4000);
	commandCenter->addCommand(wristUp);
	CommandBase *holdWrist = new MoveWrist(20, false, 3000);
	holdWrist->setParallel(true);
	commandCenter->addCommand(holdWrist);
	CommandBase *middleFinger = new MoveHand(-PINKY_HOME, -RING_HOME, MIDDLE_EXTENDED, -INDEX_HOME, -THUMB_HOME, 4000);
	commandCenter->addCommand(middleFinger);
	CommandBase *retractMiddle = new MoveFinger(2,-MIDDLE_HOME);
	retractMiddle->setParallel(true);
	commandCenter->addCommand(retractMiddle);
	CommandBase *wristDown = new MoveWrist(50,true,1000);
	commandCenter->addCommand(wristDown);
	return true;
}

bool CommandCreator::flap(Parameters *params){
	Serial.println("Flap");
	for(int i=0;i<3;i++){
		CommandBase *moveWrist = new MoveWrist(100,false,3000);
		moveWrist->setParallel(true);
		commandCenter->addCommand(moveWrist);
		CommandBase *closeHand = new MoveHand(0,0,0,0,0);
		commandCenter->addCommand(closeHand);
		CommandBase *openHand = new MoveHand(PINKY_EXTENDED,RING_EXTENDED,MIDDLE_EXTENDED,INDEX_EXTENDED,THUMB_EXTENDED);
		commandCenter->addCommand(openHand);
	}
	return true;
}

bool CommandCreator::crazy(Parameters *params){
	Serial.println("Crazy");
	CommandBase *moveHand = new MoveHand(PINKY_EXTENDED,RING_EXTENDED,MIDDLE_EXTENDED,INDEX_EXTENDED,THUMB_EXTENDED);
	commandCenter->addCommand(moveHand);
	CommandBase *moveHand1 = new MoveHand(0, -RING_HOME, 0, -INDEX_HOME, 0, 4000);
	commandCenter->addCommand(moveHand1);
	CommandBase *moveHand2 = new MoveHand(0,0,0,0,0);
	commandCenter->addCommand(moveHand2);
	return true;
}

/*
 * love(NULL)
 *
 */
bool CommandCreator::love(Parameters *params){
	Serial.println("Love");
	CommandBase *wristUp = new MoveWrist(100,false,4000);
	commandCenter->addCommand(wristUp);
	wristUp->setParallel(true);
	CommandBase *handCommand = new MoveHand(PINKY_EXTENDED,0,0,INDEX_EXTENDED,THUMB_EXTENDED,3000);
	commandCenter->addCommand(handCommand);
	wristUp = new MoveWrist(30,true,1000);
	commandCenter->addCommand(wristUp);
	return true;
}


/********************************************
 * Special Hand Commands
 ********************************************/

/*
 * wiggleFingers(int minPosition, int maxPosition, int thumbMin, int thumbMax, int fingerDelay, int cycleDelay, int repeats)
 */
bool CommandCreator::wiggleFingers(Parameters *params){
	int minPosition;
	int maxPosition;
	int thumbMin;
	int thumbMax;
	int fingerDelay;
	int cycleDelay;
	int repeats;
	if(params->getInt(0, &minPosition)
			&&params->getInt(1, &maxPosition)
			&&params->getInt(2, &thumbMin)
			&&params->getInt(3, &thumbMax)
			&&params->getInt(4, &fingerDelay)
			&&params->getInt(5, &cycleDelay)
			&&params->getInt(6, &repeats))
	{
		CommandBase *command = new WiggleFingers(minPosition,
												maxPosition,
												thumbMin,
												thumbMax,
												fingerDelay,
												cycleDelay,
												repeats);
		commandCenter->addCommand(command);
		return true;
	}else{
		return false;
	}
}

bool CommandCreator::resetFingers(Parameters *params){
	Serial.println("Reset");
	CommandBase *command = new FindHome(0); //This tells the hand to run to reed limits
	commandCenter->addCommand(command);
	CommandBase *command2 = new MoveHand(PINKY_HOME,RING_HOME,MIDDLE_HOME,INDEX_HOME,THUMB_HOME,0);
	commandCenter->addCommand(command2);
	CommandBase *reset = new FindHome(-1); //This actually sets the home position
	commandCenter->addCommand(reset);
	return true;
}

bool CommandCreator::foldUp(Parameters *params){
	Serial.println("Folding up");
	CommandBase *command = new MoveHand(100, 100, 100, 100, 100);
	commandCenter->addCommand(command);
	return true;
}

/*
 * findHome(int speed)
 * calibrates the home position using the reed switches
 */
bool CommandCreator::findHome(Parameters *params){
	int speed;
	if(params->getInt(0, &speed)){
		CommandBase *command = new FindHome(speed);
		commandCenter->addCommand(command);
		return true;
	}else{
		return false;
	}
}

/********************************************
 * Driving Commands
 ********************************************/

/*
 * drive(int leftSpeed, int rightSpeed, int driveTime)
 * leftSpeed: Integer from -100 to 100
 * rightSpeed: Integer from -100 to 100
 * driveTime: Seconds to drive before stopping. Use -1 to set indefinitely.
 */

bool CommandCreator::driveForward(Parameters *params){
	Serial.println("Drive Forward");
//	CommandBase *command = new Drive(driveSpeed, driveSpeed, -1);
//	commandCenter->addCommand(command);
	return true;
}

bool CommandCreator::driveForwardThirty(Parameters *params){
	Serial.println("Drive Forward");
//	CommandBase *command = new Drive(driveSpeed, driveSpeed, 30);
//	commandCenter->addCommand(command);
	return true;
}


bool CommandCreator::turnLeft(Parameters *params){
	Serial.println("Turn left");
//	CommandBase *command = new Drive(30, 80, 4);
//	commandCenter->addCommand(command);
	return true;
}

bool CommandCreator::turnRight(Parameters *params){
	Serial.println("Turn right");
//	CommandBase *command = new Drive(80, 30, 4);
//	commandCenter->addCommand(command);
	return true;
}

/*
 * reverse(NULL)
 * run in reverse for ten seconds
 */
bool CommandCreator::reverse(Parameters *params){
	Serial.println("Reversing");
	CommandBase *handCommand = new MoveHand(0,0,0,0,0,0); //Do we need to close the hand first?
	commandCenter->addCommand(handCommand);
//	CommandBase *command = new Drive(-70, -70, 30);
//	commandCenter->addCommand(command);
	return true;
}

/********************************************
 * Debugging Commands - called from serial
 ********************************************/

/*
 * printSomething(String str, int numberOfRepeats)
 */
bool CommandCreator::printSomething(Parameters *params){
	String str;
	int numberOfRepeats;
	if(!params->getString(0, &str)){
		return false;
	}else if(!params->getInt(1, &numberOfRepeats)){
		return false;
	}else{
		CommandBase *command = new PrintCommand(str, numberOfRepeats);
		commandCenter->addCommand(command);
		return true;
	}
}

/*
 * moveFingerWithSpeed(int fingerIndex, int targetPosition, int speed)
 */
bool CommandCreator::moveFingerWithSpeed(Parameters *params){
	int fingerIndex;
	int targetPosition;
	int speed;
	if(params->getInt(0, &fingerIndex)&&params->getInt(1, &targetPosition)&&params->getInt(2, &speed)){
		CommandBase *command = new MoveFinger(fingerIndex, targetPosition, speed);
		commandCenter->addCommand(command);
		return true;
	}else{
		return false;
	}
}

/*
 * moveFinger(int fingerIndex, int targetPosition)
 */
bool CommandCreator::moveFinger(Parameters *params){
	int fingerIndex;
	int targetPosition;
	if(params->getInt(0, &fingerIndex)&&params->getInt(1, &targetPosition)){
		CommandBase *command = new MoveFinger(fingerIndex, targetPosition);
		commandCenter->addCommand(command);
		return true;
	}else{
		return false;
	}
}

/*
 * moveHand(int pinkyPosition, int ringPosition, int middlePosition, int indexPosition, int thumbPosition)
 */
bool CommandCreator::moveHand(Parameters *params){
	int pinkyPosition;
	int ringPosition;
	int middlePosition;
	int indexPosition;
	int thumbPosition;
	if(params->getInt(0, &pinkyPosition)
			&&params->getInt(1, &ringPosition)
			&&params->getInt(2, &middlePosition)
			&&params->getInt(3, &indexPosition)
			&&params->getInt(4, &thumbPosition))
	{
		CommandBase *command = new MoveHand(pinkyPosition,
											ringPosition,
											middlePosition,
											indexPosition,
											thumbPosition);
		commandCenter->addCommand(command);
		return true;
	}else{
		return false;
	}
}

bool CommandCreator::drive(Parameters *params){
	int leftSpeed;
	int rightSpeed;
	int driveTime;
	if(params->getInt(0, &leftSpeed)&&params->getInt(1, &rightSpeed)&&params->getInt(2, &driveTime)){
//		CommandBase *command = new Drive(leftSpeed, rightSpeed, driveTime);
//		commandCenter->addCommand(command);
		return true;
	}else{
		return false;
	}
}

bool CommandCreator::moveWrist(Parameters *params){
	int speed;
	bool direction;
	int moveTime;
	if(params->getInt(0, &speed)&&params->getBool(1, &direction)&&params->getInt(2, &moveTime)){
		CommandBase *command = new MoveWrist(speed, direction, moveTime);
		commandCenter->addCommand(command);
		return true;
	}else{
		return false;
	}
}

bool CommandCreator::backAndForth(Parameters *params){
	int fingerIndex;
	int speed;
	int cycles;
	if(params->getInt(0, &fingerIndex)&&params->getInt(1, &speed)&&params->getInt(2, &cycles)){
		CommandBase *command = new BackAndForth(fingerIndex, speed, cycles);
		commandCenter->addCommand(command);
		return true;
	}else{
		return false;
	}
}

/*
 * testParallel()
 * This is for testing/demonstrating how parallel and non-parallel
 * commands execute in the Command Center
 */
bool CommandCreator::testParallel(Parameters *params){
	CommandBase *command1 = new PrintCommand("Command 1 - ", 2);
	command1->setParallel(true);
	CommandBase *command2 = new PrintCommand("Command 2 - ", 4);
	command2->setParallel(true);
	CommandBase *command3 = new PrintCommand("Command 3 - ", 3);
	CommandBase *command4 = new PrintCommand("Command 4 - ", 5);
	command4->setParallel(true);
	CommandBase *command5 = new PrintCommand("Command 5 - ", 4);
	command5->setParallel(true);
	CommandBase *command6 = new PrintCommand("Command 6 - ", 3);
	commandCenter->addCommand(command1);
	commandCenter->addCommand(command2);
	commandCenter->addCommand(command3);
	commandCenter->addCommand(command4);
	commandCenter->addCommand(command5);
	commandCenter->addCommand(command6);
	return true;
}

/********************************************
 * Other Commands
 ********************************************/

/*
 * calibrateVictor(int pin)
 * pin: the control pin for the victor
 * This command bypasses the command center and just executes here. It shouldn't
 * be used during normal operation and will screw up any currently running commands.
 * Before running this command, power up the victor and hold the "Cal" button
 * down until it starts alternating RED/GREEN. Release the button after you see
 * "Calibration Complete" printed to the console. The victor should then blink
 * green to confirm successful calibration.
 */
bool CommandCreator::calibrateVictor(Parameters *params){
	int pin;
	if(params->getInt(0,&pin)){
		Victor victor(pin);
		victor.startServo();
		Serial.println("Calibrating...");
		victor.calibrate();
		Serial.println("Calibration Complete");
		return true;
	}else{
		return false;
	}
}

/*
 * resetRFID()
 * Reset RFID Reader so it looks for a new tag
 */
bool CommandCreator::resetRFID(Parameters *params){
	CommandBase *resetCommand = new ResetRFID();
	commandCenter->addCommand(resetCommand);
	return true;
}

/*
 * smallPoof()
 * A 100 millisecond pulse to the poofer solenoid
 */
bool CommandCreator::smallPoof(Parameters *params){
	Serial.println("Small Poof");
	CommandBase *command = new Poof(100UL);
	commandCenter->addCommand(command);
	return true;
}

/*
 * ejectBlock()
 * Eject an RFID block
 */
//bool CommandCreator::ejectBlock(Parameters *params){
//	CommandBase *ejectCommand = new EjectBlock();
//	commandCenter->addCommand(ejectCommand);
//	return true;
//}


