#include "executor.h"
#include "parser.h"

#include <iostream>

Executor::Executor()
{
}

Executor::~Executor()
{
}

CommandResult Executor::perform(const std::string &cmd_str)
{
	std::cout << "Executing command: " << cmd_str << std::endl;

	try {

		Parser parser(cmd_str);
		auto cmd = parser.parse();

		return cmd->run();
	} catch (std::exception &e) {
		return CommandResult(e.what());
	}
}
