#pragma once

#include "../common.h"
#include "memory_bus.h"
#include "registers.h"

/**
 * @brief a struct that represents the cpu of the emulator
 */
struct CPU {
    Registers registers;
    Word pc;
    MemoeyBus bus;
};