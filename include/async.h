#pragma once

#include <cstddef>

namespace async {

using context_t = void *;

/*
 * Connect to the processor.
 */
context_t connect(std::size_t bulk);

/*
 * Receive set of commands. Should be performed only from one thread.
 */
void receive(context_t handle, const char *data, std::size_t size);

/*
 * Disconnect and release allocated resourced.
 */
void disconnect(context_t handle);

}
