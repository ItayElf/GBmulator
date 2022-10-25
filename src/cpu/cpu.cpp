#include "cpu.h"

#include <iostream>

#include "instructions.h"

#define HIGH_HALF_BYTE (0xf0)
#define LDH_START_ADDRESS (0xff00)

#define PC_START_VALUE (0x0000)
#define SP_START_VALUE (0xffff)

CPU::CPU() {
    registers = Registers();
    pc = PC_START_VALUE;
    sp = SP_START_VALUE;
    bus = MemoryBus();
    isHalted = false;
    areInturrptsEnabled = true;
}

void CPU::step() {
    Byte opcode = bus.readByte(pc);
    if (opcode == PREFIX_BYTE) {
        opcode = bus.readByte(pc + 1);
        pc++;
        pc = executePrefixed(opcode);
    } else {
        pc = execute(opcode);
    }
}

Word CPU::execute(const Byte& opcode) {
    if (isHalted) {
        return pc;
    }
    Byte value = 0;
    SignedByte signedValue = 0;
    Word wordValue = 0;
    switch (opcode) {
        // 0x0*
        case NOP:
            return pc + 1;
        case LD_BC_D16:
            registers.setBc(bus.readWord(pc + 1));
            return pc + 3;
        case LD_BC_A:
            bus.writeByte(registers.getBc(), registers.a);
            return pc + 1;
        case INC_BC:
            registers.setBc(registers.getBc() + 1);
            return pc + 1;
        case INC_B:
            registers.b += 1;
            return pc + 1;
        case DEC_B:
            registers.b -= 1;
            return pc + 1;
        case LD_B_D8:
            registers.b = bus.readByte(pc + 1);
            return pc + 2;
        case RLCA:
            registers.a = rlc(registers.a);
            return pc + 1;
        case LD_A16_SP:
            bus.writeWord(bus.readWord(pc + 1), sp);
            return pc + 3;
        case ADD_HL_BC:
            registers.setHl(add(registers.getBc()));
            return pc + 1;
        case LD_A_BC:
            registers.a = bus.readByte(registers.getBc());
            return pc + 1;
        case DEC_BC:
            registers.setBc(registers.getBc() - 1);
            return pc + 1;
        case INC_C:
            inc(registers.c);
            return pc + 1;
        case DEC_C:
            dec(registers.c);
            return pc + 1;
        case LD_C_D8:
            registers.c = bus.readByte(pc + 1);
            return pc + 2;
        case RRCA:
            registers.a = rrc(registers.a);
            return pc + 1;
        // 0x1*
        case LD_DE_D16:
            registers.setDe(bus.readWord(pc + 1));
            return pc + 3;
        case LD_DE_A:
            bus.writeByte(registers.getDe(), registers.a);
            return pc + 1;
        case INC_DE:
            registers.setDe(registers.getDe() + 1);
            return pc + 1;
        case INC_D:
            inc(registers.d);
            return pc + 1;
        case DEC_D:
            dec(registers.d);
        case LD_D_D8:
            registers.d = bus.readByte(pc + 1);
            return pc + 2;
        case RLA:
            registers.a = rl(registers.a);
            return pc + 1;
        case JR_R8:
            return jumpRelative(true, (SignedByte)bus.readByte(pc + 1));
        case ADD_HL_DE:
            registers.setHl(add(registers.getDe()));
            return pc + 1;
        case LD_A_DE:
            registers.a = bus.readByte(registers.getDe());
            return pc + 1;
        case DEC_DE:
            registers.setDe(registers.getDe() - 1);
            return pc + 1;
        case INC_E:
            inc(registers.e);
            return pc + 1;
        case DEC_E:
            dec(registers.e);
            return pc + 1;
        case LD_E_D8:
            registers.e = bus.readByte(pc + 1);
            return pc + 2;
        case RRA:
            registers.a = rr(registers.a);
            return pc + 1;
        // 0x2*
        case JR_NZ_R8:
            return jumpRelative(!registers.isZeroFlag(), (SignedByte)bus.readByte(pc + 1));
        case LD_HL_D16:
            registers.setHl(bus.readWord(pc + 1));
            return pc + 3;
        case LD_HLP_A:
            bus.writeByte(registers.getHl(), registers.a);
            registers.setHl(registers.getHl() + 1);
            return pc + 1;
        case INC_HL:
            registers.setHl(registers.getHl() + 1);
            return pc + 1;
        case INC_H:
            inc(registers.h);
            return pc + 1;
        case DEC_H:
            dec(registers.h);
            return pc + 1;
        case LD_H_D8:
            registers.h = bus.readByte(pc + 1);
            return pc + 2;
        case DAA:
            if (registers.isSubtractionFlag()) {
                if (registers.isCarryFlag()) {
                    registers.a -= 0x60;
                }
                if (registers.isHalfCarryFlag()) {
                    registers.a -= 0x06;
                }
            } else {
                if (registers.isCarryFlag() || registers.a > 0x99) {
                    registers.a += 0x60;
                    registers.setCarryFlag(true);
                }
                if (registers.isHalfCarryFlag() || (registers.a & 0x0f) > 0x09) {
                    registers.a += 0x06;
                }
            }
            registers.setZeroFlag(registers.a == 0);
            registers.setHalfCarryFlag(false);
            return pc + 1;
        case JR_Z_R8:
            return jumpRelative(registers.isZeroFlag(), (SignedByte)bus.readByte(pc + 1));
        case ADD_HL_HL:
            registers.setHl(add(registers.getHl()));
            return pc + 1;
        case LD_A_HLP:
            registers.a = bus.readByte(registers.getHl());
            registers.setHl(registers.getHl() - 1);
            return pc + 1;
        case DEC_HL:
            registers.setHl(registers.getHl() - 1);
            return pc + 1;
        case INC_L:
            inc(registers.l);
            return pc + 1;
        case DEC_L:
            dec(registers.l);
            return pc + 1;
        case LD_L_D8:
            registers.l = bus.readByte(pc + 1);
            return pc + 2;
        case CPL:
            registers.a = registers.a ^ FULL_BYTE;
            registers.setSubtractionFlag(true);
            registers.setHalfCarryFlag(true);
            return pc + 1;
        // 0x3*
        case JR_NC_R8:
            return jumpRelative(!registers.isCarryFlag(), (SignedByte)bus.readByte(pc + 1));
        case LD_SP_D16:
            sp = bus.readWord(pc + 1);
            return pc + 3;
        case LD_HLM_A:
            bus.writeByte(registers.getHl(), registers.a);
            registers.setHl(registers.getHl() - 1);
            return pc + 1;
        case INC_SP:
            sp++;
            return pc + 1;
        case INC_HL_ADDR:
            value = bus.readByte(registers.getHl());
            bus.writeByte(registers.getHl(), value + 1);
            return pc + 1;
        case DEC_HL_ADDR:
            value = bus.readByte(registers.getHl());
            bus.writeByte(registers.getHl(), value - 1);
            return pc + 1;
        case LD_HL_D8:
            bus.writeByte(registers.getHl(), bus.readByte(pc + 1));
            return pc + 1;
        case SCF:
            registers.setSubtractionFlag(false);
            registers.setHalfCarryFlag(false);
            registers.setCarryFlag(true);
            return pc + 1;
        case JR_C_R8:
            return jumpRelative(registers.isCarryFlag(), (SignedByte)bus.readByte(pc + 1));
        case ADD_HL_SP:
            registers.setHl(add(sp));
            return pc + 1;
        case LD_A_HLM:
            registers.a = bus.readByte(registers.getHl());
            registers.setHl(registers.getHl() - 1);
            return pc + 1;
        case DEC_SP:
            sp--;
            return pc + 1;
        case INC_A:
            inc(registers.a);
            return pc + 1;
        case DEC_A:
            dec(registers.a);
            return pc + 1;
        case LD_A_D8:
            registers.a = bus.readByte(pc + 1);
            return pc + 2;
        case CCF:
            registers.setSubtractionFlag(false);
            registers.setHalfCarryFlag(false);
            registers.setCarryFlag(!registers.isCarryFlag());
            return pc + 1;
        // 0x4*
        case LD_B_B:
            registers.b = registers.b;
            return pc + 1;
        case LD_B_C:
            registers.b = registers.c;
            return pc + 1;
        case LD_B_D:
            registers.b = registers.d;
            return pc + 1;
        case LD_B_E:
            registers.b = registers.e;
            return pc + 1;
        case LD_B_H:
            registers.b = registers.h;
            return pc + 1;
        case LD_B_L:
            registers.b = registers.l;
            return pc + 1;
        case LD_B_HL:
            registers.b = bus.readByte(registers.getHl());
            return pc + 1;
        case LD_B_A:
            registers.b = registers.a;
            return pc + 1;
        case LD_C_B:
            registers.c = registers.b;
            return pc + 1;
        case LD_C_C:
            registers.c = registers.c;
            return pc + 1;
        case LD_C_D:
            registers.c = registers.d;
            return pc + 1;
        case LD_C_E:
            registers.c = registers.e;
            return pc + 1;
        case LD_C_H:
            registers.c = registers.h;
            return pc + 1;
        case LD_C_L:
            registers.c = registers.l;
            return pc + 1;
        case LD_C_HL:
            registers.c = bus.readByte(registers.getHl());
            return pc + 1;
        case LD_C_A:
            registers.c = registers.a;
            return pc + 1;
        // 0x5*
        case LD_D_B:
            registers.d = registers.b;
            return pc + 1;
        case LD_D_C:
            registers.d = registers.c;
            return pc + 1;
        case LD_D_D:
            registers.d = registers.d;
            return pc + 1;
        case LD_D_E:
            registers.d = registers.e;
            return pc + 1;
        case LD_D_H:
            registers.d = registers.h;
            return pc + 1;
        case LD_D_L:
            registers.d = registers.l;
            return pc + 1;
        case LD_D_HL:
            registers.d = bus.readByte(registers.getHl());
            return pc + 1;
        case LD_D_A:
            registers.d = registers.a;
            return pc + 1;
        case LD_E_B:
            registers.e = registers.b;
            return pc + 1;
        case LD_E_C:
            registers.e = registers.c;
            return pc + 1;
        case LD_E_D:
            registers.e = registers.d;
            return pc + 1;
        case LD_E_E:
            registers.e = registers.e;
            return pc + 1;
        case LD_E_H:
            registers.e = registers.h;
            return pc + 1;
        case LD_E_L:
            registers.e = registers.l;
            return pc + 1;
        case LD_E_HL:
            registers.e = bus.readByte(registers.getHl());
            return pc + 1;
        case LD_E_A:
            registers.e = registers.a;
            return pc + 1;
        // 0x6*
        case LD_H_B:
            registers.h = registers.b;
            return pc + 1;
        case LD_H_C:
            registers.h = registers.c;
            return pc + 1;
        case LD_H_D:
            registers.h = registers.d;
            return pc + 1;
        case LD_H_E:
            registers.h = registers.e;
            return pc + 1;
        case LD_H_H:
            registers.h = registers.h;
            return pc + 1;
        case LD_H_L:
            registers.h = registers.l;
            return pc + 1;
        case LD_H_HL:
            registers.h = bus.readByte(registers.getHl());
            return pc + 1;
        case LD_H_A:
            registers.h = registers.a;
            return pc + 1;
        case LD_L_B:
            registers.l = registers.b;
            return pc + 1;
        case LD_L_C:
            registers.l = registers.c;
            return pc + 1;
        case LD_L_D:
            registers.l = registers.d;
            return pc + 1;
        case LD_L_E:
            registers.l = registers.e;
            return pc + 1;
        case LD_L_H:
            registers.l = registers.h;
            return pc + 1;
        case LD_L_L:
            registers.l = registers.l;
            return pc + 1;
        case LD_L_HL:
            registers.l = bus.readByte(registers.getHl());
            return pc + 1;
        case LD_L_A:
            registers.l = registers.a;
            return pc + 1;
        // 0x7*
        case LD_HL_B:
            bus.writeByte(registers.getHl(), registers.b);
            return pc + 1;
        case LD_HL_C:
            bus.writeByte(registers.getHl(), registers.c);
            return pc + 1;
        case LD_HL_D:
            bus.writeByte(registers.getHl(), registers.d);
            return pc + 1;
        case LD_HL_E:
            bus.writeByte(registers.getHl(), registers.e);
            return pc + 1;
        case LD_HL_H:
            bus.writeByte(registers.getHl(), registers.h);
            return pc + 1;
        case LD_HL_L:
            bus.writeByte(registers.getHl(), registers.l);
            return pc + 1;
        case HALT:
            isHalted = true;
            return pc + 1;
        case LD_HL_A:
            bus.writeByte(registers.getHl(), registers.a);
            return pc + 1;
        case LD_A_B:
            registers.a = registers.b;
            return pc + 1;
        case LD_A_C:
            registers.a = registers.c;
            return pc + 1;
        case LD_A_D:
            registers.a = registers.d;
            return pc + 1;
        case LD_A_E:
            registers.a = registers.e;
            return pc + 1;
        case LD_A_H:
            registers.a = registers.h;
            return pc + 1;
        case LD_A_L:
            registers.a = registers.l;
            return pc + 1;
        case LD_A_HL:
            registers.a = bus.readByte(registers.getHl());
            return pc + 1;
        case LD_A_A:
            registers.a = registers.a;
            return pc + 1;
        // 0x8*
        case ADD_A_B:
            registers.a = add(registers.b);
            return pc + 1;
        case ADD_A_C:
            registers.a = add(registers.c);
            return pc + 1;
        case ADD_A_D:
            registers.a = add(registers.d);
            return pc + 1;
        case ADD_A_E:
            registers.a = add(registers.e);
            return pc + 1;
        case ADD_A_H:
            registers.a = add(registers.h);
            return pc + 1;
        case ADD_A_L:
            registers.a = add(registers.l);
            return pc + 1;
        case ADD_A_HL:
            registers.a = add(bus.readByte(registers.getHl()));
            return pc + 1;
        case ADD_A_A:
            registers.a = add(registers.a);
            return pc + 1;
        case ADC_A_B:
            registers.a = adc(registers.b);
            return pc + 1;
        case ADC_A_C:
            registers.a = adc(registers.c);
            return pc + 1;
        case ADC_A_D:
            registers.a = adc(registers.d);
            return pc + 1;
        case ADC_A_E:
            registers.a = adc(registers.e);
            return pc + 1;
        case ADC_A_H:
            registers.a = adc(registers.h);
            return pc + 1;
        case ADC_A_L:
            registers.a = adc(registers.l);
            return pc + 1;
        case ADC_A_HL:
            registers.a = adc(bus.readByte(registers.getHl()));
            return pc + 1;
        case ADC_A_A:
            registers.a = adc(registers.a);
            return pc + 1;
        // 0x9*
        case SUB_B:
            registers.a = sub(registers.b);
            return pc + 1;
        case SUB_C:
            registers.a = sub(registers.c);
            return pc + 1;
        case SUB_D:
            registers.a = sub(registers.d);
            return pc + 1;
        case SUB_E:
            registers.a = sub(registers.e);
            return pc + 1;
        case SUB_H:
            registers.a = sub(registers.h);
            return pc + 1;
        case SUB_L:
            registers.a = sub(registers.l);
            return pc + 1;
        case SUB_HL:
            registers.a = sub(bus.readByte(registers.getHl()));
            return pc + 1;
        case SUB_A:
            registers.a = sub(registers.a);
            return pc + 1;
        case SBC_A_B:
            registers.a = sbc(registers.b);
            return pc + 1;
        case SBC_A_C:
            registers.a = sbc(registers.c);
            return pc + 1;
        case SBC_A_D:
            registers.a = sbc(registers.d);
            return pc + 1;
        case SBC_A_E:
            registers.a = sbc(registers.e);
            return pc + 1;
        case SBC_A_H:
            registers.a = sbc(registers.h);
            return pc + 1;
        case SBC_A_L:
            registers.a = sbc(registers.l);
            return pc + 1;
        case SBC_A_HL:
            registers.a = sbc(bus.readByte(registers.getHl()));
            return pc + 1;
        case SBC_A_A:
            registers.a = sbc(registers.a);
            return pc + 1;
        // 0xa*
        case AND_B:
            registers.a = logical_and(registers.b);
            return pc + 1;
        case AND_C:
            registers.a = logical_and(registers.c);
            return pc + 1;
        case AND_D:
            registers.a = logical_and(registers.d);
            return pc + 1;
        case AND_E:
            registers.a = logical_and(registers.e);
            return pc + 1;
        case AND_H:
            registers.a = logical_and(registers.h);
            return pc + 1;
        case AND_L:
            registers.a = logical_and(registers.l);
            return pc + 1;
        case AND_HL:
            registers.a = logical_and(bus.readByte(registers.getHl()));
            return pc + 1;
        case AND_A:
            registers.a = logical_and(registers.a);
            return pc + 1;
        case XOR_B:
            registers.a = logical_xor(registers.b);
            return pc + 1;
        case XOR_C:
            registers.a = logical_xor(registers.c);
            return pc + 1;
        case XOR_D:
            registers.a = logical_xor(registers.d);
            return pc + 1;
        case XOR_E:
            registers.a = logical_xor(registers.e);
            return pc + 1;
        case XOR_H:
            registers.a = logical_xor(registers.h);
            return pc + 1;
        case XOR_L:
            registers.a = logical_xor(registers.l);
            return pc + 1;
        case XOR_HL:
            registers.a = logical_xor(bus.readByte(registers.getHl()));
            return pc + 1;
        case XOR_A:
            registers.a = logical_xor(registers.a);
            return pc + 1;
        // 0xb*
        case OR_B:
            registers.a = logical_or(registers.b);
            return pc + 1;
        case OR_C:
            registers.a = logical_or(registers.c);
            return pc + 1;
        case OR_D:
            registers.a = logical_or(registers.d);
            return pc + 1;
        case OR_E:
            registers.a = logical_or(registers.e);
            return pc + 1;
        case OR_H:
            registers.a = logical_or(registers.h);
            return pc + 1;
        case OR_L:
            registers.a = logical_or(registers.l);
            return pc + 1;
        case OR_HL:
            registers.a = logical_or(bus.readByte(registers.getHl()));
            return pc + 1;
        case OR_A:
            registers.a = logical_or(registers.a);
            return pc + 1;
        case CP_B:
            sub(registers.b);
            return pc + 1;
        case CP_C:
            sub(registers.c);
            return pc + 1;
        case CP_D:
            sub(registers.d);
            return pc + 1;
        case CP_E:
            sub(registers.e);
            return pc + 1;
        case CP_H:
            sub(registers.h);
            return pc + 1;
        case CP_L:
            sub(registers.l);
            return pc + 1;
        case CP_HL:
            sub(bus.readByte(registers.getHl()));
            return pc + 1;
        case CP_A:
            sub(registers.a);
            return pc + 1;
        // 0xc*
        case RET_NZ:
            return ret(!registers.isZeroFlag());
        case POP_BC:
            registers.setBc(pop());
            return pc + 1;
        case JP_NZ_A16:
            return jump(!registers.isZeroFlag());
        case JP_A16:
            return jump(true);
        case CALL_NZ_A16:
            return call(!registers.isZeroFlag());
        case PUSH_BC:
            push(registers.getBc());
            return pc + 1;
        case ADD_A_D8:
            registers.a = add(bus.readByte(pc + 1));
            return pc + 2;
        case RST_00H:
            push(pc);
            return 0x00;
        case RET_Z:
            return ret(registers.isZeroFlag());
        case RET:
            return ret(true);
        case JP_Z_A16:
            return jump(registers.isZeroFlag());
        case CALL_Z_A16:
            return call(registers.isZeroFlag());
        case CALL_A16:
            return call(true);
        case ADC_A_D8:
            registers.a = adc(bus.readByte(pc + 1));
            return pc + 2;
        case RST_08H:
            push(pc);
            return 0x08;
        // 0xd*
        case RET_NC:
            return ret(!registers.isCarryFlag());
        case POP_DE:
            registers.setDe(pop());
            return pc + 1;
        case JP_NC_A16:
            return jump(!registers.isCarryFlag());
        case CALL_NC_A16:
            return call(!registers.isCarryFlag());
        case PUSH_DE:
            push(registers.getDe());
            return pc + 1;
        case SUB_D8:
            registers.a = sub(bus.readByte(pc + 1));
            return pc + 2;
        case RST_10H:
            push(pc);
            return 0x10;
        case RET_C:
            return ret(registers.isCarryFlag());
        case JP_C_A16:
            return jump(registers.isCarryFlag());
        case CALL_C_A16:
            return call(registers.isCarryFlag());
        case SBC_A_D8:
            registers.a = sbc(bus.readByte(pc + 1));
            return pc + 2;
        case RST_18H:
            push(pc);
            return 0x18;
        // 0xe*
        case LDH_A8_A:
            bus.writeByte(LDH_START_ADDRESS + bus.readByte(pc + 1), registers.a);
            return pc + 2;
        case POP_HL:
            registers.setHl(pop());
            return pc + 1;
        case LD_C_ADDR_A:
            bus.writeByte(bus.readByte(LDH_START_ADDRESS + registers.c), registers.a);
            return pc + 1;
        case PUSH_HL:
            push(registers.getHl());
            return pc + 1;
        case AND_D8:
            registers.a = logical_and(bus.readByte(pc + 1));
            return pc + 2;
        case RST_20H:
            push(pc);
            return 0x20;
        case ADD_SP_R8:
            signedValue = (SignedByte)bus.readByte(pc + 1);
            wordValue = sp + (SignedWord)signedValue;
            registers.setZeroFlag(false);
            registers.setSubtractionFlag(false);
            registers.setCarryFlag(wordValue < (SignedWord)signedValue);
            registers.setHalfCarryFlag((sp ^ signedValue ^ wordValue) & BYTE_SIZE != 0);
            sp = wordValue;
            return pc + 2;
        case JP_HL:
            return registers.getHl();
        case LD_A16_A:
            bus.writeByte(bus.readWord(pc + 1), registers.a);
            return pc + 3;
        case XOR_D8:
            registers.a = logical_xor(bus.readByte(pc + 1));
            return pc + 2;
        case RST_28H:
            push(pc);
            return 0x28;
        // 0xf*
        case LDH_A_A8:
            registers.a = bus.readByte(LDH_START_ADDRESS + bus.readByte(pc + 1));
            return pc + 2;
        case POP_AF:
            registers.setAf(pop());
            return pc + 1;
        case LD_A_C_ADDR:
            registers.a = bus.readByte(LDH_START_ADDRESS + registers.c);
            return pc + 1;
        case DI:
            areInturrptsEnabled = false;
            return pc + 1;
        case PUSH_AF:
            push(registers.getAf());
            return pc + 1;
        case OR_D8:
            registers.a = logical_or(bus.readByte(pc + 1));
            return pc + 2;
        case RST_30H:
            push(pc);
            return 0x30;
        case LD_HL_SP_R8:
            signedValue = (SignedByte)bus.readByte(pc + 1);
            wordValue = sp + (SignedWord)signedValue;
            registers.setZeroFlag(false);
            registers.setSubtractionFlag(false);
            registers.setCarryFlag(wordValue < (SignedWord)signedValue);
            registers.setHalfCarryFlag((sp ^ signedValue ^ wordValue) & BYTE_SIZE != 0);
            registers.setHl(wordValue);
            return pc + 2;
        case LD_SP_HL:
            sp = registers.getHl();
            return pc + 1;
        case LD_A_A16:
            registers.a = bus.readByte(bus.readWord(pc + 1));
            return pc + 3;
        case EI:
            areInturrptsEnabled = true;
        case CP_D8:
            sub(bus.readByte(pc + 1));
            return pc + 2;
        case RST_38H:
            push(pc);
            return 0x38;
        default:
            std::cout << "Unknown Opcode: " << (Word)opcode << "\n";
            throw 1;
            return pc;
    }
}

