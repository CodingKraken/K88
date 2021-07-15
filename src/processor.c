#include <stdio.h>
#include <stdint.h>

#include "processor.h"
#include "funcs.h"

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

void PointerStep(struct CPU* cpu, uint8_t pointer, int incdec, int display) {

    switch(pointer) {
        case SP: {
            cpu->SP += incdec;
            break;
        }
        case PC: {
            cpu->PC += incdec;
            break;
        }
    }
    if(display) PrintState(cpu);
}

/*
    In a real processor, the ALU or Arithmetic and Logic Unit performs arithmetic operations on given inputs
    For the sake of simplicity, only addition and subtraction has been implemented. The result of these operations
    set the various processor flags, excluding the E (Equals) flag.
*/

void ALU(struct CPU* cpu, int op) {
    if(op == 1) {
        cpu->BR += cpu->ACC;
        if(cpu->BR < cpu->ACC) {cpu->C = 1; cpu->V = 1;} // If overflow set C and V flags high
        LoadRegister(cpu, ACC);
        if(cpu->ACC == 0) cpu->Z = 1; // If result is zero set Z flag high
        return;
    }
    cpu->BR -= cpu->ACC;
    if(cpu->ACC > cpu->BR) cpu->V = 1; // If underflow set V flag high
    LoadRegister(cpu, ACC);
    if(cpu->ACC == 0) cpu->Z = 1; // If result is zero set Z flag high
    
}

