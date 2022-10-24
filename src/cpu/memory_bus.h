#pragma once

#include "../common.h"

#define MEMORY_SIZE (0xffff)

/**
 * @brief a struct that holds the cpu memory bus
 */
struct MemoryBus {
    Byte memory[MEMORY_SIZE];

    /**
     * @brief default init for the memory bus;
     */
    MemoryBus();

    /**
     * @brief reads a byte from the memory bus
     * @param address the address to read from
     */
    Byte readByte(const Word& address) const;
    /**
     * @brief reads a word from the memory bus
     * @param address the address to read from
     */
    Word readWord(const Word& address) const;

    /**
     * @brief writes a byte to a given address
     * @param address the address to write to
     * @param value the value to write
     */
    void writeByte(const Word& address, const Byte& value);
    /**
     * @brief writes a word to a given address
     * @param address the address to write to
     * @param value the value to write
     */
    void writeWord(const Word& address, const Word& value);
};