Word CPU::executePrefixed(const Byte& opcode) {
    if (isHalted) {
        return pc;
    }
    Byte byteValue = 0;
    switch (opcode) {
        // 0x0*
        case RLC_B:
            registers.b = rlc(registers.b);
            return pc + 1;
        case RLC_C:
            registers.c = rlc(registers.c);
            return pc + 1;
        case RLC_D:
            registers.d = rlc(registers.d);
            return pc + 1;
        case RLC_E:
            registers.e = rlc(registers.e);
            return pc + 1;
        case RLC_H:
            registers.h = rlc(registers.h);
            return pc + 1;
        case RLC_L:
            registers.l = rlc(registers.l);
            return pc + 1;
        case RLC_HL:
            bus.writeByte(registers.getHl(), rlc(bus.readByte(registers.getHl())));
            return pc + 1;
        case RLC_A:
            registers.a = rlc(registers.a);
            return pc + 1;
        case RRC_B:
            registers.b = rrc(registers.b);
            return pc + 1;
        case RRC_C:
            registers.c = rrc(registers.c);
            return pc + 1;
        case RRC_D:
            registers.d = rrc(registers.d);
            return pc + 1;
        case RRC_E:
            registers.e = rrc(registers.e);
            return pc + 1;
        case RRC_H:
            registers.h = rrc(registers.h);
            return pc + 1;
        case RRC_L:
            registers.l = rrc(registers.l);
            return pc + 1;
        case RRC_HL:
            bus.writeByte(registers.getHl(), rrc(bus.readByte(registers.getHl())));
            return pc + 1;
        case RRC_A:
            registers.a = rrc(registers.a);
            return pc + 1;
        // 0x1*
        case RL_B:
            registers.b = rl(registers.b);
            return pc + 1;
        case RL_C:
            registers.c = rl(registers.c);
            return pc + 1;
        case RL_D:
            registers.d = rl(registers.d);
            return pc + 1;
        case RL_E:
            registers.e = rl(registers.e);
            return pc + 1;
        case RL_H:
            registers.h = rl(registers.h);
            return pc + 1;
        case RL_L:
            registers.l = rl(registers.l);
            return pc + 1;
        case RL_HL:
            bus.writeByte(registers.getHl(), rl(bus.readByte(registers.getHl())));
            return pc + 1;
        case RL_A:
            registers.a = rl(registers.a);
            return pc + 1;
        case RR_B:
            registers.b = rr(registers.b);
            return pc + 1;
        case RR_C:
            registers.c = rr(registers.c);
            return pc + 1;
        case RR_D:
            registers.d = rr(registers.d);
            return pc + 1;
        case RR_E:
            registers.e = rr(registers.e);
            return pc + 1;
        case RR_H:
            registers.h = rr(registers.h);
            return pc + 1;
        case RR_L:
            registers.l = rr(registers.l);
            return pc + 1;
        case RR_HL:
            bus.writeByte(registers.getHl(), rr(bus.readByte(registers.getHl())));
            return pc + 1;
        case RR_A:
            registers.a = rr(registers.a);
            return pc + 1;
        // 0x2*
        case SLA_B:
            registers.b = sla(registers.b);
            return pc + 1;
        case SLA_C:
            registers.c = sla(registers.c);
            return pc + 1;
        case SLA_D:
            registers.d = sla(registers.d);
            return pc + 1;
        case SLA_E:
            registers.e = sla(registers.e);
            return pc + 1;
        case SLA_H:
            registers.h = sla(registers.h);
            return pc + 1;
        case SLA_L:
            registers.l = sla(registers.l);
            return pc + 1;
        case SLA_HL:
            bus.writeByte(registers.getHl(), sla(bus.readByte(registers.getHl())));
            return pc + 1;
        case SLA_A:
            registers.a = sla(registers.a);
            return pc + 1;
        case SRA_B:
            registers.b = sra(registers.b);
            return pc + 1;
        case SRA_C:
            registers.c = sra(registers.c);
            return pc + 1;
        case SRA_D:
            registers.d = sra(registers.d);
            return pc + 1;
        case SRA_E:
            registers.e = sra(registers.e);
            return pc + 1;
        case SRA_H:
            registers.h = sra(registers.h);
            return pc + 1;
        case SRA_L:
            registers.l = sra(registers.l);
            return pc + 1;
        case SRA_HL:
            bus.writeByte(registers.getHl(), sra(bus.readByte(registers.getHl())));
            return pc + 1;
        case SRA_A:
            registers.a = sra(registers.a);
            return pc + 1;
        // 0x3*
        case SWAP_B:
            registers.b = swap(registers.b);
            return pc + 1;
        case SWAP_C:
            registers.c = swap(registers.c);
            return pc + 1;
        case SWAP_D:
            registers.d = swap(registers.d);
            return pc + 1;
        case SWAP_E:
            registers.e = swap(registers.e);
            return pc + 1;
        case SWAP_H:
            registers.h = swap(registers.h);
            return pc + 1;
        case SWAP_L:
            registers.l = swap(registers.l);
            return pc + 1;
        case SWAP_HL:
            bus.writeByte(registers.getHl(), swap(bus.readByte(registers.getHl())));
            return pc + 1;
        case SWAP_A:
            registers.a = swap(registers.a);
            return pc + 1;
        case SRL_B:
            registers.b = srl(registers.b);
            return pc + 1;
        case SRL_C:
            registers.c = srl(registers.c);
            return pc + 1;
        case SRL_D:
            registers.d = srl(registers.d);
            return pc + 1;
        case SRL_E:
            registers.e = srl(registers.e);
            return pc + 1;
        case SRL_H:
            registers.h = srl(registers.h);
            return pc + 1;
        case SRL_L:
            registers.l = srl(registers.l);
            return pc + 1;
        case SRL_HL:
            bus.writeByte(registers.getHl(), srl(bus.readByte(registers.getHl())));
            return pc + 1;
        case SRL_A:
            registers.a = srl(registers.a);
            return pc + 1;
        // 0x4*
        case BIT_0_B:
            bit(0, registers.b);
            return pc + 1;
        case BIT_0_C:
            bit(0, registers.c);
            return pc + 1;
        case BIT_0_D:
            bit(0, registers.d);
            return pc + 1;
        case BIT_0_E:
            bit(0, registers.e);
            return pc + 1;
        case BIT_0_H:
            bit(0, registers.h);
            return pc + 1;
        case BIT_0_L:
            bit(0, registers.l);
            return pc + 1;
        case BIT_0_HL:
            bit(0, bus.readByte(registers.getHl()));
            return pc + 1;
        case BIT_0_A:
            bit(0, registers.a);
            return pc + 1;
        case BIT_1_B:
            bit(1, registers.b);
            return pc + 1;
        case BIT_1_C:
            bit(1, registers.c);
            return pc + 1;
        case BIT_1_D:
            bit(1, registers.d);
            return pc + 1;
        case BIT_1_E:
            bit(1, registers.e);
            return pc + 1;
        case BIT_1_H:
            bit(1, registers.h);
            return pc + 1;
        case BIT_1_L:
            bit(1, registers.l);
            return pc + 1;
        case BIT_1_HL:
            bit(1, bus.readByte(registers.getHl()));
            return pc + 1;
        case BIT_1_A:
            bit(1, registers.a);
            return pc + 1;
        // 0x5*
        case BIT_2_B:
            bit(2, registers.b);
            return pc + 1;
        case BIT_2_C:
            bit(2, registers.c);
            return pc + 1;
        case BIT_2_D:
            bit(2, registers.d);
            return pc + 1;
        case BIT_2_E:
            bit(2, registers.e);
            return pc + 1;
        case BIT_2_H:
            bit(2, registers.h);
            return pc + 1;
        case BIT_2_L:
            bit(2, registers.l);
            return pc + 1;
        case BIT_2_HL:
            bit(2, bus.readByte(registers.getHl()));
            return pc + 1;
        case BIT_2_A:
            bit(2, registers.a);
            return pc + 1;
        case BIT_3_B:
            bit(3, registers.b);
            return pc + 1;
        case BIT_3_C:
            bit(3, registers.c);
            return pc + 1;
        case BIT_3_D:
            bit(3, registers.d);
            return pc + 1;
        case BIT_3_E:
            bit(3, registers.e);
            return pc + 1;
        case BIT_3_H:
            bit(3, registers.h);
            return pc + 1;
        case BIT_3_L:
            bit(3, registers.l);
            return pc + 1;
        case BIT_3_HL:
            bit(3, bus.readByte(registers.getHl()));
            return pc + 1;
        case BIT_3_A:
            bit(3, registers.a);
            return pc + 1;
        // 0x6*
        case BIT_4_B:
            bit(4, registers.b);
            return pc + 1;
        case BIT_4_C:
            bit(4, registers.c);
            return pc + 1;
        case BIT_4_D:
            bit(4, registers.d);
            return pc + 1;
        case BIT_4_E:
            bit(4, registers.e);
            return pc + 1;
        case BIT_4_H:
            bit(4, registers.h);
            return pc + 1;
        case BIT_4_L:
            bit(4, registers.l);
            return pc + 1;
        case BIT_4_HL:
            bit(4, bus.readByte(registers.getHl()));
            return pc + 1;
        case BIT_4_A:
            bit(4, registers.a);
            return pc + 1;
        case BIT_5_B:
            bit(5, registers.b);
            return pc + 1;
        case BIT_5_C:
            bit(5, registers.c);
            return pc + 1;
        case BIT_5_D:
            bit(5, registers.d);
            return pc + 1;
        case BIT_5_E:
            bit(5, registers.e);
            return pc + 1;
        case BIT_5_H:
            bit(5, registers.h);
            return pc + 1;
        case BIT_5_L:
            bit(5, registers.l);
            return pc + 1;
        case BIT_5_HL:
            bit(5, bus.readByte(registers.getHl()));
            return pc + 1;
        case BIT_5_A:
            bit(5, registers.a);
            return pc + 1;
        // 0x7*
        case BIT_6_B:
            bit(6, registers.b);
            return pc + 1;
        case BIT_6_C:
            bit(6, registers.c);
            return pc + 1;
        case BIT_6_D:
            bit(6, registers.d);
            return pc + 1;
        case BIT_6_E:
            bit(6, registers.e);
            return pc + 1;
        case BIT_6_H:
            bit(6, registers.h);
            return pc + 1;
        case BIT_6_L:
            bit(6, registers.l);
            return pc + 1;
        case BIT_6_HL:
            bit(6, bus.readByte(registers.getHl()));
            return pc + 1;
        case BIT_6_A:
            bit(6, registers.a);
            return pc + 1;
        case BIT_7_B:
            bit(7, registers.b);
            return pc + 1;
        case BIT_7_C:
            bit(7, registers.c);
            return pc + 1;
        case BIT_7_D:
            bit(7, registers.d);
            return pc + 1;
        case BIT_7_E:
            bit(7, registers.e);
            return pc + 1;
        case BIT_7_H:
            bit(7, registers.h);
            return pc + 1;
        case BIT_7_L:
            bit(7, registers.l);
            return pc + 1;
        case BIT_7_HL:
            bit(7, bus.readByte(registers.getHl()));
            return pc + 1;
        case BIT_7_A:
            bit(7, registers.a);
            return pc + 1;
        // 0x8*
        case RES_0_B:
            res(0, registers.b);
            return pc + 1;
        case RES_0_C:
            res(0, registers.c);
            return pc + 1;
        case RES_0_D:
            res(0, registers.d);
            return pc + 1;
        case RES_0_E:
            res(0, registers.e);
            return pc + 1;
        case RES_0_H:
            res(0, registers.h);
            return pc + 1;
        case RES_0_L:
            res(0, registers.l);
            return pc + 1;
        case RES_0_HL:
            byteValue = bus.readByte(registers.getHl());
            res(0, byteValue);
            bus.writeByte(registers.getHl(), byteValue);
            return pc + 1;
        case RES_0_A:
            res(0, registers.a);
            return pc + 1;
        case RES_1_B:
            res(1, registers.b);
            return pc + 1;
        case RES_1_C:
            res(1, registers.c);
            return pc + 1;
        case RES_1_D:
            res(1, registers.d);
            return pc + 1;
        case RES_1_E:
            res(1, registers.e);
            return pc + 1;
        case RES_1_H:
            res(1, registers.h);
            return pc + 1;
        case RES_1_L:
            res(1, registers.l);
            return pc + 1;
        case RES_1_HL:
            byteValue = bus.readByte(registers.getHl());
            res(1, byteValue);
            bus.writeByte(registers.getHl(), byteValue);
            return pc + 1;
        case RES_1_A:
            res(1, registers.a);
            return pc + 1;
        // 0x9*
        case RES_2_B:
            res(2, registers.b);
            return pc + 1;
        case RES_2_C:
            res(2, registers.c);
            return pc + 1;
        case RES_2_D:
            res(2, registers.d);
            return pc + 1;
        case RES_2_E:
            res(2, registers.e);
            return pc + 1;
        case RES_2_H:
            res(2, registers.h);
            return pc + 1;
        case RES_2_L:
            res(2, registers.l);
            return pc + 1;
        case RES_2_HL:
            byteValue = bus.readByte(registers.getHl());
            res(2, byteValue);
            bus.writeByte(registers.getHl(), byteValue);
            return pc + 1;
        case RES_2_A:
            res(2, registers.a);
            return pc + 1;
        case RES_3_B:
            res(3, registers.b);
            return pc + 1;
        case RES_3_C:
            res(3, registers.c);
            return pc + 1;
        case RES_3_D:
            res(3, registers.d);
            return pc + 1;
        case RES_3_E:
            res(3, registers.e);
            return pc + 1;
        case RES_3_H:
            res(3, registers.h);
            return pc + 1;
        case RES_3_L:
            res(3, registers.l);
            return pc + 1;
        case RES_3_HL:
            byteValue = bus.readByte(registers.getHl());
            res(3, byteValue);
            bus.writeByte(registers.getHl(), byteValue);
            return pc + 1;
        case RES_3_A:
            res(3, registers.a);
            return pc + 1;
        // 0xa*
        case RES_4_B:
            res(4, registers.b);
            return pc + 1;
        case RES_4_C:
            res(4, registers.c);
            return pc + 1;
        case RES_4_D:
            res(4, registers.d);
            return pc + 1;
        case RES_4_E:
            res(4, registers.e);
            return pc + 1;
        case RES_4_H:
            res(4, registers.h);
            return pc + 1;
        case RES_4_L:
            res(4, registers.l);
            return pc + 1;
        case RES_4_HL:
            byteValue = bus.readByte(registers.getHl());
            res(4, byteValue);
            bus.writeByte(registers.getHl(), byteValue);
            return pc + 1;
        case RES_4_A:
            res(4, registers.a);
            return pc + 1;
        case RES_5_B:
            res(5, registers.b);
            return pc + 1;
        case RES_5_C:
            res(5, registers.c);
            return pc + 1;
        case RES_5_D:
            res(5, registers.d);
            return pc + 1;
        case RES_5_E:
            res(5, registers.e);
            return pc + 1;
        case RES_5_H:
            res(5, registers.h);
            return pc + 1;
        case RES_5_L:
            res(5, registers.l);
            return pc + 1;
        case RES_5_HL:
            byteValue = bus.readByte(registers.getHl());
            res(5, byteValue);
            bus.writeByte(registers.getHl(), byteValue);
            return pc + 1;
        case RES_5_A:
            res(5, registers.a);
            return pc + 1;
        // 0xb*
        case RES_6_B:
            res(6, registers.b);
            return pc + 1;
        case RES_6_C:
            res(6, registers.c);
            return pc + 1;
        case RES_6_D:
            res(6, registers.d);
            return pc + 1;
        case RES_6_E:
            res(6, registers.e);
            return pc + 1;
        case RES_6_H:
            res(6, registers.h);
            return pc + 1;
        case RES_6_L:
            res(6, registers.l);
            return pc + 1;
        case RES_6_HL:
            byteValue = bus.readByte(registers.getHl());
            res(6, byteValue);
            bus.writeByte(registers.getHl(), byteValue);
            return pc + 1;
        case RES_6_A:
            res(6, registers.a);
            return pc + 1;
        case RES_7_B:
            res(7, registers.b);
            return pc + 1;
        case RES_7_C:
            res(7, registers.c);
            return pc + 1;
        case RES_7_D:
            res(7, registers.d);
            return pc + 1;
        case RES_7_E:
            res(7, registers.e);
            return pc + 1;
        case RES_7_H:
            res(7, registers.h);
            return pc + 1;
        case RES_7_L:
            res(7, registers.l);
            return pc + 1;
        case RES_7_HL:
            byteValue = bus.readByte(registers.getHl());
            res(7, byteValue);
            bus.writeByte(registers.getHl(), byteValue);
            return pc + 1;
        case RES_7_A:
            res(7, registers.a);
            return pc + 1;
        // 0xc*
        case SET_0_B:
            set(0, registers.b);
            return pc + 1;
        case SET_0_C:
            set(0, registers.c);
            return pc + 1;
        case SET_0_D:
            set(0, registers.d);
            return pc + 1;
        case SET_0_E:
            set(0, registers.e);
            return pc + 1;
        case SET_0_H:
            set(0, registers.h);
            return pc + 1;
        case SET_0_L:
            set(0, registers.l);
            return pc + 1;
        case SET_0_HL:
            byteValue = bus.readByte(registers.getHl());
            set(0, byteValue);
            bus.writeByte(registers.getHl(), byteValue);
            return pc + 1;
        case SET_0_A:
            set(0, registers.a);
            return pc + 1;
        case SET_1_B:
            set(1, registers.b);
            return pc + 1;
        case SET_1_C:
            set(1, registers.c);
            return pc + 1;
        case SET_1_D:
            set(1, registers.d);
            return pc + 1;
        case SET_1_E:
            set(1, registers.e);
            return pc + 1;
        case SET_1_H:
            set(1, registers.h);
            return pc + 1;
        case SET_1_L:
            set(1, registers.l);
            return pc + 1;
        case SET_1_HL:
            byteValue = bus.readByte(registers.getHl());
            set(1, byteValue);
            bus.writeByte(registers.getHl(), byteValue);
            return pc + 1;
        case SET_1_A:
            set(1, registers.a);
            return pc + 1;
        // 0xd*
        case SET_2_B:
            set(2, registers.b);
            return pc + 1;
        case SET_2_C:
            set(2, registers.c);
            return pc + 1;
        case SET_2_D:
            set(2, registers.d);
            return pc + 1;
        case SET_2_E:
            set(2, registers.e);
            return pc + 1;
        case SET_2_H:
            set(2, registers.h);
            return pc + 1;
        case SET_2_L:
            set(2, registers.l);
            return pc + 1;
        case SET_2_HL:
            byteValue = bus.readByte(registers.getHl());
            set(2, byteValue);
            bus.writeByte(registers.getHl(), byteValue);
            return pc + 1;
        case SET_2_A:
            set(2, registers.a);
            return pc + 1;
        case SET_3_B:
            set(3, registers.b);
            return pc + 1;
        case SET_3_C:
            set(3, registers.c);
            return pc + 1;
        case SET_3_D:
            set(3, registers.d);
            return pc + 1;
        case SET_3_E:
            set(3, registers.e);
            return pc + 1;
        case SET_3_H:
            set(3, registers.h);
            return pc + 1;
        case SET_3_L:
            set(3, registers.l);
            return pc + 1;
        case SET_3_HL:
            byteValue = bus.readByte(registers.getHl());
            set(3, byteValue);
            bus.writeByte(registers.getHl(), byteValue);
            return pc + 1;
        case SET_3_A:
            set(3, registers.a);
            return pc + 1;
        // 0xe*
        case SET_4_B:
            set(4, registers.b);
            return pc + 1;
        case SET_4_C:
            set(4, registers.c);
            return pc + 1;
        case SET_4_D:
            set(4, registers.d);
            return pc + 1;
        case SET_4_E:
            set(4, registers.e);
            return pc + 1;
        case SET_4_H:
            set(4, registers.h);
            return pc + 1;
        case SET_4_L:
            set(4, registers.l);
            return pc + 1;
        case SET_4_HL:
            byteValue = bus.readByte(registers.getHl());
            set(4, byteValue);
            bus.writeByte(registers.getHl(), byteValue);
            return pc + 1;
        case SET_4_A:
            set(4, registers.a);
            return pc + 1;
        case SET_5_B:
            set(5, registers.b);
            return pc + 1;
        case SET_5_C:
            set(5, registers.c);
            return pc + 1;
        case SET_5_D:
            set(5, registers.d);
            return pc + 1;
        case SET_5_E:
            set(5, registers.e);
            return pc + 1;
        case SET_5_H:
            set(5, registers.h);
            return pc + 1;
        case SET_5_L:
            set(5, registers.l);
            return pc + 1;
        case SET_5_HL:
            byteValue = bus.readByte(registers.getHl());
            set(5, byteValue);
            bus.writeByte(registers.getHl(), byteValue);
            return pc + 1;
        case SET_5_A:
            set(5, registers.a);
            return pc + 1;
        // 0xf*
        case SET_6_B:
            set(6, registers.b);
            return pc + 1;
        case SET_6_C:
            set(6, registers.c);
            return pc + 1;
        case SET_6_D:
            set(6, registers.d);
            return pc + 1;
        case SET_6_E:
            set(6, registers.e);
            return pc + 1;
        case SET_6_H:
            set(6, registers.h);
            return pc + 1;
        case SET_6_L:
            set(6, registers.l);
            return pc + 1;
        case SET_6_HL:
            byteValue = bus.readByte(registers.getHl());
            set(6, byteValue);
            bus.writeByte(registers.getHl(), byteValue);
            return pc + 1;
        case SET_6_A:
            set(6, registers.a);
            return pc + 1;
        case SET_7_B:
            set(7, registers.b);
            return pc + 1;
        case SET_7_C:
            set(7, registers.c);
            return pc + 1;
        case SET_7_D:
            set(7, registers.d);
            return pc + 1;
        case SET_7_E:
            set(7, registers.e);
            return pc + 1;
        case SET_7_H:
            set(7, registers.h);
            return pc + 1;
        case SET_7_L:
            set(7, registers.l);
            return pc + 1;
        case SET_7_HL:
            byteValue = bus.readByte(registers.getHl());
            set(7, byteValue);
            bus.writeByte(registers.getHl(), byteValue);
            return pc + 1;
        case SET_7_A:
            set(7, registers.a);
            return pc + 1;
        default:
            std::cout << "Unknown prefixed Opcode: " << (Word)opcode << "\n";
            throw 1;
            return pc;
    }
}

