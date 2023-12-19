//
// Created by denis on 19.12.23.
//

#include "inputproc.h"
#include "command.h"

void InputProcessor::process(const std::string &cmd_str)
{
    if (cmd_str == "{") {
        level++;

        if (level == 1) {
            send_block();
        }
    } else if (cmd_str == "}") {
        level--;
        if (level == 0) {
            send_block();
        }
    } else {
        // This is a command
        Command cmd(cmd_str);
        auto nelem = current_block->add_command(std::move(cmd));
        if (level == 0 && nelem >= block_size) {
            send_block();
        }
    }
}