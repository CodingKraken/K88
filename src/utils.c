#include "processor.h"
#include "funcs.h"

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