Byte CPU::add(const Byte& value) {
    Byte newValue = registers.a + value;
    registers.setZeroFlag(newValue == 0);
    registers.setSubtractionFlag(false);
    registers.setCarryFlag(newValue < value);
    registers.setHalfCarryFlag(newValue & HIGH_HALF_BYTE == registers.a & HIGH_HALF_BYTE);
    return newValue;
}
Word CPU::add(const Word& value) {
    Word newValue = registers.getHl() + value;
    registers.setSubtractionFlag(false);
    registers.setCarryFlag(newValue < value);
    registers.setHalfCarryFlag((registers.getHl() ^ value ^ newValue) & BYTE_SIZE != 0);
    return newValue;
}
Byte CPU::adc(const Byte& value) {
    Byte newValue = registers.a + value + (Byte)registers.isCarryFlag();
    registers.setZeroFlag(newValue == 0);
    registers.setSubtractionFlag(false);
    registers.setCarryFlag(newValue < value + (Byte)registers.isCarryFlag());
    registers.setHalfCarryFlag(newValue & HIGH_HALF_BYTE == registers.a & HIGH_HALF_BYTE);
    return newValue;
}
Word CPU::adc(const Word& value) {
    Word newValue = registers.getHl() + value + (Word)registers.isCarryFlag();
    registers.setZeroFlag(newValue == 0);
    registers.setSubtractionFlag(false);
    registers.setHalfCarryFlag((registers.getHl() ^ (value + (Word)registers.isCarryFlag()) ^ newValue) & BYTE_SIZE != 0);
    registers.setCarryFlag(newValue < value + (Word)registers.isCarryFlag());
    return newValue;
}

