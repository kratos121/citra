/* This file is part of the dynarmic project.
 * Copyright (c) 2016 MerryMage
 * This software may be used and distributed according to the terms of the GNU
 * General Public License version 2 or any later version.
 */

#include <array>
#include <ostream>

#include "common/bit_util.h"
#include "frontend/arm/types.h"

namespace Dynarmic {
namespace Arm {

const char* CondToString(Cond cond, bool explicit_al) {
    constexpr std::array<const char*, 15> cond_strs = {
            "eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt", "gt", "le", "al"
    };
    return (!explicit_al && cond == Cond::AL) ? "" : cond_strs.at(static_cast<size_t>(cond));
}

const char* RegToString(Reg reg) {
    constexpr std::array<const char*, 16> reg_strs = {
        "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "sp", "lr", "pc"
    };
    return reg_strs.at(static_cast<size_t>(reg));
}

const char* ExtRegToString(ExtReg reg) {
    constexpr std::array<const char*, 64> reg_strs = {
        "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "s12", "s13", "s14", "s15",
        "s16", "s17", "s18", "s19", "s20", "s21", "s22", "s23", "s24", "s25", "s26", "s27", "s28", "s29", "s30", "s31",
        "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8", "d9", "d10", "d11", "d12", "d13", "d14", "d15",
        "d16", "d17", "d18", "d19", "d20", "d21", "d22", "d23", "d24", "d25", "d26", "d27", "d28", "d29", "d30", "d31",
    };
    return reg_strs.at(static_cast<size_t>(reg));
}

std::string RegListToString(RegList reg_list) {
    std::string ret = "";
    bool first_reg = true;
    for (size_t i = 0; i < 16; i++) {
        if (Common::Bit(i, reg_list)) {
            if (!first_reg)
                ret += ", ";
            ret += RegToString(static_cast<Reg>(i));
            first_reg = false;
        }
    }
    return ret;
}

std::ostream& operator<<(std::ostream& o, Reg reg) {
    o << RegToString(reg);
    return o;
}

std::ostream& operator<<(std::ostream& o, ExtReg reg) {
    o << ExtRegToString(reg);
    return o;
}

std::ostream& operator<<(std::ostream& o, RegList reg_list) {
    o << RegListToString(reg_list);
    return o;
}

} // namespace Arm
} // namespace Dynarmic
