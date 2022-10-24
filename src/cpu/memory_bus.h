#pragma once

#include "../common.h"

#define MEMORY_SIZE (0xffff)

/**
 * @brief a struct that holds the cpu memory bus
 */
struct MemoeyBus {
    Byte memory[MEMORY_SIZE];

    /**
     * @brief reads a byte from the memory bus
     * @param address the address to read from
     */
    Byte readByte(const Word& address) const;
};
