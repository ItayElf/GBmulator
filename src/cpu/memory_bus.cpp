#include "memory_bus.h"

#include <algorithm>

#include "instructions.h"

#define BYTE_OFF (0)

#define BOOTSTRAP_LENGTH (0XFE)
#define BOOTSTRAP_ADDRESS (0x00)

#define NINTENDO_LOGO 0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0c, 0x00, 0x0d, 0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e, 0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99, 0xbb, 0xbb, 0x67, 0x63, 0x6e, 0x0e, 0xec, 0xcc, 0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e
#define NINTENDO_LOGO_ADDRESS (0x104)
#define NINTENDO_LOGO_SIZE (48)

const static Byte g_bootstrap[BOOTSTRAP_LENGTH] = {
    LD_SP_D16,
    0xff,
    0xff,
    XOR_A,
    LD_HL_D16,
    0xff,
    0x9f,
    LD_HLM_A,
    BIT_7_H,
    JR_NZ_R8,
    0xfb,
    LD_HL_D16,
    0x26,
    0xff,
    LD_C_D8,
    0x11,
    LD_A_D8,
    0X80,
    LD_HLM_A,
    LD_C_ADDR_A,
    INC_C,
    LD_A_D8,
    0XF3,
    LD_C_ADDR_A,
    LD_HLM_A,
    LD_A_D8,
    0X77,
    LD_HL_A,
    LD_A_D8,
    0XFC,
    LD_A16_A,
    0X047,
    0XFF,
    LD_DE_D16,
    0X04,
    0X01,
    LD_A_DE,
    CALL_A16,
    0X95,
    0X00,
    CALL_A16,
    0X96,
    0X00,
    INC_DE,
    LD_A_E,
    CP_D8,
    0X34,
    JR_NZ_R8,
    0XF3,
    LD_DE_D16,
    0XD8,
    0X00,
    LD_B_D8,
    0X08,
    LD_A_DE,
    INC_DE,
    LD_HLP_A,
    INC_HL,
    DEC_B,
    JR_NZ_R8,
    0XF9,
    LD_A_D8,
    0X19,
    LD_A16_A,
    0X10,
    0X99,
    LD_HL_D16,
    0X2F,
    0X99,
    LD_C_D8,
    0X0C,
    DEC_A,
    JR_Z_R8,
    0X08,
    LD_HLM_A,
    DEC_C,
    JR_NZ_R8,
    0XF9,
    LD_L_D8,
    0X0F,
    JR_R8,
    0XB8,
    LD_H_A,
    LD_A_D8,
    0X64,
    LD_D_A,
    LD_A16_A,
    0X42,
    0XFF,
    LD_A_D8,
    0X91,
    LD_A16_A,
    0X40,
    0XFF,
    INC_B,
    LD_E_D8,
    0X02,
    LD_C_D8,
    0X0C,
    LD_A_A16,
    0X44,
    0XFF,
    CP_D8,
    0X90,
    JR_NZ_R8,
    0XFA,
    DEC_C,
    JR_NZ_R8,
    0XF7,
    DEC_E,
    JR_NZ_R8,
    0XF2,
    LD_C_D8,
    0X13,
    INC_H,
    LD_A_H,
    LD_E_D8,
    0X83,
    CP_D8,
    0X62,
    JR_Z_R8,
    0X6,
    LD_E_D8,
    0XC1,
    CP_D8,
    0X64,
    JR_NZ_R8,
    0X06,
    // ADDR_0080
    LD_A_E,
    LD_C_ADDR_A,
    INC_C,
    LD_A_D8,
    0X87,
    LD_C_ADDR_A,
    LD_A_A16,
    0X42,
    0XFF,
    SUB_B,
    LD_A16_A,
    0X42,
    0XFF,
    DEC_D,
    JR_NZ_R8,
    0XD2,
    DEC_B,
    JR_NZ_R8,
    0X4F,
    LD_D_D8,
    0X20,
    JR_R8,
    0XCB,
    LD_C_A,
    LD_B_D8,
    0X04,
    PUSH_BC,
    RL_C,
    RLA,
    POP_BC,
    RL_C,
    RLA,
    DEC_B,
    JR_NZ_R8,
    0XF5,
    LD_HLP_A,
    INC_HL,
    LD_HLP_A,
    INC_HL,
    RET,
    // NINTENDO LOGO
    0xce,
    0xed,
    0x66,
    0x66,
    0xcc,
    0x0d,
    0x00,
    0x0b,
    0x03,
    0x73,
    0x00,
    0x83,
    0x00,
    0x0c,
    0x00,
    0x0d,
    0x00,
    0x08,
    0x11,
    0x1f,
    0x88,
    0x89,
    0x00,
    0x0e,
    0xdc,
    0xcc,
    0x6e,
    0xe6,
    0xdd,
    0xdd,
    0xd9,
    0x99,
    0xbb,
    0xbb,
    0x67,
    0x63,
    0x6e,
    0x0e,
    0xec,
    0xcc,
    0xdd,
    0xdc,
    0x99,
    0x9f,
    0xbb,
    0xb9,
    0x33,
    0x3e,
    // MORE VIDEO DATA
    0X3C,
    0X42,
    0XB9,
    0XA5,
    0X42,
    0X3C,
    // ADDR_00E0
    LD_HL_D16,
    0X04,
    0X01,
    LD_DE_D16,
    0XA8,
    0X00,
    LD_A_DE,
    INC_DE,
    CP_HL,
    JR_NZ_R8,
    0XFE,
    LD_B_D8,
    0X19,
    LD_A_B,
    ADD_A_HL,
    INC_HL,
    DEC_B,
    JR_NZ_R8,
    0XFB,
    ADD_A_HL,
    JR_NZ_R8,
    0XFE,
    LD_A_D8,
    0X01,
    LD_A16_A,
    0X50,
    0XFF,
};

const static Byte g_nintendoLogo[NINTENDO_LOGO_SIZE] = {NINTENDO_LOGO};

MemoryBus::MemoryBus() {
    std::fill_n(memory, MEMORY_SIZE, BYTE_OFF);
    writeArray(BOOTSTRAP_ADDRESS, g_bootstrap, BOOTSTRAP_LENGTH);
    writeArray(NINTENDO_LOGO_ADDRESS, g_nintendoLogo, NINTENDO_LOGO_SIZE);
}

Byte MemoryBus::readByte(const Word& address) const {
    return memory[address];
}
Word MemoryBus::readWord(const Word& address) const {
    Word least_significant_byte = (Word)readByte(address);
    Word most_significant_byte = (Word)readByte(address + 1);
    return (most_significant_byte << 8) | least_significant_byte;
}
void MemoryBus::readArray(const Word& address, Byte buffer[], const Word& size) const {
    for (Word i = 0; i < size; i++) {
        buffer[i] = readByte(address + i);
    }
}

void MemoryBus::writeByte(const Word& address, const Byte& value) {
    memory[address] = value;
}
void MemoryBus::writeWord(const Word& address, const Word& value) {
    Byte most_significant_byte = (Byte)((value & TOP_WORD) >> 8);
    Byte least_significant_byte = (Byte)(value & FULL_BYTE);
    writeByte(address, least_significant_byte);
    writeByte(address + 1, most_significant_byte);
}
void MemoryBus::writeArray(const Byte& address, const Byte buffer[], const Word& size) {
    for (Word i = 0; i < size; i++) {
        writeByte(address + i, buffer[i]);
    }
}