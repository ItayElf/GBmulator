#include "cpu.h"

#include <iostream>

#include "instructions.h"

#define WORD_OFF (0)
#define HIGH_HALF_BYTE (0xf0)
#define LDH_START_ADDRESS (0xff00)

CPU::CPU() {
    registers = Registers();
    pc = WORD_OFF;
    sp = WORD_OFF;
    bus = MemoryBus();
    isHalted = false;
    areInturrptsEnabled = true;
}

void CPU::step() {
    Byte opcode = bus.readByte(pc);
    if (opcode == PREFIX_BYTE) {
        opcode = bus.readByte(pc + 1);
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
        case SUB_A_B:
            registers.a = sub(registers.b);
            return pc + 1;
        case SUB_A_C:
            registers.a = sub(registers.c);
            return pc + 1;
        case SUB_A_D:
            registers.a = sub(registers.d);
            return pc + 1;
        case SUB_A_E:
            registers.a = sub(registers.e);
            return pc + 1;
        case SUB_A_H:
            registers.a = sub(registers.h);
            return pc + 1;
        case SUB_A_L:
            registers.a = sub(registers.l);
            return pc + 1;
        case SUB_A_HL:
            registers.a = sub(bus.readByte(registers.getHl()));
            return pc + 1;
        case SUB_A_A:
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
        case AND_A_B:
            registers.a = logical_and(registers.b);
            return pc + 1;
        case AND_A_C:
            registers.a = logical_and(registers.c);
            return pc + 1;
        case AND_A_D:
            registers.a = logical_and(registers.d);
            return pc + 1;
        case AND_A_E:
            registers.a = logical_and(registers.e);
            return pc + 1;
        case AND_A_H:
            registers.a = logical_and(registers.h);
            return pc + 1;
        case AND_A_L:
            registers.a = logical_and(registers.l);
            return pc + 1;
        case AND_A_HL:
            registers.a = logical_and(bus.readByte(registers.getHl()));
            return pc + 1;
        case AND_A_A:
            registers.a = logical_and(registers.a);
            return pc + 1;
        case XOR_A_B:
            registers.a = logical_xor(registers.b);
            return pc + 1;
        case XOR_A_C:
            registers.a = logical_xor(registers.c);
            return pc + 1;
        case XOR_A_D:
            registers.a = logical_xor(registers.d);
            return pc + 1;
        case XOR_A_E:
            registers.a = logical_xor(registers.e);
            return pc + 1;
        case XOR_A_H:
            registers.a = logical_xor(registers.h);
            return pc + 1;
        case XOR_A_L:
            registers.a = logical_xor(registers.l);
            return pc + 1;
        case XOR_A_HL:
            registers.a = logical_xor(bus.readByte(registers.getHl()));
            return pc + 1;
        case XOR_A_A:
            registers.a = logical_xor(registers.a);
            return pc + 1;
        // 0xb*
        case OR_A_B:
            registers.a = logical_or(registers.b);
            return pc + 1;
        case OR_A_C:
            registers.a = logical_or(registers.c);
            return pc + 1;
        case OR_A_D:
            registers.a = logical_or(registers.d);
            return pc + 1;
        case OR_A_E:
            registers.a = logical_or(registers.e);
            return pc + 1;
        case OR_A_H:
            registers.a = logical_or(registers.h);
            return pc + 1;
        case OR_A_L:
            registers.a = logical_or(registers.l);
            return pc + 1;
        case OR_A_HL:
            registers.a = logical_or(bus.readByte(registers.getHl()));
            return pc + 1;
        case OR_A_A:
            registers.a = logical_or(registers.a);
            return pc + 1;
        case CP_A_B:
            sub(registers.b);
            return pc + 1;
        case CP_A_C:
            sub(registers.c);
            return pc + 1;
        case CP_A_D:
            sub(registers.d);
            return pc + 1;
        case CP_A_E:
            sub(registers.e);
            return pc + 1;
        case CP_A_H:
            sub(registers.h);
            return pc + 1;
        case CP_A_L:
            sub(registers.l);
            return pc + 1;
        case CP_A_HL:
            sub(bus.readByte(registers.getHl()));
            return pc + 1;
        case CP_A_A:
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
        case LC_C_ADDR_A:
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
            registers.setHalfCarryFlag((sp ^ signedValue ^ wordValue) & 8 != 0);
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
            registers.setHalfCarryFlag((sp ^ signedValue ^ wordValue) & 8 != 0);
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
            std::cout << "Unknown Opcode: " << opcode << "\n";
            break;
            return pc;
    }
}

Word CPU::executePrefixed(const Byte& opcode) {
    if (isHalted) {
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
    registers.setHalfCarryFlag((registers.getHl() ^ value ^ newValue) & 8 != 0);
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
    registers.setHalfCarryFlag((registers.getHl() ^ (value + (Word)registers.isCarryFlag()) ^ newValue) & 8 != 0);
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
    Byte topBit = topBit >> 7;
    Byte newValue = (value << 1) | topBit;
    registers.setCarryFlag(topBit != 0);
    return newValue;
}

Byte CPU::rrc(const Byte& value) {
    Byte lowBit = value % 2;
    Byte newValue = (value >> 1) | (lowBit << 7);
    registers.setCarryFlag(lowBit != 0);
    return newValue;
}
Byte CPU::rl(const Byte& value) {
    Byte topBit = topBit >> 7;
    Byte newValue = (value << 1) | (Byte)registers.isCarryFlag();
    registers.setCarryFlag(topBit != 0);
    return newValue;
}
Byte CPU::rr(const Byte& value) {
    Byte lowBit = value % 2;
    Byte newValue = (value >> 1) | (((Byte)registers.isCarryFlag()) << 7);
    registers.setCarryFlag(lowBit != 0);
    return newValue;
}

Word CPU::jump(const bool& shouldJump) {
    if (!shouldJump) {
        return pc + 3;
    }
    Word least_significant_byte = (Word)bus.readByte(pc + 1);
    Word most_significant_byte = (Word)bus.readByte(pc + 2);
    return (most_significant_byte << 8 | least_significant_byte);
}
Word CPU::jumpRelative(const bool& shouldJump, const SignedByte& value) {
    if (!shouldJump) {
        return pc + 2;
    }
    return pc + (SignedWord)value;
}

void CPU::push(const Word& value) {
    sp -= 1;
    bus.writeByte(sp, (Byte)((value & TOP_WORD) >> 8));
    sp -= 1;
    bus.writeByte(sp, (Byte)(value & FULL_BYTE));
}

Word CPU::pop() {
    Word least_significant_byte = (Word)bus.readByte(sp);
    sp += 1;
    Word most_significant_byte = (Word)bus.readByte(sp);
    sp += 1;
    return (most_significant_byte << 8) | least_significant_byte;
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