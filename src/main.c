/*
    The K88 8-bit processor as emulated by a computer
    
    The idea for this project takes significant inspiration from JDH's JDH-8, 
    along with Ben Eater's 8 bit computer series.

    The goals are to create a fully turing complete cpu processor, with a relatively simple yet 
    readable instruction set and assembly language, and an end goal of emulating a processor fairly close to a physical
    implementation.

    Data regarding the CPU structure and instruction handling can be found in the processor header and c file, with
    read/write functions coming from the funcs header file and the readwrite/utils c files.

    Below is a program which calculates fibonacci numbers. While they aren't directly outputted, the respective
    values are calculated and then stored in the Accumulator. A more human readable version of this program can be found
    in fibonacci.ksm
*/

#include <stdio.h>
#include <stdint.h>

#include "processor.h"
#include "funcs.h"

int main() {
    struct CPU cpu;
    uint8_t memory[0x10000];
    
    ProcessorInit(&cpu, memory);

    /* 
    These memory locations have had specific values initialized to them, 
    similar to variable declarations. These are from where the load command can read from
    */
    memory[0x8100] = 0x01;
    memory[0x8101] = 0x00;

    // Boot sequence
    memory[0xFFEC] = JMP;
    memory[0xFFED] = 0x00;
    memory[0xFFEE] = 0x80;

    // Start Label
    memory[0x8000] = LD;
    memory[0x8001] = 0x00;
    memory[0x8002] = 0x81;
    memory[0x8003] = IDY;

    memory[0x8004] = JSR;
    memory[0x8005] = 0x08;
    memory[0x8006] = 0x80;

    memory[0x8007] = HLT;

    // Fibloop Label
    
    memory[0x8008] = LD;
    memory[0x8009] = 0x01;
    memory[0x800A] = 0x81;
    memory[0x800B] = FGZ;

    memory[0x800C] = ADD;
    memory[0x800D] = IDX;
    memory[0x800E] = IDY;

    memory[0x800F] = MOV;
    memory[0x8010] = IDY;
    memory[0x8011] = IDX;

    memory[0x8012] = MOV;
    memory[0x8013] = ACC;
    memory[0x8014] = IDY;

    memory[0x8015] = CMP;
    memory[0x8016] = FGC;
    memory[0x8017] = 1;

    memory[0x8018] = JZ;
    memory[0x8019] = 0x08;
    memory[0x801A] = 0x80;

    memory[0x801B] = RTS;

    ProcessorLoop(&cpu, memory);
    return 0;
}