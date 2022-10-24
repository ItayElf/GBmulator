#pragma once

#include "../common.h"

/**
 * @brief a struct that holds all the registers of the gb cpu.
 */
struct Registers {
    Byte a;
    Byte b;
    Byte c;
    Byte d;
    Byte e;
    Byte f;
    Byte h;
    Byte l;

    /**
     * @brief default init for the registers
     */
    Registers();

    /**
     * @brief returns the value of the artificial af register
     */
    Word getAf() const;
    /**
     * @brief returns the value of the artificial bc register
     */
    Word getBc() const;
    /**
     * @brief returns the value of the artificial de register
     */
    Word getDe() const;
    /**
     * @brief returns the value of the artificial hl register
     */
    Word getHl() const;

    /**
     * @brief returns whether the zero flag is on or not
     */
    bool isZeroFlag() const;
    /**
     * @brief returns whether the subtraction flag is on or not
     */
    bool isSubtractionFlag() const;
    /**
     * @brief returns whether the half-carry flag is on or not
     */
    bool isHalfCarryFlag() const;
    /**
     * @brief returns whether the carry flag is on or not
     */
    bool isCarryFlag() const;

    /**
     * @brief sets the artificial register af to a given value
     * @param v the value to set the register to
     */
    void setAf(const Word& v);
    /**
     * @brief sets the artificial register bc to a given value
     * @param v the value to set the register to
     */
    void setBc(const Word& v);
    /**
     * @brief sets the artificial register de to a given value
     * @param v the value to set the register to
     */
    void setDe(const Word& v);
    /**
     * @brief sets the artificial register hl to a given value
     * @param v the value to set the register to
     */
    void setHl(const Word& v);

    /**
     * @brief sets the zero flag
     * @param value if the flag should be on or off
     */
    void setZeroFlag(const bool& value);
    /**
     * @brief sets the subtraction flag
     * @param value if the flag should be on or off
     */
    void setSubtractionFlag(const bool& value);
    /**
     * @brief sets the half-carry flag
     * @param value if the flag should be on or off
     */
    void setHalfCarryFlag(const bool& value);
    /**
     * @brief sets the carry flag
     * @param value if the flag should be on or off
     */
    void setCarryFlag(const bool& value);
};