#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STACK_SIZE 65536

FILE *memFile;

uint8_t memory[STACK_SIZE];
uint32_t address, instruction, pc, gpr[32], instructionCount = 0, opcode;

void store() {
    uint32_t function = (instruction >> 12) & 0x7;
    uint32_t rs1 = (instruction >> 15) & 0x1F;
    uint32_t rs2 = (instruction >> 20) & 0x1F;
    uint32_t upper = (instruction >> 31) ? 0xFFFFF000 : 0;
    uint32_t middle = (instruction >> 20) & 0xFE0;
    uint32_t lower = (instruction >> 7) & 0x1F;
    uint32_t immediate = upper | middle | lower;
    uint32_t byteAddress = immediate + gpr[rs1];

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
    uint32_t function = (instruction >> 12) & 0x7;
    uint32_t rs = (instruction >> 15) & 0x1F;
    uint32_t rd = (instruction >> 7) & 0x1F;
    uint32_t upper = (instruction >> 31) ? 0xFFFFF000 : 0;
    uint32_t lower = (instruction >> 20) & 0xFFF;
    uint32_t immediate = upper | lower;
    uint32_t byteAddress = immediate + gpr[rs];

    switch (function)
    {
    case 0:
        gpr[rd] = (memory[byteAddress] >> 7) ? (memory[byteAddress] | 0xFFFFFF00) : memory[byteAddress] & 0xFF;
        break;
    case 1:
        gpr[rd] = (memory[byteAddress + 1] >> 7) ? (memory[byteAddress] | (memory[byteAddress + 1] << 8) | 0xFFFF0000) : ((memory[byteAddress] | (memory[byteAddress + 1] << 8)) & 0xFFFF);
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
    uint32_t function = (instruction >> 12) & 0x7;
    uint32_t rs = (instruction >> 15) & 0x1F;
    uint32_t rd = (instruction >> 7) & 0x1F;
    uint32_t upper = (instruction >> 31) ? 0xFFFFF000 : 0;
    uint32_t lower = (instruction >> 20) & 0xFFF;
    uint32_t immediate = upper | lower;

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
    if (argc > 4)
	{
		printf(stderr, "Usage: %s [mem_file] [starting address] [stack address]\n", argv[0]);
        return EXIT_FAILURE;
	}
    memFile = argc > 1 ? fopen(argv[1], "r") : fopen("program.mem", "r");
    if (!memFile) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    pc = argc > 2 ? argv[2] : 0;
    gpr[2] = argc > 3 ? argv[3] : 65536;

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

    while (memory[pc] | (memory[pc + 1] << 8) | (memory[pc + 2] << 16) | (memory[pc + 3] << 24)) {
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