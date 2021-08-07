#include <stdio.h>
#include <stdint.h>

#include "cpu.h"

/*
    ReadBit and ReadSubByte are separate from the other Read/Write instructions in that
    they don't directly access memory or the registers.
    Readbit reads a bit at a given position in a byte, and ReadSubByte
*/

char* InsAsString(uint8_t instruction) {
    char* INS;
    switch(instruction) {
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
            INS = "NOP";
            break;
    }
    return INS;
}

void PrintState(struct CPU* cpu, uint8_t memory[0x10000]) {
    // Print Address of the Program Counter and Stack Pointer
    printf("PC: %04X | ", cpu->PC);
    printf("SP: %02X | ", cpu->SP);

    // Print the instruction being performed
    struct Instruction instruction = Decode(cpu->IR);
    printf("OP: %02X | ", cpu->IR);

    char* INS = InsAsString(instruction.ins);
    printf("INS: %-3s   ", INS);

    printf("VAL: %02X   ", memory[cpu->PC]);

    //Print the values of the various data registers
    printf("REGISTERS: ");
    printf("IDX=%02X  IDY=%02X  IDZ=%02X   ", cpu->IDX, cpu->IDY, cpu->IDZ);

    //Print the state of the various flags
    printf("FLAGS: Z=%01X  E=%01X  C=%01X  V=%01X\n\n", cpu->Z, cpu->E, cpu->C, cpu->V);
}