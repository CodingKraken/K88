#include <stdio.h>
#include <stdint.h>

#include "cpu.h"

/*
    The three read functions simply copy the data from either memory (ReadByte), the address of 
    the program counter (ReadAddress), or the data registers (ReadRegister)
*/

void ReadByte(struct CPU* cpu, uint8_t memory[0x10000], int location) {
    /*
        0 indicates a read from the program counter
        1 indicates a read from the stack 
        2 indicates a read from the address register
    */
    
    if(location == 1) {
        cpu->BR = memory[cpu->SP];
        return;
    }
    if(location == 2) {
        cpu->BR = memory[cpu->AR];
        return;
    }
    cpu->BR = memory[cpu->PC];
}

void ReadAddress(struct CPU* cpu, int islow) {
    if(islow) {
        cpu->BR = cpu->PC & 0xFF;
        return;
    }
    cpu->BR = cpu->PC >> 8;
}

void ReadRegister(struct CPU* cpu, uint8_t Register) {
        switch(Register) {
        case IDX: {
            cpu->BR = cpu->IDX;
            break;
        }
        case IDY: {
            cpu->BR = cpu->IDY;
            break;
        }
        case ACC: {
            cpu->BR = cpu->ACC;
            break;
        }
        case FGZ: {
            cpu->BR = cpu->Z;
            break;
        }
        case FGE: {
            cpu->BR = cpu->E;
            break;
        }
        case FGC: {
            cpu->BR = cpu->C;
            break;
        }
        case FGV: {
            cpu->BR = cpu->V;
            break;
        }
    }
}

/*
    Similarly to the read functions, the load functions load data from a destination to a source, given the
    buffer register and a target. LoadByte sets the value at a memory address equal to the buffer register,
    LoadAddress sets the low and high bytes of the address register based on the buffer register, and
    load register copies the data in the buffer register into one of the three data register
*/

void LoadByte(struct CPU* cpu, uint8_t memory[0x10000], uint16_t dest) {
    memory[dest] = cpu->BR;
}

void LoadAddress(struct CPU* cpu, int islow) {
    if(islow) {
        cpu->AR = cpu->BR;
        return;
    }
    cpu->AR += (uint16_t)cpu->BR << 8;
}

void LoadRegister(struct CPU* cpu, uint8_t Register) {
    switch(Register) {
        case IDX: {
            cpu->IDX = cpu->BR;
            break;
        }
        case IDY: {
            cpu->IDY = cpu->BR;
            break;
        }
        case ACC: {
            cpu->ACC = cpu->BR;
            break;
        }
        case FGZ: {
            cpu->Z = cpu->BR;
            break;
        }
        case FGE: {
            cpu->E = cpu->BR;
            break;
        }
        case FGC: {
            cpu->C = cpu->BR;
            break;
        }
        case FGV: {
            cpu->V = cpu->BR;
            break;
        }
    }
}