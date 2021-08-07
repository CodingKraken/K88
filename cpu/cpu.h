#ifndef CPU_H
#define CPU_H

#include <stdint.h>

enum InstructionSet {
    ADD = 0x00,
    SUB = 0x01,
    AND = 0x02,
    OR  = 0x03,
    NOR = 0x04,
    CLF = 0x05,
    CMP = 0x06,
    PSH = 0x07,
    POP = 0x08,
    MOV = 0x09,
    STO = 0x0A,
    JMP = 0x0B,
    JFS = 0x0C,
    JSR = 0x0D,
    RTS = 0x0E,
    NOP = 0x0F
};

enum Register {
    IDX = 0x0,
    IDY = 0x1,
    IDZ = 0x2,
    FGZ = 0x3,
    FGE = 0x4,
    FGC = 0x5,
    FGV = 0x6,

    SP  = 0xE,
    PC  = 0xF
};

struct Instruction {
    uint8_t ins  : 4;
    uint8_t mode : 1;
    uint8_t reg  : 3;
};

struct CPU {

    /*
        The CPU has 3 registers for reading and processing instructions, a 16-bit address register, and 4 8-bit registers
        
        The Program Counter stores the current location of memory being read
        The Stack Pointer points to the last location where data was written to the stack.
    */

    uint16_t  PC;
    uint8_t   SP;

    uint8_t   IR;

    /*
        The three data registers include two general purpose registers prefixed ID- (IDX and IDY) and an
        Accumulator (ACC), which has extra functionality used in arithmetic operations. All of the data registers are 8 bit. 
    */

    uint8_t   IDX, IDY, IDZ;

    /*
        The four flags in the 4bit flags register are split up into one bit bitfields. The Flags store the status of the processor
        
        Z: The Z flag or Zero Flag indicates whether or not an arithmetic or comparison operation resulted in a zero
        E: The E flag or Equals Flag indicates whether or not a comparison operation resulted in equality
        C: The C flag or Carry Flag indicates whether or not an arithmetic or comparison operation resulted in a Carry (such as overflow)
        V: The V flag or Overflow Flag indicates whether or not an arithmetic operation violated two's complement

        H: The Halt Flag, if set high the cpu stops executing instructions until set low again
    */

    uint8_t Z : 1;
    uint8_t E : 1;
    uint8_t C : 1;
    uint8_t V : 1;

    uint8_t H : 1;
};

char* InsAsString(uint8_t instruction);
void PrintState(struct CPU* cpu, uint8_t memory[0x10000]);

void PointerStep(struct CPU* cpu, uint8_t[0x10000], uint8_t pointer, int incdec, int display);

struct Instruction Decode(uint8_t opcode);
void ALU(struct CPU* cpu, uint8_t memory[0x10000], int val);

void ExecuteInstruction(struct CPU* cpu, uint8_t memory[0x10000]);

void ProcessorInit(struct CPU* cpu, uint8_t memory[0x10000]);
void ProcessorLoop(struct CPU* cpu, uint8_t memory[0x10000]);

uint8_t ReadBit(uint8_t byte, int bitpos);
uint8_t ReadSubByte(uint8_t byte, int startpos, int endpos);
uint8_t ReadReg(struct CPU* cpu, int reg);
uint8_t ReadAddress(struct CPU* cpu, uint8_t memory[0x10000]);

void WriteReg(struct CPU* cpu, int reg, int val);

#endif