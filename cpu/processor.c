#include <stdio.h>
#include <stdint.h>

#include "cpu.h"

void PointerStep(struct CPU* cpu, uint8_t memory[0x10000], uint8_t pointer, int incdec, int display) {

    switch(pointer) {
        case SP: {
            cpu->SP += incdec;
            break;
        }
        case PC: {
            cpu->PC += incdec;
            break;
        }
    }
    if(display) PrintState(cpu, memory);
}

/*
    In a real processor, the ALU or Arithmetic and Logic Unit performs arithmetic operations on given inputs
    For the sake of simplicity, only addition and subtraction has been implemented. 
    The result of these operations
    set the various processor flags, excluding the E (Equals) flag.
*/

struct Instruction Decode(uint8_t opcode) {
    struct Instruction instruction;

    instruction.ins = opcode >> 4;
    instruction.mode = ReadBit(opcode, 3);
    instruction.reg = ReadSubByte(opcode, 2, 0);

    return instruction;
}

void ALU(struct CPU* cpu, uint8_t memory[0x10000], int val) {
    struct Instruction instruction = Decode(cpu->IR);

    int reg_val = ReadReg(cpu, instruction.reg);

    switch(instruction.ins) {
        case ADD:
            reg_val += val;
            if(reg_val == 0) cpu->Z = 1;
            if(ReadBit(reg_val,7) == 0 && ReadBit(ReadReg(cpu, instruction.reg),7) == 1) {cpu->C = 1; cpu->V = 1;}
            break;
        case SUB:
            reg_val -= val;
            if(reg_val == 0) cpu->Z = 1;
            if(reg_val > ReadReg(cpu, instruction.reg)) cpu->V = 1;
            break;
        case AND:
            reg_val = reg_val & val;
            break;
        case OR:
            reg_val = reg_val | val;
            break;
        case NOR:
            reg_val = ~(reg_val | val);
            break;
    }
    WriteReg(cpu, instruction.reg, reg_val);
}
void ExecuteInstruction(struct CPU* cpu, uint8_t memory[0x10000]) {
    
    /*
        The Execution of an Instruction is split into three stages, those being:
        
        Fetch: The Opcode of the Instruction to be executed is read from the address pointed to by 
        the PC and is loaded into the instruction register where it is saved
        
        Decode: The instruction is read from the opcode, as is the instruction mode and register data.
        
        Execute: The CPU executes the instruction after reading it.
    */
    
    // Fetch The Instruction to be read is saved to the Buffer Register and then is copied into the Instruction Register
    cpu->IR = memory[cpu->PC];

    // Decode
    struct Instruction instruction = Decode(cpu->IR);

    // Execute
    cpu->PC++;

    int val;
    if(instruction.mode == 1) { val = ReadReg(cpu, memory[cpu->PC]); }
    switch(instruction.ins) {
        case ADD: {
            if(instruction.mode == 0) { val = ReadAddress(cpu, memory); }
            ALU(cpu, memory, val);
            break;
        }
        case SUB: {
            if(instruction.mode == 0) { val = ReadAddress(cpu, memory); }
            ALU(cpu, memory, val);
            break;
        }
        case AND: {
            if(instruction.mode == 0) { val = ReadAddress(cpu, memory); }
            ALU(cpu, memory, val);
            break;
        }
        case OR: {
            if(instruction.mode == 0) { val = ReadAddress(cpu, memory); }
            ALU(cpu, memory, val);
            break;
        }
        case NOR: {
            if(instruction.mode == 0) { val = ReadAddress(cpu, memory); }
            ALU(cpu, memory, val);
            break;
        }
        case CLF: {
            switch(instruction.reg) {
                case FGZ:
                    cpu->Z = 0;
                    break;
                case FGE:
                    cpu->E = 0;
                    break;
                case FGC:
                    cpu->C = 0;
                    break;
                case FGV:
                    cpu->V = 0;
                    break;
            }
            break;
        }
        case CMP: {
            if(instruction.mode == 0) { val = ReadAddress(cpu, memory); }
            
            if(ReadReg(cpu, instruction.reg) < val)  {cpu->Z = 1; cpu->E = 0; cpu->C = 0;}
            if(ReadReg(cpu, instruction.reg) == val) {cpu->Z = 0; cpu->E = 1; cpu->C = 0;}
            if(ReadReg(cpu, instruction.reg) > val)  {cpu->Z = 0; cpu->E = 0; cpu->C = 1;}
            break;
        }
        case PSH: {
            cpu->SP--;
            switch(instruction.reg) {
                case IDX:
                    memory[cpu->SP] = cpu->IDX;
                    break;
                case IDY:
                    memory[cpu->SP] = cpu->IDY;
                    break;
                case IDZ:
                    memory[cpu->SP] = cpu->IDZ;
                    break;
            }
            break;
        }
        case POP: {
            switch(instruction.reg) {
                case IDX:
                    cpu->IDX = memory[cpu->SP];
                    break;
                case IDY:
                    cpu->IDY = memory[cpu->SP];
                    break;
                case IDZ:
                    cpu->IDZ = memory[cpu->SP];
                    break;
            }
            cpu->SP++;
            break;
        }
        case MOV: {
            if(instruction.mode == 0) { val = ReadAddress(cpu, memory); }
            switch(instruction.reg) {
                case IDX:
                    cpu->IDX = val;
                    break;
                case IDY:
                    cpu->IDY = val;
                    break;
                case IDZ:
                    cpu->IDZ = val;
                    break;
            }
            break;
        }
        case STO: {
            int adr_low = memory[cpu->PC];
            cpu->PC++;
            int adr_high = memory[cpu->PC];
            int adr_full = (adr_high << 8) + adr_low;
            switch(instruction.reg) {
                case IDX:
                    memory[adr_full] = cpu->IDX;
                    break;
                case IDY:
                    memory[adr_full] = cpu->IDY;
                    break;
                case IDZ:
                    memory[adr_full] = cpu->IDZ;
                    break;
            }
            break;
        }
        case JMP: {
            int adr_low = memory[cpu->PC];
            cpu->PC++;
            int adr_high = memory[cpu->PC];
            int adr_full = (adr_high << 8) + adr_low;
            cpu->PC = adr_full;
            cpu->PC--;
            break;
        }
        case JFS: {
            if(ReadReg(cpu, instruction.reg) == 1) {
                int adr_low = memory[cpu->PC];
                cpu->PC++;
                int adr_high = memory[cpu->PC];
                int adr_full = (adr_high << 8) + adr_low;
                cpu->PC = adr_full;
                cpu->PC--;
                break;
            }
            cpu->PC++;
            break;
        }
        case JSR: {
            cpu->SP--;
            memory[cpu->SP] = (cpu->PC+1) >> 8;
            cpu->SP--;
            memory[cpu->SP] = ReadSubByte((cpu->PC+1), 3, 0);
            int adr_low = memory[cpu->PC];
            cpu->PC++;
            int adr_high = memory[cpu->PC];
            int adr_full = (adr_high << 8) + adr_low;
            cpu->PC = adr_full;
            cpu->PC--;
            break;
        }
        case RTS: {
            int return_low = memory[cpu->SP];
            cpu->SP++;
            int return_high = memory[cpu->SP];
            cpu->SP++;
            int return_adr = (return_high << 8) + return_low;
            cpu->PC = return_adr;
            break;
        }
        case NOP: {
            if(instruction.mode == 0b1 && instruction.reg == 0b111) {
                printf("\nCPU Halted");
                cpu->H = 1;
            }
            break;
        }
        default: {
            printf("\nERROR: Invalid Instruction at %04X", cpu->PC);
            cpu->H = 1;
            return;
        }
    }
}

void ProcessorInit(struct CPU* cpu, uint8_t memory[0x10000]) {
    cpu->PC  = 0xFFF0;
    cpu->SP  = 0x00;

    cpu->IDX = 0x00;
    cpu->IDY = 0x00;
    cpu->IDZ = 0x00;

    cpu->Z   = 0;
    cpu->E   = 0;
    cpu->C   = 0;
    cpu->V   = 0;

    cpu->H   = 0;
}

void ProcessorLoop(struct CPU* cpu, uint8_t memory[0x10000]) {
    while(!cpu->H) {
        PrintState(cpu, memory);
        ExecuteInstruction(cpu, memory);
        cpu->PC++;
    }
}