#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STACK_SIZE 65536

FILE *memFile;

int8_t memory[STACK_SIZE];
uint16_t address;
int instruction, pc = 0, gpr[32], instructionCount = 0, opcode;

void store() {
    int function = (instruction >> 12) & 0x7;
    int rs1 = (instruction >> 15) & 0x1F;
    int rs2 = (instruction >> 20) & 0x1F;
    int upper = (instruction >> 31) ? 0xFFFFF000 : 0;
    int middle = (instruction >> 20) & 0xFE0;
    int lower = (instruction >> 7) & 0x1F;
    int immediate = upper | middle | lower;
    int byteAddress = immediate + gpr[rs1];

    switch (function)
    {
    case 0:
        memory[byteAddress] = gpr[rs2];
        break;
    case 1:
        memory[byteAddress] = gpr[rs2];
        memory[byteAddress + 1] = gpr[rs2] >> 8;
        break;
    case 2:
        memory[byteAddress] = gpr[rs2];
        memory[byteAddress + 1] = gpr[rs2] >> 8;
        memory[byteAddress + 2] = gpr[rs2] >> 16;
        memory[byteAddress + 3] = gpr[rs2] >> 24;
        break;
    default:
        break;
    }
}

void load() {
    int function = (instruction >> 12) & 0x7;
    int rs = (instruction >> 15) & 0x1F;
    int rd = (instruction >> 7) & 0x1F;
    int upper = (instruction >> 31) ? 0xFFFFF000 : 0;
    int lower = (instruction >> 20) & 0xFFF;
    int immediate = upper | lower;
    int byteAddress = immediate + gpr[rs];

    switch (function)
    {
    case 0:
        gpr[rd] = memory[byteAddress];
        break;
    case 1:
        gpr[rd] = (memory[byteAddress] | (memory[byteAddress + 1] << 8));
        break;
    case 2:
        gpr[rd] = (memory[byteAddress] | (memory[byteAddress + 1] << 8) | (memory[byteAddress + 2] << 16) | (memory[byteAddress + 3] << 24));
        break;
    case 4:
        gpr[rd] = memory[byteAddress] & 0xFF;
        break;
    case 5:
        gpr[rd] = (memory[byteAddress] | (memory[byteAddress + 1] << 8)) & 0xFFFF;
        break;
    default:
        break;
    }
}

void immediate() {
    int function = (instruction >> 12) & 0x7;
    int rs = (instruction >> 15) & 0x1F;
    int rd = (instruction >> 7) & 0x1F;
    int upper = (instruction >> 31) ? 0xFFFFF000 : 0;
    int lower = (instruction >> 20) & 0xFFF;
    int immediate = upper | lower;
    // int byteAddress = immediate + gpr[rs]; 

    switch (function)
    {
    case 0:
        gpr[rd] = gpr[rs] + immediate;
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[2]) {
    if (argc > 2)
	{
		printf(stderr, "Usage: %s [memory_file]\n", argv[0]);
        return EXIT_FAILURE;
	}

    memFile = argc == 2 ? fopen(argv[1], "r") : fopen("program.mem", "r");
    if (!memFile) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    gpr[2] = -1;

    while (fscanf(memFile, "%x: %x", &address, &instruction) == 2) {
        if (address + 3 >= STACK_SIZE) {
            printf(stderr, "Error: Address out of bounds (0x%hx)\n", address);
            continue;
        }
        memory[address] = instruction;
        memory[address + 1] = instruction >> 8;
        memory[address + 2] = instruction >> 16;
        memory[address + 3] = instruction >> 24;
        instructionCount += 4;
    }

    while (pc < instructionCount) {
        instruction = (memory[pc] | (memory[pc + 1] << 8) | (memory[pc + 2] << 16) | (memory[pc + 3] << 24));
        opcode = instruction & 0x7F;
        switch (opcode)
        {
        case 0x23:
            store();
            pc += 4;
            break;
        case 0x3:
            load();
            pc += 4;
            break;
        case 0x13:
            immediate();
            pc += 4;
            break;
        default:
            pc += 4;
            break;
        }
    }

    fclose(memFile);

    return 0;
}