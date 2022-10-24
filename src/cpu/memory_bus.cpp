#include "memory_bus.h"

#include <algorithm>

#define BYTE_OFF (0)

MemoryBus::MemoryBus() {
    std::fill_n(memory, MEMORY_SIZE, BYTE_OFF);
}

Byte MemoryBus::readByte(const Word& address) const {
    return (Byte)memory[address];
}

Word MemoryBus::readWord(const Word& address) const {
    Word least_significant_byte = (Word)readByte(address);
    Word most_significant_byte = (Word)readByte(address + 1);
    return (most_significant_byte << 8) | least_significant_byte;
}

void MemoryBus::writeByte(const Word& address, const Byte& value) {
    memory[address] = value;
}
void MemoryBus::writeWord(const Word& address, const Word& value) {
    Byte most_significant_byte = (Byte)((value & TOP_WORD) >> 8);
    Byte least_significant_byte = (Byte)(value & FULL_BYTE);
    memory[address] = least_significant_byte;
    memory[address + 1] = most_significant_byte;
}