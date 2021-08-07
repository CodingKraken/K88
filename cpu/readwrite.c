#include <stdio.h>
#include <stdint.h>

#include "cpu.h"

uint8_t ReadBit(uint8_t byte, int bitpos) {
    if(bitpos > 7 || bitpos < 0) return 0;

    uint8_t bit;
    bit = 1 << bitpos;
    return ((bit & byte) >> bitpos);
}

uint8_t ReadSubByte(uint8_t byte, int startpos, int endpos) {
    if(startpos < endpos || startpos > 7) return 0;

    uint8_t sub_byte = ReadBit(byte, startpos);
    for(int i = startpos-1; i >= endpos; i--) {
        sub_byte = sub_byte << 1;
        sub_byte += ReadBit(byte, i);
    }
    return sub_byte;
}

uint8_t ReadReg(struct CPU* cpu, int reg) {
    switch(reg) {
        case IDX: {
            return cpu->IDX;
        }
        case IDY: {
            return cpu->IDY;
        }
        case IDZ: {
            return cpu->IDZ;
        }
        case FGZ: {
            return cpu->Z;
        }
        case FGE: {
            return cpu->E;
        }
        case FGC: {
            return cpu->C;
        }
        case FGV: {
            return cpu->V;
        }
    }
}

uint8_t ReadAddress(struct CPU* cpu, uint8_t memory[0x10000]) {
    uint8_t adr_low = memory[cpu->PC];
    cpu->PC++;
    uint8_t adr_high = memory[cpu->PC];
    uint16_t adr_full = (adr_high << 8) | adr_low;
    return memory[adr_full];
}

void WriteReg(struct CPU* cpu, int reg, int val) {
    switch(reg) {
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
}