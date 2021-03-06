/*
 * CommandBase.h
 *
 *  Created on: Apr 23, 2015
 *      Author: williamcoon
 */

#ifndef COMMANDS_COMMANDBASE_H_
#define COMMANDS_COMMANDBASE_H_

class CommandBase {
public:
	CommandBase();
	virtual ~CommandBase(){};
	virtual void init() = 0;
	virtual void execute() = 0;
	virtual void stop() = 0;
	virtual bool isFinished() = 0;
	bool isParallel();
	void setParallel(bool isParallel);
private:
	bool parallel;
};

#endif /* COMMANDS_COMMANDBASE_H_ */
