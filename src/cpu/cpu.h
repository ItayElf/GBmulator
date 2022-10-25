#pragma once

#include "../common.h"
#include "memory_bus.h"
#include "registers.h"

/**
 * @brief a struct that represents the cpu of the emulator
 */
struct CPU {
    Registers registers;
    Word pc;
    Word sp;
    MemoryBus bus;
    bool isHalted;
    bool areInturrptsEnabled;

    /**
     * @brief default init for the cpu
     */
    CPU();

    /**
     * @brief steps the pc and execute the next opcode
     */
    void step();

    /**
     * @brief executes the given opcode
     * @param opcode the opcode for the instruction to execute
     * @return the next pc
     */
    Word execute(const Byte& opcode);

    /**
     * @brief executes the given opcode for a prefixed instruction
     * @param opcode the prefixed opcode for the instruction to execute
     * @return the next pc
     */
    Word executePrefixed(const Byte& opcode);

    /**
     * @brief adds the given value to the a register (ADD_A_* opcodes)
     * @param value the value to add to the a register
     * @return the result of the addition
     */
    Byte add(const Byte& value);
    /**
     * @brief adds the given value to the hl register (ADD_HL_* opcodes)
     * @param value the value to add to the a target
     * @return the result of the addition
     */
    Word add(const Word& value);
    /**
     * @brief adds the given value to the a register and the value of the carry flag (ADC_A_* opcodes)
     * @param value the value to add to the a register
     * @return the result of the addition
     */
    Byte adc(const Byte& value);
    /**
     * @brief adds the given value to the hl register and the carry flag (ADC_HL_* opcodes)
     * @param value the value to add to the a target
     * @return the result of the addition
     */
    Word adc(const Word& value);

    /**
     * @brief subtructs the given value from the a register
     * @param value the value to subtruct from the a register
     * @return the result of the subtraction
     */
    Byte sub(const Byte& value);
    /**
     * @brief subtructs the given value from the sum of the a register and the carry flag
     * @param value the value to subtruct from the a register
     * @return the result of the subtraction
     */
    Byte sbc(const Byte& value);

    /**
     * @brief logically AND the value with the a register and returns the result
     * @param value the value to AND with the a register
     * @return the result of the operation
     */
    Byte logical_and(const Byte& value);
    /**
     * @brief logically XOR the value with the a register and returns the result
     * @param value the value to XOR with the a register
     * @return the result of the operation
     */
    Byte logical_xor(const Byte& value);
    /**
     * @brief logically OR the value with the a register and returns the result
     * @param value the value to OR with the a register
     * @return the result of the operation
     */
    Byte logical_or(const Byte& value);

    /**
     * @brief increase the value by 1 and sets the corresponding flags.
     * @param value the value to increase, usually a register.
     */
    void inc(Byte& value);
    /**
     * @brief decreases the value by 1 and sets the corresponding flags.
     * @param value the value to decrease, usually a register.
     */
    void dec(Byte& value);

    /**
     * @brief rotates a value to the left not through the carry flag
     * @param value the value to rotate
     * @return the new value
     */
    Byte rlc(const Byte& value);
    /**
     * @brief rotates a value to the right not through the carry flag
     * @param value the value to rotate
     * @return the new value
     */
    Byte rrc(const Byte& value);
    /**
     * @brief rotates a value to the left through the carry flag
     * @param value the value to rotate
     * @return the new value
     */
    Byte rl(const Byte& value);
    /**
     * @brief rotates a value to the right through the carry flag
     * @param value the value to rotate
     * @return the new value
     */
    Byte rr(const Byte& value);

    /**
     * @brief shifts left a value to the right into the carry flag
     * @param value the value to shift
     * @return the new value
     */
    Byte sla(const Byte& value);
    /**
     * @brief arithmatically shifts right a value to the right into the carry flag
     * @param value the value to shift
     * @return the new value
     */
    Byte sra(const Byte& value);
    /**
     * @brief logially shifts right a value to the right into the carry flag
     * @param value the value to shift
     * @return the new value
     */
    Byte srl(const Byte& value);

    /**
     * @brief switches the lower and upper nibbles
     * @param value the value to switch
     * @return the new value
     */
    Byte swap(const Byte& value);

    /**
     * @brief tests a bit inside a byte
     * @param idx the bit to test
     * @param value the value to test
     */
    void bit(const Byte& idx, const Byte& value);
    /**
     * @brief turn off a bit inside a byte
     * @param idx the bit to turn off
     * @param value the value to turn off from
     */
    void res(const Byte& idx, Byte& value);
    /**
     * @brief turn on a bit inside a byte
     * @param idx the bit to turn on
     * @param value the value to turn on from
     */
    void set(const Byte& idx, Byte& value);

    /**
     * @brief returns the address for the next instruction if should jump
     * @param shouldJump tells if the jump should happen or not
     * @return the next pc value
     */
    Word jump(const bool& shouldJump);
    /**
     * @brief returns the address for the next instruction if should jump
     * @param shouldJump tells if the jump should happen or not
     * @param value how much to increase/decrease the pc
     * @return the next pc value
     */
    Word jumpRelative(const bool& shouldJump, const SignedByte& value);

    /**
     * @brief pushes a value to the stack
     * @param value the value to push to the stack
     */
    void push(const Word& value);
    /**
     * @brief pops a value from the stack
     * @return the value on top of the stack
     */
    Word pop();

    /**
     * @brief calls a function if the condition is true
     * @param shouldCall wheather the function should be called or not
     * @return the next pc
     */
    Word call(const bool& shouldCall);
    /**
     * @brief returns based on the given condition
     * @param shouldReturn wheather the function should return or not
     * @return the next pc
     */
    Word ret(const bool& shouldReturn);
    /**
     * @brief returns and enables interrupts
     * @return the next pc
     */
    Word reti();
};