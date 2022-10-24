#include "registers.h"

#define FLAG_OFF (0)
#define REGISTER_INIT_VALUE (0)

#define ZERO_FLAG_ON (0b10000000)
#define SUBTRACTION_FLAG_ON (0b01000000)
#define HALF_CARRY_FLAG_ON (0b00100000)
#define CARRY_FLAG_ON (0b00010000)

Registers::Registers() {
    a = b = c = d = e = f = h = l = REGISTER_INIT_VALUE;
}

Word Registers::getAf() const {
    return (Word)a << 8 | (Word)f;
}
Word Registers::getBc() const {
    return (Word)b << 8 | (Word)c;
}
Word Registers::getDe() const {
    return (Word)d << 8 | (Word)e;
}
Word Registers::getHl() const {
    return (Word)h << 8 | (Word)l;
}

bool Registers::isZeroFlag() const {
    return FLAG_OFF != f & ZERO_FLAG_ON;
}
bool Registers::isSubtractionFlag() const {
    return FLAG_OFF != f & SUBTRACTION_FLAG_ON;
}
bool Registers::isHalfCarryFlag() const {
    return FLAG_OFF != f & HALF_CARRY_FLAG_ON;
}
bool Registers::isCarryFlag() const {
    return FLAG_OFF != f & CARRY_FLAG_ON;
}

void Registers::setAf(const Word& v) {
    a = (Byte)((v & TOP_WORD) >> 8);
    f = (Byte)(v & FULL_BYTE);
}
void Registers::setBc(const Word& v) {
    b = (Byte)((v & TOP_WORD) >> 8);
    c = (Byte)(v & FULL_BYTE);
}
void Registers::setDe(const Word& v) {
    d = (Byte)((v & TOP_WORD) >> 8);
    e = (Byte)(v & FULL_BYTE);
}
void Registers::setHl(const Word& v) {
    h = (Byte)((v & TOP_WORD) >> 8);
    l = (Byte)(v & FULL_BYTE);
}

void Registers::setZeroFlag(const bool& value) {
    f = (value) ? f | ZERO_FLAG_ON : f ^ ZERO_FLAG_ON;
}
void Registers::setSubtractionFlag(const bool& value) {
    f = (value) ? f | SUBTRACTION_FLAG_ON : f ^ SUBTRACTION_FLAG_ON;
}
void Registers::setHalfCarryFlag(const bool& value) {
    f = (value) ? f | HALF_CARRY_FLAG_ON : f ^ HALF_CARRY_FLAG_ON;
}
void Registers::setCarryFlag(const bool& value) {
    f = (value) ? f | CARRY_FLAG_ON : f ^ CARRY_FLAG_ON;
}