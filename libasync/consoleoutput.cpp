//
// Created by denis on 21.04.24.
//

#include "consoleoutput.h"

std::shared_ptr<IOutHandler> ConsoleHandler::instance = std::make_shared<ConsoleHandler>();
