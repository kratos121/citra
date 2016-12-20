/* This file is part of the dynarmic project.
 * Copyright (c) 2016 MerryMage
 * This software may be used and distributed according to the terms of the GNU
 * General Public License version 2 or any later version.
 */
#pragma once

#include <xbyak.h>

#include "backend_x64/jitstate.h"
#include "common/assert.h"
#include "common/common_types.h"

namespace Dynarmic {
namespace BackendX64 {

enum class HostLoc {
    // Ordering of the registers is intentional. See also: HostLocToX64.
    RAX, RCX, RDX, RBX, RSP, RBP, RSI, RDI, R8, R9, R10, R11, R12, R13, R14, R15,
    XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7,
    XMM8, XMM9, XMM10, XMM11, XMM12, XMM13, XMM14, XMM15,
    CF, PF, AF, ZF, SF, OF,
    FirstSpill,
};

constexpr size_t HostLocCount = static_cast<size_t>(HostLoc::FirstSpill) + SpillCount;

inline bool HostLocIsGPR(HostLoc reg) {
    return reg >= HostLoc::RAX && reg <= HostLoc::R15;
}

inline bool HostLocIsXMM(HostLoc reg) {
    return reg >= HostLoc::XMM0 && reg <= HostLoc::XMM15;
}

inline bool HostLocIsRegister(HostLoc reg) {
    return HostLocIsGPR(reg) || HostLocIsXMM(reg);
}

inline bool HostLocIsFlag(HostLoc reg) {
    return reg >= HostLoc::CF && reg <= HostLoc::OF;
}

inline HostLoc HostLocSpill(size_t i) {
    ASSERT_MSG(i < SpillCount, "Invalid spill");
    return static_cast<HostLoc>(static_cast<int>(HostLoc::FirstSpill) + i);
}

inline bool HostLocIsSpill(HostLoc reg) {
    return reg >= HostLoc::FirstSpill && reg <= HostLocSpill(SpillCount - 1);
}

using HostLocList = std::initializer_list<HostLoc>;

// RSP is preserved for function calls
// R15 contains the JitState pointer
const HostLocList any_gpr = {
    HostLoc::RAX,
    HostLoc::RBX,
    HostLoc::RCX,
    HostLoc::RDX,
    HostLoc::RSI,
    HostLoc::RDI,
    HostLoc::RBP,
    HostLoc::R8,
    HostLoc::R9,
    HostLoc::R10,
    HostLoc::R11,
    HostLoc::R12,
    HostLoc::R13,
    HostLoc::R14,
};

const HostLocList any_xmm = {
    HostLoc::XMM0,
    HostLoc::XMM1,
    HostLoc::XMM2,
    HostLoc::XMM3,
    HostLoc::XMM4,
    HostLoc::XMM5,
    HostLoc::XMM6,
    HostLoc::XMM7,
    HostLoc::XMM8,
    HostLoc::XMM9,
    HostLoc::XMM10,
    HostLoc::XMM11,
    HostLoc::XMM12,
    HostLoc::XMM13,
    HostLoc::XMM14,
    HostLoc::XMM15,
};

Xbyak::Reg64 HostLocToReg64(HostLoc loc);
Xbyak::Xmm HostLocToXmm(HostLoc loc);
Xbyak::Address SpillToOpArg(HostLoc loc);

} // namespace BackendX64
} // namespace Dynarmic