Byte CPU::sub(const Byte& value) {
    Byte newValue = registers.a - value;
    registers.setZeroFlag(newValue == 0);
    registers.setSubtractionFlag(true);
    registers.setCarryFlag(value > registers.a);
    registers.setHalfCarryFlag(newValue & HIGH_HALF_BYTE == registers.a & HIGH_HALF_BYTE);
    return newValue;
}
Byte CPU::sbc(const Byte& value) {
    Byte newValue = registers.a + (Byte)registers.isCarryFlag() - value;
    registers.setZeroFlag(newValue == 0);
    registers.setSubtractionFlag(true);
    registers.setCarryFlag(value > registers.a + (Byte)registers.isCarryFlag());
    registers.setHalfCarryFlag(newValue & HIGH_HALF_BYTE == registers.a & HIGH_HALF_BYTE);
    return newValue;
}

Byte CPU::logical_and(const Byte& value) {
    Byte newValue = registers.a & value;
    registers.setZeroFlag(newValue == 0);
    registers.setSubtractionFlag(false);
    registers.setHalfCarryFlag(true);
    registers.setCarryFlag(false);
    return newValue;
}
Byte CPU::logical_xor(const Byte& value) {
    Byte newValue = registers.a ^ value;
    registers.setZeroFlag(newValue == 0);
    registers.setSubtractionFlag(false);
    registers.setHalfCarryFlag(false);
    registers.setCarryFlag(false);
    return newValue;
}
Byte CPU::logical_or(const Byte& value) {
    Byte newValue = registers.a | value;
    registers.setZeroFlag(newValue == 0);
    registers.setSubtractionFlag(false);
    registers.setHalfCarryFlag(false);
    registers.setCarryFlag(false);
    return newValue;
}

