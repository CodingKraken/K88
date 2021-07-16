#include <stdio.h>

#include "cpu.h"

char* InsAsString(struct CPU* cpu) {
    char* INS;
    switch(cpu->IR) {
        case ADD: {
            INS = "ADD";
            break;
        }
        case SUB: {
            INS = "SUB";
            break;
        }
        case AND: {
            INS = "AND";
            break;
        }
        case OR: {
            INS = "OR";
            break;
        }
        case CMP: {
            INS = "CMP";
            break;
        }
        case PSH: {
            INS = "PSH";
            break;
        }
        case POP: {
            INS = "POP";
            break;
        }
        case MOV: {
            INS = "MOV";
            break;
        }
        case LD: {
            INS = "LD";
            break;
        }
        case STO: {
            INS = "STO";
            break;
        }
        case JMP: {
            INS = "JMP";
            break;
        }
        case JZ: {
            INS = "JZ";
            break;
        }
        case JE: {
            INS = "JE";
            break;
        }
        case JC: {
            INS = "JC";
            break;
        }
        case JSR: {
            INS = "JSR";
            break;
        }
        case RTS: {
            INS = "RTS";
            break;
        }
        case NOP: {
            INS = "NOP";
            break;
        }
        case RST: {
            INS = "RST";
            break;
        }
        case HLT: {
            INS = "HLT";
            break;
        }
    }
    return INS;
}

void PrintState(struct CPU* cpu) {
    // Print Address of the Program Counter and Stack Pointer
    printf("PC: %04X | ", cpu->PC);
    printf("AR: %04X | ", cpu->AR);
    printf("SP: %02X | ", cpu->SP);

    // Print the value of the buffer register
    printf("BR: %02X   ", cpu->BR);

    // Print the instruction being performed
    char* INS = InsAsString(cpu);
    printf("INS: %-3s   ", INS);

    //Print the values of the various data registers
    printf("REGISTERS: ");
    printf("IDX=%02X  IDY=%02X  ACC=%02X   ", cpu->IDX, cpu->IDY, cpu->ACC);

    //Print the state of the various flags
    printf("FLAGS: Z=%01X  E=%01X  C=%01X  V=%01X\n", cpu->Z, cpu->E, cpu->C, cpu->V);
}
