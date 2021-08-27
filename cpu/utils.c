#include <stdio.h>
#include <stdint.h>

#include "cpu.h"

char* InsAsString(struct Instruction instruction) {
    char* INS;
    switch(instruction.ins) {
        case ADD:
            INS = "ADD";
            break;
        case SUB: 
            INS = "SUB";
            break;
        case AND: 
            INS = "AND";
            break;
        case OR: 
            INS = "OR";
            break;
        case NOR:
            INS = "NOR";
            break;
        case CLF:
            INS = "CLF";
            break;
        case CMP: 
            INS = "CMP";
            break;
        case PSH: 
            INS = "PSH";
            break;
        case POP: 
            INS = "POP";
            break;
        case MOV: 
            INS = "MOV";
            break;
        case STO: 
            INS = "STO";
            break;
        case JMP: 
            INS = "JMP";
            break;
        case JFS: 
            INS = "JFS";
            break;
        case JSR: 
            INS = "JSR";
            break;
        case RTS: 
            INS = "RTS";
            break;
        case NOP:
            if(instruction.mode == 1 && instruction.reg == 0b111) {
                INS = "HLT";
                break;
            }
            INS = "NOP";
            break;
    }
    return INS;
}

void PrintState(struct CPU* cpu, uint8_t memory[0x10000]) {
    // Print Address of the Program Counter and Stack Pointer
    printf("MEM PTRS: PC: %04X | ", cpu->PC);
    printf("SP: %02X  ", cpu->SP);

    // Print the instruction being performed
    struct Instruction instruction = Decode(cpu->IR);
    printf("INSTRUCTION: OP: %02X | ", cpu->IR);

    char* INS = InsAsString(instruction);
    printf("INS: %-3s | ", INS);
    
    char *mode = "ADR";
    if(instruction.mode == 1) mode = "REG";

    printf("MODE: %s | ", mode);
    printf("REG: %01X  ", instruction.reg);

    //Print the values of the various data registers
    printf("REGISTERS: ");
    printf("IDX=%02X  IDY=%02X  IDZ=%02X  ", cpu->IDX, cpu->IDY, cpu->IDZ);

    //Print the state of the various flags
    printf("FLAGS: Z=%01X  E=%01X  C=%01X  V=%01X\n\n", cpu->Z, cpu->E, cpu->C, cpu->V);
}