void CPU::inc(Byte& value) {
    value++;
    registers.setZeroFlag(value == 0);
    registers.setSubtractionFlag(false);
    registers.setHalfCarryFlag(value & HIGH_HALF_BYTE == (value - 1) & HIGH_HALF_BYTE);
}
void CPU::dec(Byte& value) {
    value--;
    registers.setZeroFlag(value == 0);
    registers.setSubtractionFlag(true);
    registers.setHalfCarryFlag(value & HIGH_HALF_BYTE == (value + 1) & HIGH_HALF_BYTE);
}

Byte CPU::rlc(const Byte& value) {
    Byte topBit = value >> 7;
    Byte newValue = (value << 1) | topBit;
    registers.setZeroFlag(newValue == 0);
    registers.setSubtractionFlag(false);
    registers.setHalfCarryFlag(false);
    registers.setCarryFlag(topBit != 0);
    return newValue;
}
Byte CPU::rrc(const Byte& value) {
    Byte lowBit = value % 2;
    Byte newValue = (value >> 1) | (lowBit << 7);
    registers.setZeroFlag(newValue == 0);
    registers.setSubtractionFlag(false);
    registers.setHalfCarryFlag(false);
    registers.setCarryFlag(lowBit != 0);
    return newValue;
}
Byte CPU::rl(const Byte& value) {
    Byte topBit = value >> 7;
    Byte newValue = (value << 1) | (Byte)registers.isCarryFlag();
    registers.setZeroFlag(newValue == 0);
    registers.setSubtractionFlag(false);
    registers.setHalfCarryFlag(false);
    registers.setCarryFlag(topBit != 0);
    return newValue;
}
Byte CPU::rr(const Byte& value) {
    Byte lowBit = value % 2;
    Byte newValue = (value >> 1) | (((Byte)registers.isCarryFlag()) << 7);
    registers.setZeroFlag(newValue == 0);
    registers.setSubtractionFlag(false);
    registers.setHalfCarryFlag(false);
    registers.setCarryFlag(lowBit != 0);
    return newValue;
}

