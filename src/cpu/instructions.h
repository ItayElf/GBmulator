#pragma once

// https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html

// 0x0*
#define NOP (0x00)
#define LD_BC_D16 (0x01)
#define LD_BC_A (0x02)
#define INC_BC (0x03)
#define INC_B (0x04)
#define DEC_B (0x05)
#define LD_B_D8 (0x06)
#define RLCA (0x07)
#define LD_A16_SP (0x08)
#define ADD_HL_BC (0x09)
#define LD_A_BC (0x0a)
#define DEC_BC (0x0b)
#define INC_C (0x0c)
#define DEC_C (0x0d)
#define LD_C_D8 (0x0e)
#define RRCA (0x0f)

// 0x1*
#define LD_DE_D16 (0x11)
#define LD_DE_A (0x12)
#define INC_DE (0x13)
#define INC_D (0x14)
#define DEC_D (0x15)
#define LD_D_D8 (0x16)
#define RLA (0x17)
#define JR_R8 (0x18)
#define ADD_HL_DE (0x19)
#define LD_A_DE (0x1a)
#define DEC_DE (0x1b)
#define INC_E (0x1c)
#define DEC_E (0x1d)
#define LD_E_D8 (0x1e)
#define RRA (0x1f)

// 0x2*
#define JR_NZ_R8 (0x20)
#define LD_HL_D16 (0x21)
#define LD_HLP_A (0x22)
#define INC_HL (0x23)
#define INC_H (0x24)
#define DEC_H (0x25)
#define LD_H_D8 (0x26)
#define DAA (0x27)
#define JR_Z_R8 (0x28)
#define ADD_HL_HL (0x29)
#define LD_A_HLP (0x2a)
#define DEC_HL (0x2b)
#define INC_L (0x2c)
#define DEC_L (0x2d)
#define LD_L_D8 (0x2e)
#define CPL (0x2f)

// 0x3*
#define JR_NC_R8 (0x30)
#define LD_SP_D16 (0x31)
#define LD_HLM_A (0x32)
#define INC_SP (0x33)
#define INC_HL_ADDR (0x34)
#define DEC_HL_ADDR (0x35)
#define LD_HL_D8 (0x36)
#define SCF (0x37)
#define JR_C_R8 (0x38)
#define ADD_HL_SP (0x39)
#define LD_A_HLM (0x3a)
#define DEC_SP (0x3b)
#define INC_A (0x3c)
#define DEC_A (0x3d)
#define LD_A_D8 (0x3e)
#define CCF (0x3f)

// 0x4*
#define LD_B_B (0x40)
#define LD_B_C (0x41)
#define LD_B_D (0x42)
#define LD_B_E (0x43)
#define LD_B_H (0x44)
#define LD_B_L (0x45)
#define LD_B_HL (0x46)
#define LD_B_A (0x47)
#define LD_C_B (0x48)
#define LD_C_C (0x49)
#define LD_C_D (0x4a)
#define LD_C_E (0x4b)
#define LD_C_H (0x4c)
#define LD_C_L (0x4d)
#define LD_C_HL (0x4e)
#define LD_C_A (0x4f)

// 0x5*
#define LD_D_B (0x50)
#define LD_D_C (0x51)
#define LD_D_D (0x52)
#define LD_D_E (0x53)
#define LD_D_H (0x54)
#define LD_D_L (0x55)
#define LD_D_HL (0x56)
#define LD_D_A (0x57)
#define LD_E_B (0x58)
#define LD_E_C (0x59)
#define LD_E_D (0x5a)
#define LD_E_E (0x5b)
#define LD_E_H (0x5c)
#define LD_E_L (0x5d)
#define LD_E_HL (0x5e)
#define LD_E_A (0x5f)

// 0x6*
#define LD_H_B (0x60)
#define LD_H_C (0x61)
#define LD_H_D (0x62)
#define LD_H_E (0x63)
#define LD_H_H (0x64)
#define LD_H_L (0x65)
#define LD_H_HL (0x66)
#define LD_H_A (0x67)
#define LD_L_B (0x68)
#define LD_L_C (0x69)
#define LD_L_D (0x6a)
#define LD_L_E (0x6b)
#define LD_L_H (0x6c)
#define LD_L_L (0x6d)
#define LD_L_HL (0x6e)
#define LD_L_A (0x6f)

// 0x7*
#define LD_HL_B (0x70)
#define LD_HL_C (0x71)
#define LD_HL_D (0x72)
#define LD_HL_E (0x73)
#define LD_HL_H (0x74)
#define LD_HL_L (0x75)
#define HALT (0x76)
#define LD_HL_A (0x77)
#define LD_A_B (0x78)
#define LD_A_C (0x79)
#define LD_A_D (0x7a)
#define LD_A_E (0x7b)
#define LD_A_H (0x7c)
#define LD_A_L (0x7d)
#define LD_A_HL (0x7e)
#define LD_A_A (0x7f)

