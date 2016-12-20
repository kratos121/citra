/* This file is part of the dynarmic project.
 * Copyright (c) 2016 MerryMage
 * This software may be used and distributed according to the terms of the GNU
 * General Public License version 2 or any later version.
 */

#pragma once

#include <iosfwd>
#include <string>
#include <utility>

#include "common/assert.h"
#include "common/common_types.h"

namespace Dynarmic {
namespace Arm {

enum class Cond {
    EQ, NE, CS, CC, MI, PL, VS, VC, HI, LS, GE, LT, GT, LE, AL, NV
};

enum class Reg {
    R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15,
    SP = R13,
    LR = R14,
    PC = R15,
    INVALID_REG = 99
};

enum class ExtReg {
    S0, S1, S2, S3, S4, S5, S6, S7,
    S8, S9, S10, S11, S12, S13, S14, S15,
    S16, S17, S18, S19, S20, S21, S22, S23,
    S24, S25, S26, S27, S28, S29, S30, S31,
    D0, D1, D2, D3, D4, D5, D6, D7,
    D8, D9, D10, D11, D12, D13, D14, D15,
    D16, D17, D18, D19, D20, D21, D22, D23,
    D24, D25, D26, D27, D28, D29, D30, D31,
};

using Imm3 = u8;
using Imm4 = u8;
using Imm5 = u8;
using Imm7 = u8;
using Imm8 = u8;
using Imm11 = u16;
using Imm12 = u16;
using Imm24 = u32;
using RegList = u16;

enum class ShiftType {
    LSL,
    LSR,
    ASR,
    ROR ///< RRX falls under this too
};

enum class SignExtendRotation {
    ROR_0,  ///< ROR #0 or omitted
    ROR_8,  ///< ROR #8
    ROR_16, ///< ROR #16
    ROR_24  ///< ROR #24
};

const char* CondToString(Cond cond, bool explicit_al = false);
const char* RegToString(Reg reg);
const char* ExtRegToString(ExtReg reg);
std::string RegListToString(RegList reg_list);

std::ostream& operator<<(std::ostream& o, Reg reg);
std::ostream& operator<<(std::ostream& o, ExtReg reg);
std::ostream& operator<<(std::ostream& o, RegList reg_list);

constexpr bool IsSingleExtReg(ExtReg reg) {
    return reg >= ExtReg::S0 && reg <= ExtReg::S31;
}

constexpr bool IsDoubleExtReg(ExtReg reg) {
    return reg >= ExtReg::D0 && reg <= ExtReg::D31;
}

inline size_t RegNumber(Reg reg) {
    ASSERT(reg != Reg::INVALID_REG);
    return static_cast<size_t>(reg);
}

inline size_t RegNumber(ExtReg reg) {
    if (IsSingleExtReg(reg)) {
        return static_cast<size_t>(reg) - static_cast<size_t>(ExtReg::S0);
    }

    if (IsDoubleExtReg(reg)) {
        return static_cast<size_t>(reg) - static_cast<size_t>(ExtReg::D0);
    }

    ASSERT_MSG(false, "Invalid extended register");
}

inline Reg operator+(Reg reg, size_t number) {
    ASSERT(reg != Reg::INVALID_REG);

    size_t new_reg = static_cast<size_t>(reg) + number;
    ASSERT(new_reg <= 15);

    return static_cast<Reg>(new_reg);
}

inline ExtReg operator+(ExtReg reg, size_t number) {
    ExtReg new_reg = static_cast<ExtReg>(static_cast<size_t>(reg) + number);

    ASSERT((IsSingleExtReg(reg) && IsSingleExtReg(new_reg)) ||
           (IsDoubleExtReg(reg) && IsDoubleExtReg(new_reg)));

    return new_reg;
}

} // namespace Arm
} // namespace Dynarmic