Byte CPU::sla(const Byte& value) {
    Byte topBit = value >> 7;
    Byte newValue = value << 1;
    registers.setZeroFlag(newValue == 0);
    registers.setSubtractionFlag(false);
    registers.setHalfCarryFlag(false);
    registers.setCarryFlag(topBit != 0);
    return newValue;
}
Byte CPU::sra(const Byte& value) {
    Byte lowBit = value % 2;
    Byte topBit = value >> 7;
    Byte newValue = (value >> 1) | (topBit << 7);
    registers.setZeroFlag(newValue == 0);
    registers.setSubtractionFlag(false);
    registers.setHalfCarryFlag(false);
    registers.setCarryFlag(lowBit != 0);
    return newValue;
}
Byte CPU::srl(const Byte& value) {
    Byte lowBit = value % 2;
    Byte newValue = (value >> 1);
    registers.setZeroFlag(newValue == 0);
    registers.setSubtractionFlag(false);
    registers.setHalfCarryFlag(false);
    registers.setCarryFlag(lowBit != 0);
    return newValue;
}

Byte CPU::swap(const Byte& value) {
    Byte topNibble = (value >> HALF_BYTE_SIZE) << HALF_BYTE_SIZE;
    Byte lowNibble = (value << HALF_BYTE_SIZE) >> HALF_BYTE_SIZE;
    Byte newValue = (topNibble >> HALF_BYTE_SIZE) | (lowNibble << HALF_BYTE_SIZE);
    registers.setZeroFlag(newValue == 0);
    registers.setSubtractionFlag(false);
    registers.setHalfCarryFlag(false);
    registers.setCarryFlag(false);
    return newValue;
}

