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
     * @brief reads an array from the memory bus
     * @param address the address to read from
     * @param buffer the buffer to store the result
     * @param size the size of the buffer
     */
    void readArray(const Word& address, Byte buffer[], const Word& size) const;

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
    /**
     * @brief writes an array of byte to a given address
     * @param address the address to write to
     * @param buffer the array to write
     * @param size the size of the array
     */
    void writeArray(const Byte& address, const Byte buffer[], const Word& size);
};
