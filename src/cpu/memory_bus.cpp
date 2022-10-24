#include "memory_bus.h"

Byte MemoeyBus::readByte(const Word& address) const {
    return (Byte)memory[address];
}