// 0x8*
#define ADD_A_B (0x80)
#define ADD_A_C (0x81)
#define ADD_A_D (0x82)
#define ADD_A_E (0x83)
#define ADD_A_H (0x84)
#define ADD_A_L (0x85)
#define ADD_A_HL (0x86)
#define ADD_A_A (0x87)
#define ADC_A_B (0x88)
#define ADC_A_C (0x89)
#define ADC_A_D (0x8a)
#define ADC_A_E (0x8b)
#define ADC_A_H (0x8c)
#define ADC_A_L (0x8d)
#define ADC_A_HL (0x8e)
#define ADC_A_A (0x8f)

// 0x9*
#define SUB_B (0x90)
#define SUB_C (0x91)
#define SUB_D (0x92)
#define SUB_E (0x93)
#define SUB_H (0x94)
#define SUB_L (0x95)
#define SUB_HL (0x96)
#define SUB_A (0x97)
#define SBC_A_B (0x98)
#define SBC_A_C (0x99)
#define SBC_A_D (0x9a)
#define SBC_A_E (0x9b)
#define SBC_A_H (0x9c)
#define SBC_A_L (0x9d)
#define SBC_A_HL (0x9e)
#define SBC_A_A (0x9f)

// 0xa*
#define AND_B (0xa0)
#define AND_C (0xa1)
#define AND_D (0xa2)
#define AND_E (0xa3)
#define AND_H (0xa4)
#define AND_L (0xa5)
#define AND_HL (0xa6)
#define AND_A (0xa7)
#define XOR_B (0xa8)
#define XOR_C (0xa9)
#define XOR_D (0xaa)
#define XOR_E (0xab)
#define XOR_H (0xac)
#define XOR_L (0xad)
#define XOR_HL (0xae)
#define XOR_A (0xaf)

// 0xb*
#define OR_B (0xb0)
#define OR_C (0xb1)
#define OR_D (0xb2)
#define OR_E (0xb3)
#define OR_H (0xb4)
#define OR_L (0xb5)
#define OR_HL (0xb6)
#define OR_A (0xb7)
#define CP_B (0xb8)
#define CP_C (0xb9)
#define CP_D (0xba)
#define CP_E (0xbb)
#define CP_H (0xbc)
#define CP_L (0xbd)
#define CP_HL (0xbe)
#define CP_A (0xbf)

// 0xc*
#define RET_NZ (0xc0)
#define POP_BC (0xc1)
#define JP_NZ_A16 (0Xc2)
#define JP_A16 (0xc3)
#define CALL_NZ_A16 (0xc4)
#define PUSH_BC (0xc5)
#define ADD_A_D8 (0xc6)
#define RST_00H (0xc7)
#define RET_Z (0xc8)
#define RET (0xc9)
#define JP_Z_A16 (0Xca)
#define PREFIX_BYTE (0xcb)
#define CALL_Z_A16 (0xcc)
#define CALL_A16 (0xcd)
#define ADC_A_D8 (0xce)
#define RST_08H (0xcf)

// 0xd*
#define RET_NC (0xd0)
#define POP_DE (0xd1)
#define JP_NC_A16 (0Xd2)
#define CALL_NC_A16 (0xd4)
#define PUSH_DE (0xd5)
#define SUB_D8 (0xd6)
#define RST_10H (0xd7)
#define RET_C (0xd8)
#define RETI (0xd9)
#define JP_C_A16 (0Xda)
#define CALL_C_A16 (0xdc)
#define SBC_A_D8 (0xde)
#define RST_18H (0xdf)

// 0xe*
#define LDH_A8_A (0xe0)
#define POP_HL (0xe1)
#define LC_C_ADDR_A (0xe2)
#define PUSH_HL (0xe5)
#define AND_D8 (0xe6)
#define RST_20H (0xe7)
#define ADD_SP_R8 (0xe8)
#define JP_HL (0xe9)
#define LD_A16_A (0xea)
#define XOR_D8 (0xee)
#define RST_28H (0xef)

// 0xf*
#define LDH_A_A8 (0xf0)
#define POP_AF (0xf1)
#define LD_A_C_ADDR (0xf2)
#define DI (0xf3)
#define PUSH_AF (0xf5)
#define OR_D8 (0xf6)
#define RST_30H (0xf7)
#define LD_HL_SP_R8 (0xf8)
#define LD_SP_HL (0xf9)
#define LD_A_A16 (0xfa)
#define EI (0xfb)
#define CP_D8 (0xfe)
#define RST_38H (0xff)

// prefixed opcodes
// 0x0*
// #define