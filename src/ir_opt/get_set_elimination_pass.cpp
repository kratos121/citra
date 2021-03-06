/* This file is part of the dynarmic project.
 * Copyright (c) 2016 MerryMage
 * This software may be used and distributed according to the terms of the GNU
 * General Public License version 2 or any later version.
 */

#include <array>

#include "common/assert.h"
#include "common/common_types.h"
#include "frontend/ir/basic_block.h"
#include "frontend/ir/value.h"
#include "ir_opt/passes.h"

namespace Dynarmic {
namespace Optimization {

void GetSetElimination(IR::Block& block) {
    using Iterator = IR::Block::iterator;
    struct RegisterInfo {
        IR::Value register_value;
        bool set_instruction_present = false;
        Iterator last_set_instruction;
    };
    std::array<RegisterInfo, 15> reg_info;
    std::array<RegisterInfo, 32> ext_reg_singles_info;
    std::array<RegisterInfo, 32> ext_reg_doubles_info;
    struct CpsrInfo {
        RegisterInfo n;
        RegisterInfo z;
        RegisterInfo c;
        RegisterInfo v;
        RegisterInfo ge;
    } cpsr_info;

    const auto do_set = [&block](RegisterInfo& info, IR::Value value, Iterator set_inst) {
        if (info.set_instruction_present) {
            info.last_set_instruction->Invalidate();
            block.Instructions().erase(info.last_set_instruction);
        }

        info.register_value = value;
        info.set_instruction_present = true;
        info.last_set_instruction = set_inst;
    };

    const auto do_get = [](RegisterInfo& info, Iterator get_inst) {
        if (info.register_value.IsEmpty()) {
            info.register_value = IR::Value(&*get_inst);
            return;
        }
        get_inst->ReplaceUsesWith(info.register_value);
    };

    for (auto inst = block.begin(); inst != block.end(); ++inst) {
        switch (inst->GetOpcode()) {
        case IR::Opcode::SetRegister: {
            Arm::Reg reg = inst->GetArg(0).GetRegRef();
            if (reg == Arm::Reg::PC)
                break;
            size_t reg_index = static_cast<size_t>(reg);
            do_set(reg_info[reg_index], inst->GetArg(1), inst);
            break;
        }
        case IR::Opcode::GetRegister: {
            Arm::Reg reg = inst->GetArg(0).GetRegRef();
            ASSERT(reg != Arm::Reg::PC);
            size_t reg_index = static_cast<size_t>(reg);
            do_get(reg_info[reg_index], inst);
            break;
        }
        case IR::Opcode::SetExtendedRegister32: {
            Arm::ExtReg reg = inst->GetArg(0).GetExtRegRef();
            size_t reg_index = Arm::RegNumber(reg);
            do_set(ext_reg_singles_info[reg_index], inst->GetArg(1), inst);

            size_t doubles_reg_index = reg_index / 2;
            if (doubles_reg_index < ext_reg_doubles_info.size()) {
                ext_reg_doubles_info[doubles_reg_index] = {};
            }
            break;
        }
        case IR::Opcode::GetExtendedRegister32: {
            Arm::ExtReg reg = inst->GetArg(0).GetExtRegRef();
            size_t reg_index = Arm::RegNumber(reg);
            do_get(ext_reg_singles_info[reg_index], inst);

            size_t doubles_reg_index = reg_index / 2;
            if (doubles_reg_index < ext_reg_doubles_info.size()) {
                ext_reg_doubles_info[doubles_reg_index] = {};
            }
            break;
        }
        case IR::Opcode::SetExtendedRegister64: {
            Arm::ExtReg reg = inst->GetArg(0).GetExtRegRef();
            size_t reg_index = Arm::RegNumber(reg);
            do_set(ext_reg_doubles_info[reg_index], inst->GetArg(1), inst);

            size_t singles_reg_index = reg_index * 2;
            if (singles_reg_index < ext_reg_singles_info.size()) {
                ext_reg_singles_info[singles_reg_index] = {};
                ext_reg_singles_info[singles_reg_index+1] = {};
            }
            break;
        }
        case IR::Opcode::GetExtendedRegister64: {
            Arm::ExtReg reg = inst->GetArg(0).GetExtRegRef();
            size_t reg_index = Arm::RegNumber(reg);
            do_get(ext_reg_doubles_info[reg_index], inst);

            size_t singles_reg_index = reg_index * 2;
            if (singles_reg_index < ext_reg_singles_info.size()) {
                ext_reg_singles_info[singles_reg_index] = {};
                ext_reg_singles_info[singles_reg_index+1] = {};
            }
            break;
        }
        case IR::Opcode::SetNFlag: {
            do_set(cpsr_info.n, inst->GetArg(0), inst);
            break;
        }
        case IR::Opcode::GetNFlag: {
            do_get(cpsr_info.n, inst);
            break;
        }
        case IR::Opcode::SetZFlag: {
            do_set(cpsr_info.z, inst->GetArg(0), inst);
            break;
        }
        case IR::Opcode::GetZFlag: {
            do_get(cpsr_info.z, inst);
            break;
        }
        case IR::Opcode::SetCFlag: {
            do_set(cpsr_info.c, inst->GetArg(0), inst);
            break;
        }
        case IR::Opcode::GetCFlag: {
            do_get(cpsr_info.c, inst);
            break;
        }
        case IR::Opcode::SetVFlag: {
            do_set(cpsr_info.v, inst->GetArg(0), inst);
            break;
        }
        case IR::Opcode::GetVFlag: {
            do_get(cpsr_info.v, inst);
            break;
        }
        case IR::Opcode::SetGEFlags: {
            do_set(cpsr_info.ge, inst->GetArg(0), inst);
            break;
        }
        case IR::Opcode::GetGEFlags: {
            do_get(cpsr_info.ge, inst);
            break;
        }
        default: {
            if (inst->ReadsFromCPSR() || inst->WritesToCPSR()) {
                cpsr_info = {};
            }
            break;
        }
        }
    }
}

} // namespace Optimization
} // namespace Dynarmic
