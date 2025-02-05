#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STACK_SIZE 65536

FILE *memFile;

uint8_t stack[STACK_SIZE];
uint16_t address;
uint32_t instruction, pc = 0, gpr[32], instructionCount = 0, opcode;

void store(int instruction) {
    uint32_t function = (instruction >> 12) & 0x00000007;
    uint32_t rs1 = (instruction >> 15) & 0x0000001F;
    uint32_t rs2 = (instruction >> 20) & 0x0000001F;
    int immediate = (instruction >> 7) & 0x0000001F;
}

int main(int argc, char *argv[2]) {
    if (argc > 2)
	{
		fprintf(stderr, "Usage: %s [memory_file]\n", argv[0]);
        return EXIT_FAILURE;
	}

    memFile = argc == 2 ? fopen(argv[1], "r") : fopen("program.mem", "r");
    if (!memFile) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    while (fscanf(memFile, "%x: %x", &address, &instruction) == 2) {
        if (address + 3 >= STACK_SIZE) {
            fprintf(stderr, "Error: Address out of bounds (0x%hx)\n", address);
            continue;
        }
        stack[address] = instruction & 0x000000FF;
        stack[address + 1] = (instruction >> 8) & 0x000000FF;
        stack[address + 2] = (instruction >> 16) & 0x000000FF;
        stack[address + 3] = (instruction >> 24) & 0x000000FF;
        instructionCount++;
    }

    // printf("Contents of Stack:\n");
    // for (int i = 0; i <= instructionCount; i += 4) {
    //     printf("%08x\n", (stack[i] | (stack[i + 1] << 8) | (stack[i + 2] << 16) | (stack[i + 3] << 24)));
    // }

    while ((pc / 4) < instructionCount) {
        instruction = (stack[pc] | (stack[pc + 1] << 8) | (stack[pc + 2] << 16) | (stack[pc + 3] << 24));
        opcode = instruction & 0x0000007F;
        switch (opcode)
        {
        case 0x23:
            store(instruction);
            break;
        default:
            pc += 4;
        }
    }

    fclose(memFile);

    return 0;
}