/*
    The K88 8-bit processor as emulated by a computer
    
    The idea for this project takes significant inspiration from JDH's JDH-8, 
    along with Ben Eater's 8 bit computer series.

    The goals are to create a fully turing complete 8 bit processor, with a relatively simple yet 
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

#include "cpu.h"

int main() {
    struct CPU cpu;
    uint8_t memory[0x10000] = { 0 };

    ProcessorInit(&cpu, memory);

    // storing constant '1'
    memory[0x8100] = 0x01;

    // jump to start of program at 0x8000
    memory[0xFFF0] = 0b10110000;
    memory[0xFFF1] = 0x00;
    memory[0xFFF2] = 0x80;

    // Main

    // move 1 into IDY
    memory[0x8000] = 0b10010001;
    memory[0x8001] = 0x00;
    memory[0x8002] = 0x81;

    // jump to Fibloop Subroutine
    memory[0x8003] = 0b11010000;
    memory[0x8004] = 0x07;
    memory[0x8005] = 0x80;

    // halt instruction
    memory[0x8006] = 0b11111111;

    // Fibloop

    // move idy to idz
    memory[0x8007] = 0b10011010;
    memory[0x8008] = 0x01;

    // add idx to idz
    memory[0x8009] = 0b00001010;
    memory[0x800A] = 0x00;

    // move idy into idx
    memory[0x800B] = 0b10011000;
    memory[0x800C] = 0x01;

    // move idz into idy
    memory[0x800D] = 0b10011001;
    memory[0x800E] = 0x2;

    // compare 1 to carry flag
    memory[0x800F] = 0b01100101;
    memory[0x8010] = 0x00;
    memory[0x8011] = 0x81;

    // if carry flag not set, jump to top of fibloop
    memory[0x8012] = 0b11000011;
    memory[0x8013] = 0x07;
    memory[0x8014] = 0x80;

    // otherwise, return from subroutine
    memory[0x8015] = 0b11100000;

    ProcessorLoop(&cpu, memory);

    return 0;
}