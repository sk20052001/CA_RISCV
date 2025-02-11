#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MEMORY_SIZE 65536

FILE *memFile;

uint8_t memory[MEMORY_SIZE];
uint32_t address, instruction, pc, gpr[32] = {0}, opcode;

void pcDisplay(char *instructionType) {
    printf("PC: %08x\n", pc);
    printf("Instruction type: %s\tInstruction: %08x\n\n", instructionType, instruction);
}

void registerFileDisplay() {
    printf("Hardwired zero: %08x\n", gpr[0]);
    printf("Return Address (ra): %08x\n", gpr[1]);
    printf("Stack Pointer (sp): %08x\n", gpr[2]);
    printf("Global Pointer (gp): %08x\n", gpr[3]);
    printf("Thread Pointer (tp): %08x\n", gpr[4]);
    printf("Temporaries:\nt0: %08x\tt1: %08x\tt2: %08x\n", gpr[5], gpr[6], gpr[7]);
    printf("Frame Pointer (s0): %08x\n", gpr[8]);
    printf("Saved Register (s1): %08x\n", gpr[9]);
    printf("Function arguments/ Return values:\na0: %08x\ta1: %08x\n", gpr[10], gpr[11]);
    printf("Function arguments:\na2: %08x\ta3: %08x\ta4: %08x\ta5: %08x\ta6: %08x\ta7: %08x\n", gpr[12], gpr[13], gpr[14], gpr[15], gpr[16], gpr[17]);
    printf("Saved Register:\ns2: %08x\ts3: %08x\ts4: %08x\ts5: %08x\ts6: %08x\n", gpr[18], gpr[19], gpr[20], gpr[21], gpr[22]);
    printf("s7: %08x\ts8: %08x\ts9: %08x\ts10: %08x\ts11: %08x\n", gpr[23], gpr[24], gpr[25], gpr[26], gpr[27]);
    printf("Temporaries:\nt3: %08x\tt4: %08x\tt5: %08x\tt6: %08x\n\n", gpr[28], gpr[29], gpr[30], gpr[31]);
}

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
        pcDisplay("sb");
        memory[byteAddress] = gpr[rs2];
        break;
    case 1:
        pcDisplay("sh");
        memory[byteAddress] = gpr[rs2];
        memory[byteAddress + 1] = gpr[rs2] >> 8;
        break;
    case 2:
        pcDisplay("sw");
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
        pcDisplay("lb");
        gpr[rd] = (memory[byteAddress] >> 7) ? (memory[byteAddress] | 0xFFFFFF00) : memory[byteAddress] & 0xFF;
        break;
    case 1:
        pcDisplay("lh");
        gpr[rd] = (memory[byteAddress + 1] >> 7) ? (memory[byteAddress] | (memory[byteAddress + 1] << 8) | 0xFFFF0000) : ((memory[byteAddress] | (memory[byteAddress + 1] << 8)) & 0xFFFF);
        break;
    case 2:
        pcDisplay("lw");
        gpr[rd] = (memory[byteAddress] | (memory[byteAddress + 1] << 8) | (memory[byteAddress + 2] << 16) | (memory[byteAddress + 3] << 24));
        break;
    case 4:
        pcDisplay("lbu");
        gpr[rd] = memory[byteAddress] & 0xFF;
        break;
    case 5:
        pcDisplay("lhu");
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
        pcDisplay("addi");
        gpr[rd] = gpr[rs] + immediate;
        break;
    default:
        break;
    }
}

void lui() {
    uint32_t rd = (instruction >> 7) & 0x1F;
    pcDisplay("lui");
    gpr[rd] = instruction & 0xFFFFFF000;
}

void auipc() {
    uint32_t rd = (instruction >> 7) & 0x1F;
    pcDisplay("auipc");
    gpr[rd] = (instruction & 0xFFFFF000) + pc;
}

int main(int argc, char *argv[4]) {
    if (argc > 4)
	{
		printf("Usage: %s [mem_file] [starting address] [stack address]\n", argv[0]);
        return 0;
	}
    memFile = argc > 1 ? fopen(argv[1], "r") : fopen("test.mem", "r");
    if (!memFile) {
        perror("Error opening file");
        memFile = fopen("test.mem", "r");
        if (!memFile) {
            perror("Error opening default file");
            return 0;
        }
    }
    pc = argc > 2 ? atoi(argv[2]) : 0;
    gpr[2] = argc > 3 ? atoi(argv[3]) : 65536;

    while (fscanf(memFile, "%x: %x", &address, &instruction) == 2) {
        if (address + 3 >= MEMORY_SIZE) {
            printf("Error: Address out of bounds (0x%hx)\n", address);
            continue;
        }
        memory[address] = instruction;
        memory[address + 1] = instruction >> 8;
        memory[address + 2] = instruction >> 16;
        memory[address + 3] = instruction >> 24;
    }

    fclose(memFile);

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
        case 0x37:
            lui();
            pc += 4;
            break;
        case 0x17:
            lui();
            pc += 4;
            break;
        default:
            pc += 4;
            break;
        }
        registerFileDisplay();
    }

    return 0;
}