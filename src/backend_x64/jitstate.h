/* This file is part of the dynarmic project.
 * Copyright (c) 2016 MerryMage
 * This software may be used and distributed according to the terms of the GNU
 * General Public License version 2 or any later version.
 */

#pragma once

#include <array>

#include "common/common_types.h"

namespace Dynarmic {
namespace BackendX64 {

class BlockOfCode;

constexpr size_t SpillCount = 64;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4324) // Structure was padded due to alignment specifier
#endif

struct JitState {
    JitState() { ResetRSB(); }

    u32 Cpsr = 0;
    std::array<u32, 16> Reg{}; // Current register file.
    // TODO: Mode-specific register sets unimplemented.

    alignas(u64) std::array<u32, 64> ExtReg{}; // Extension registers.

    std::array<u64, SpillCount> Spill{}; // Spill.

    // For internal use (See: BlockOfCode::RunCode)
    u32 guest_MXCSR = 0x00001f80;
    u32 save_host_MXCSR = 0;
    s64 cycles_remaining = 0;
    bool halt_requested = false;

    // Exclusive state
    static constexpr u32 RESERVATION_GRANULE_MASK = 0xFFFFFFF8;
    u32 exclusive_state = 0;
    u32 exclusive_address = 0;

    static constexpr size_t RSBSize = 8; // MUST be a power of 2.
    u32 rsb_ptr = 0;
    std::array<u64, RSBSize> rsb_location_descriptors;
    std::array<u64, RSBSize> rsb_codeptrs;
    void ResetRSB();

    u32 FPSCR_IDC = 0;
    u32 FPSCR_UFC = 0;
    u32 FPSCR_mode = 0;
    u32 FPSCR_nzcv = 0;
    u32 old_FPSCR = 0;
    u32 Fpscr() const;
    void SetFpscr(u32 FPSCR);
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

using CodePtr = const void*;

} // namespace BackendX64
} // namespace Dynarmic
