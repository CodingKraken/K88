#ifndef FUNCS_H
#define FUNCS_H

#include <stdint.h>

char* InsAsString(struct CPU* cpu);

void ReadByte(struct CPU* cpu, uint8_t memory[0x10000], int location);
void ReadAddress(struct CPU* cpu, int islow);
void LoadByte(struct CPU* cpu, uint8_t memory[0x10000], uint16_t dest);
void LoadAddress(struct CPU* cpu, int islow);

void ReadRegister(struct CPU* cpu, uint8_t Register);
void LoadRegister(struct CPU* cpu, uint8_t Register);

#endif