void CPU::bit(const Byte& idx, const Byte& value) {
    Byte bit = 1 << idx;
    registers.setZeroFlag(value & bit == 0);
    registers.setSubtractionFlag(false);
    registers.setHalfCarryFlag(true);
}
void CPU::res(const Byte& idx, Byte& value) {
    Byte bit = 1 << idx;
    value = value & ~bit;
}
void CPU::set(const Byte& idx, Byte& value) {
    Byte bit = 1 << idx;
    value = value | bit;
}

Word CPU::jump(const bool& shouldJump) {
    if (!shouldJump) {
        return pc + 3;
    }
    Word least_significant_byte = (Word)bus.readByte(pc + 1);
    Word most_significant_byte = (Word)bus.readByte(pc + 2);
    return (most_significant_byte << BYTE_SIZE | least_significant_byte);
}
Word CPU::jumpRelative(const bool& shouldJump, const SignedByte& value) {
    if (!shouldJump) {
        return pc + 2;
    }
    return pc + 2 + (SignedWord)value;
}

void CPU::push(const Word& value) {
    sp -= 1;
    bus.writeByte(sp, (Byte)((value & TOP_WORD) >> BYTE_SIZE));
    sp -= 1;
    bus.writeByte(sp, (Byte)(value & FULL_BYTE));
}

Word CPU::pop() {
    Word least_significant_byte = (Word)bus.readByte(sp);
    sp += 1;
    Word most_significant_byte = (Word)bus.readByte(sp);
    sp += 1;
    return (most_significant_byte << BYTE_SIZE) | least_significant_byte;
}

Word CPU::call(const bool& shouldCall) {
    if (!shouldCall) {
        return pc + 3;
    }
    push(pc + 3);
    return bus.readWord(pc);
}

Word CPU::ret(const bool& shouldReturn) {
    if (shouldReturn) {
        return pop();
    }
    return pc + 1;
}
Word CPU::reti() {
    areInturrptsEnabled = true;
    return ret(true);
}