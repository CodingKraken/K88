#ifndef CPU_H
#define CPU_H

#include <stdint.h>

enum Instruction {
    ADD,
    SUB,
    AND,
    OR,
    CMP,
    PSH,
    POP,
    MOV,
    LD,
    STO,
    JMP,
    JZ,
    JE,
    JC,
    JSR,
    RTS,
    NOP,
    RST,
    HLT
};

enum Register {
    SP,
    PC,
    IDX,
    IDY,
    ACC,
    FGZ,
    FGE,
    FGC,
    FGV
};

struct CPU {

    /*
        The CPU has five primary registers for reading instructions, 2 16-bit address registers, and 3 8-bit registers
        
        The Program Counter stores the current location of memory being read
        The Address Register copies the location of the program counter prior to execution of an instruction
        The Stack Pointer points to the last location where data was written to the stack.
        The Buffer Register stores the value of the memory address being read
        The Instruction Register stores the opcode of the current instruction being executed.
    */

    uint16_t  PC, AR;
    uint8_t   SP, BR, IR;

    /*
        The three data registers include two general purpose registers prefixed ID- (IDX and IDY) and an
        Accumulator (ACC), which has extra functionality used in arithmetic operations. All of the data registers are 8 bit. 
    */

    uint8_t   IDX, IDY;
    uint8_t   ACC;

    /*
        The four flags in the 4bit flags register are split up into one bit bitfields. The Flags store the status of the processor
        
        Z: The Z flag or Zero Flag indicates whether or not an arithmetic or comparison operation resulted in a zero
        E: The E flag or Equals Flag indicates whether or not a comparison operation resulted in equality
        C: The C flag or Carry Flag indicates whether or not an arithmetic or comparison operation resulted in a Carry (such as overflow)
        V: The V flag or Overflow Flag indicates whether or not an arithmetic operation violated two's complement
    */

    uint8_t Z : 1;
    uint8_t E : 1;
    uint8_t C : 1;
    uint8_t V : 1;

    // The Halt bit, if high the cpu will no longer execute instructions
    int HLT;
};

char* InsAsString(struct CPU* cpu);
void PrintState(struct CPU* cpu);

void ALU(struct CPU* cpu, int op);
void PointerStep(struct CPU* cpu, uint8_t pointer, int incdec, int display);
void ExecuteInstruction(struct CPU* cpu, uint8_t memory[0x10000]);

void ProcessorInit(struct CPU* cpu, uint8_t memory[0x10000]);
void ProcessorLoop(struct CPU* cpu, uint8_t memory[0x10000]);

void ReadByte(struct CPU* cpu, uint8_t memory[0x10000], int location);
void ReadAddress(struct CPU* cpu, int islow);
void ReadRegister(struct CPU* cpu, uint8_t Register);

void LoadByte(struct CPU* cpu, uint8_t memory[0x10000], uint16_t dest);
void LoadAddress(struct CPU* cpu, int islow);
void LoadRegister(struct CPU* cpu, uint8_t Register);

#endif