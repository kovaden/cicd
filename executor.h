#ifndef __EXECUTOR_H_LOADED___
#define __EXECUTOR_H_LOADED___

#include <string>
#include <memory>

class Parser;
class CommandResult;

class Executor
{
	public:
		Executor();
		virtual ~Executor();
		virtual CommandResult perform(const std::string &cmd);
	private:

};


#endif