void ExecuteInstruction(struct CPU* cpu, uint8_t memory[0x10000]) {
    
    /*
        The Execution of an Instruction is split into two stages, those being:
        Fetch: The Instruction to be executed is read from the address pointed to by the PC and is loaded into the 
        Buffer Register for storage, and then passes that on to the instruction register where it is saved
    */
    
    // Fetch Stage 1) The Address register stores the location of the pc prior to reading the instruction
    cpu->AR = cpu->PC;
    // Fetch Stage 2) The Instruction to be read is saved to the Buffer Register and then is copied into the Instruction Register
    ReadByte(cpu, memory, 0);
    cpu->IR = cpu->BR;

    // Execute
    PointerStep(cpu, cpu->IR, 0, 1);
    switch(cpu->IR) {
        /*
            The following two instructions achieve simple addition and subtraction through
            the ALU. Comments explaining the various lines of code are here to establish the structure
            of other instructions
        */
        case ADD: {
            PointerStep(cpu, PC, 1, 1);
            ReadByte(cpu, memory, 0);         // Read the value of the first register to be added
            ReadRegister(cpu, cpu->BR);
            cpu->ACC = cpu->BR;               // Load this value into the accumulator for storage
            PointerStep(cpu, PC, 1, 1);
            ReadByte(cpu, memory, 0);         // Read the byte of the 2nd register to be added
            ReadRegister(cpu, cpu->BR);       
            ALU(cpu, 1);                      // Send data to ALU to complete the addition
            break;
        }
        case SUB: {
            PointerStep(cpu, PC, 1, 1);
            ReadByte(cpu, memory, 0);         // Read the value of the register to be subtracted
            ReadRegister(cpu, cpu->BR);
            cpu->ACC = cpu->BR;               // Load this value into the accumulator for storage
            PointerStep(cpu, PC, 1, 1);
            ReadByte(cpu, memory, 0);         
            ReadRegister(cpu, cpu->BR);       // Read the byte of the 2nd register to be added
            ALU(cpu, 0);                      // Send data to ALU to complete the subtraction
            break;
        }
        /* 
            The next 2 instructions serve to implement basic bitwise operations between registers, namely
            & (and), and | (or)
            And compares a value with the accumulator. If equal the equals flag is set high, if not the zero flag is
            Or compares a value with the accumulator. If they're both not zero the equals flag is set high, if not the zero flag is
        */
        case AND: {
            PointerStep(cpu, PC, 1, 1);
            ReadByte(cpu, memory, 0);
            cpu->BR = (cpu->ACC == cpu->BR);     // AND the value read from memory and the ACC, set the flags
            if(cpu->BR) { cpu->Z = 0; cpu->E = 1;}
            else { cpu->Z = 1; cpu->E = 0; }
            break;
        }
        case OR: {
            PointerStep(cpu, PC, 1, 1);
            ReadByte(cpu, memory, 0);
            cpu->BR = (cpu->ACC != 0 || cpu->BR != 0);       // Likewise with AND, OR the value read from memory and ACC, set the flags
            if(cpu->BR == 1) { cpu->Z = 0; cpu->E = 1; }
            else { cpu->Z = 1; cpu->E = 0; }
            break;
        }
        /*
            The comparison operation compares the value in a register with a value in memory
            The result of the comparison sets the cpu flags, based on this table:
            A < B: Z = 1; E = 0; C = 0;
            A = B: Z = 0; E = 1; C = 0;
            A > B: Z = 0; E = 0; C = 1;
        */
        case CMP: {
            PointerStep(cpu, PC, 1, 1);
            ReadByte(cpu, memory, 0);
            ReadRegister(cpu, cpu->BR);
            PointerStep(cpu, PC, 1, 1);
            if(cpu->BR < memory[cpu->PC])  { cpu->Z = 1; cpu->E = 0; cpu->C = 0; }
            if(cpu->BR == memory[cpu->PC]) { cpu->Z = 0; cpu->E = 1; cpu->C = 0; }
            if(cpu->BR > memory[cpu->PC])  { cpu->Z = 0; cpu->E = 0; cpu->C = 1; }
            break;
        }
        /*
            The next two instructions, psh (push) and pop load and retrieve data from the stack
            Push copies the data in a register  and stores it at the current location pointed to by the stack, 
            and decrementing the stack pointer
            Pop reads the data stored at the stack pointer and copies it into the accumulator, incrementing the SP.
        */
        case PSH: {
            PointerStep(cpu, PC, 1, 1);
            PointerStep(cpu, SP, -1, 1);
            ReadByte(cpu, memory, 0);
            ReadRegister(cpu, cpu->BR);
            LoadByte(cpu, memory, cpu->SP);
            break;
        }
        case POP: {
            ReadByte(cpu, memory, 1);
            LoadRegister(cpu, ACC);
            cpu->BR = 0;
            LoadByte(cpu, memory, cpu->SP);
            PointerStep(cpu, SP, 1, 1);
            break;
        }
        /*
            The three memory operations, MOV (Move), LD (Load), and STO (Store), allow for basic memory manipulation
            MOV copies data from one register to another
            LD takes a value from memory and stores it in a register
            STO is the inverse of LD, taking the value of a register and storing it in memory
        */
        case MOV: {
            PointerStep(cpu, PC, 1, 1);
            ReadByte(cpu, memory, 0);
            ReadRegister(cpu, cpu->BR);
            PointerStep(cpu, PC, 1, 1);
            LoadRegister(cpu, memory[cpu->PC]);
            break;
        }
        case LD: {
            PointerStep(cpu, PC, 1, 1);
            ReadByte(cpu, memory, 0);
            LoadAddress(cpu, 1);
            PointerStep(cpu, PC, 1, 1);
            ReadByte(cpu, memory, 0);
            LoadAddress(cpu, 0);
            ReadByte(cpu, memory, 2);
            PointerStep(cpu, PC, 1, 1);
            LoadRegister(cpu, memory[cpu->PC]);
            break;
        }
        case STO: {
            PointerStep(cpu, PC, 1, 1);
            ReadByte(cpu, memory, 0);
            LoadAddress(cpu, 1);
            PointerStep(cpu, PC, 1, 1);
            ReadByte(cpu, memory, 0);
            LoadAddress(cpu, 0);
            PointerStep(cpu, PC, 1, 1);
            ReadByte(cpu, memory, 0);
            ReadRegister(cpu, cpu->BR);
            LoadByte(cpu, memory, cpu->AR);
            break;
        }
        /*
            The next four instructions are the Jump instructions, one unconditional and three conditional ones
            JMP is completely unconditional, it simply takes a memory address and sets the program counter to that address
            JZ, JE, and JC Jump if their respective flags (Z, E, and C) are set high. If not they do not jump
        */
        case JMP: {
            PointerStep(cpu, PC, 1, 1);
            ReadByte(cpu, memory, 0);
            LoadAddress(cpu, 1);
            PointerStep(cpu, PC, 1, 1);
            ReadByte(cpu, memory, 0);
            LoadAddress(cpu, 0);
            cpu->PC = cpu->AR-1;
            break;
        }
        case JZ: {
            PointerStep(cpu, PC, 1, 1);
            if(cpu->Z == 1) {
                ReadByte(cpu, memory, 0);
                LoadAddress(cpu, 1);
                PointerStep(cpu, PC, 1, 1);
                ReadByte(cpu, memory, 0);
                LoadAddress(cpu, 0);
                cpu->PC = cpu->AR-1;
            } else PointerStep(cpu, PC, 1, 1);
            break;
        }
        case JE: {
            PointerStep(cpu, PC, 1, 1);
            if(cpu->E) {
                ReadByte(cpu, memory, 0);
                LoadAddress(cpu, 1);
                PointerStep(cpu, PC, 1, 1);
                ReadByte(cpu, memory, 0);
                LoadAddress(cpu, 0);
                cpu->PC = cpu->AR-1;
            } else PointerStep(cpu, PC, 1, 1);
            break;
        }
        case JC: {
            PointerStep(cpu, PC, 1, 1);
            if(cpu->C) {
                ReadByte(cpu, memory, 0);
                LoadAddress(cpu, 1);
                PointerStep(cpu, PC, 1, 1);
                ReadByte(cpu, memory, 0);
                LoadAddress(cpu, 0);
                cpu->PC = cpu->AR-1;
            } else PointerStep(cpu, PC, 1, 1);
            break;
        }
        /*
            Below are the subroutine instructions, which allow for function (in this case subroutine) calls
            JSR or Jump to Subroutine Jumps to a subroutine and writes the a return address to the stack, which is
            simply the current address of the program counter prior to the jump
            RTS or Return from Subroutine reads the written address written in the stack and sets the program counter
            accordingly
        */
        case JSR: {
            PointerStep(cpu, PC, 1, 1);
            ReadAddress(cpu, 0);
            LoadByte(cpu, memory, cpu->SP);
            PointerStep(cpu, SP, -1, 1);
            ReadAddress(cpu, 1);
            LoadByte(cpu, memory, cpu->SP);
            PointerStep(cpu, SP, -1, 1);
            ReadByte(cpu, memory, 0);
            LoadAddress(cpu, 1);
            PointerStep(cpu, PC, 1, 1);
            ReadByte(cpu, memory, 0);
            LoadAddress(cpu, 0);
            cpu->PC = cpu->AR-1;
            break;
        }
        case RTS: {
            PointerStep(cpu, SP, 1, 1);
            ReadByte(cpu, memory, 1);
            LoadAddress(cpu, 1);
            PointerStep(cpu, SP, 1, 1);
            ReadByte(cpu, memory, 1);
            LoadAddress(cpu, 0);
            cpu->PC = cpu->AR+1;
            break;
        }
        /*
            Below are the final main processor instructions. These are incredibly simple and don't do much of anything
            NOP or No Operation just wastes a clock cycle, and does not affect any register or memory address
            RST or Reset sets the program counter to the reset vector, effectively resetting the processor
            HLT or Halt completely stops the CPU and ends program
        */
        case NOP: {
            break;
        }
        case RST: {
            cpu->PC = 0xFFEC;
            break;
        }
        case HLT: {
            cpu->HLT = 1;
        }
    }
    PointerStep(cpu, PC, 1, 0);
}

void ProcessorInit(struct CPU* cpu, uint8_t memory[0x10000]) {
    cpu->PC = 0xFFEC;
    cpu->SP = 0xFF;

    cpu->AR = 0xFFEB;
    cpu->BR = 0x00;
    cpu->IR = 0x13;

    cpu->IDX = 0x00;
    cpu->IDY = 0x00;
    cpu->ACC = 0x00;

    cpu->Z = 0;
    cpu->E = 0;
    cpu->C = 0;
    cpu->V = 0;

    cpu->HLT = 0;
}

void ProcessorLoop(struct CPU* cpu, uint8_t memory[0x10000]) {
    while(!cpu->HLT) {
        ExecuteInstruction(cpu, memory);
    }
}
