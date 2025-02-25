#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "defines.c"

FILE *memFile;

uint8_t memory[MEMORY_SIZE];
uint32_t address, instruction, pc, gpr[32] = {0}, opcode;

// Start of Author: Sanjeev Krishnan
void display_pc_instruction(char *instructionType) {
    printf("PC: %08x\n", pc);
    printf("Instruction type: %s\tInstruction: %08x\n\n", instructionType, instruction);
}

void displayRegisterFile() {
    printf("x0 (zero): %08x\n", ZERO);
    printf("x1 (ra): %08x\n", RA);
    printf("x2 (sp): %08x\n", SP);
    printf("x3 (gp): %08x\n", GP);
    printf("x4 (tp): %08x\n", TP);
    printf("x5 (t0): %08x\tx6 (t1): %08x\tx7 (t2): %08x\n", T0, T1, T2);
    printf("x8 (s0): %08x\n", S0);
    printf("x9 (s1): %08x\n", S1);
    printf("x10 (a0): %08x\tx11 (a1): %08x\tx12 (a2): %08x\tx13 (a3): %08x\n", A0, A1, A2, A3);
    printf("x14 (a4): %08x\tx15 (a5): %08x\tx16 (a6): %08x\tx17 (a7): %08x\n", A4, A5, A6, A7);
    printf("x18 (s2): %08x\tx19 (s3): %08x\tx20 (s4): %08x\tx21 (s5): %08x\tx22 (s6): %08x\n", S2, S3, S4, S5, S6);
    printf("x23 (s7): %08x\tx24 (s8): %08x\tx25 (s9): %08x\tx26 (s10): %08x\tx27 (s11): %08x\n", S7, S8, S9, S10, S11);
    printf("x28 (t3): %08x\tx29 (t4): %08x\tx30 (t5): %08x\tx31 (t6): %08x\n\n", T3, T4, T5, T6);
}

void writeMem(uint32_t byteAddress, int numberOfBytes, uint32_t data) {
    switch (numberOfBytes) {
        case LB:
            memory[byteAddress] = data;
            break;
        case LH:
            memory[byteAddress] = data;
            memory[byteAddress + 1] = data >> 8;
            break;
        case LW:
            memory[byteAddress] = data;
            memory[byteAddress + 1] = data >> 8;
            memory[byteAddress + 2] = data >> 16;
            memory[byteAddress + 3] = data >> 24;
        break;
    default:
        break;
    }
}

uint32_t readMem(uint32_t byteAddress, int numberOfBytes) {
    switch (numberOfBytes) {
        case LB:
            return memory[byteAddress] & 0xFF;
            break;
        case LH:
            return (memory[byteAddress] | (memory[byteAddress + 1] << 8)) & 0xFFFF;
            break;
        case LW:
            return (memory[byteAddress] | (memory[byteAddress + 1] << 8) | (memory[byteAddress + 2] << 16) | (memory[byteAddress + 3] << 24));
            break;
        default:
            break;
    }
}

void store() {
    uint32_t function = FUNCT3(instruction);
    uint32_t rs1 = RS1(instruction);
    uint32_t rs2 = RS2(instruction);
    uint32_t immediate = STORE_IMMEDIATE(instruction);
    uint32_t byteAddress = immediate + gpr[rs1];

    switch (function) {
        case 0:
            display_pc_instruction("sb");
            writeMem(byteAddress, 1, gpr[rs2]);
            break;
        case 1:
            display_pc_instruction("sh");
            writeMem(byteAddress, 2, gpr[rs2]);
            break;
        case 2:
            display_pc_instruction("sw");
            writeMem(byteAddress, 4, gpr[rs2]);
            break;
        default:
            break;
    }
}

void load() {
    uint32_t function = FUNCT3(instruction);
    uint32_t rs1 = RS1(instruction);
    uint32_t rd = RD(instruction);
    uint32_t immediate = IMMEDIATE(instruction);
    uint32_t byteAddress = immediate + gpr[rs1];
    uint32_t loadedValue;

    switch (function) {
        case 0:
            display_pc_instruction("lb");
            loadedValue = readMem(byteAddress, LB);
            gpr[rd] = (loadedValue & 0x80) ? (loadedValue | 0xFFFFFF00) : loadedValue;
            break;
        case 1:
            display_pc_instruction("lh");
            loadedValue = readMem(byteAddress, LH);
            gpr[rd] = (loadedValue & 0x8000) ? (loadedValue | 0xFFFF0000) : loadedValue;
            break;
        case 2:
            display_pc_instruction("lw");
            gpr[rd] = readMem(byteAddress, LW);
            break;
        case 4:
            display_pc_instruction("lbu");
            gpr[rd] = readMem(byteAddress, LB);
            break;
        case 5:
            display_pc_instruction("lhu");
            gpr[rd] = readMem(byteAddress, LH);
            break;
        default:
            break;
    }
}
// End of Author: Sanjeev krishnan

// Start of Author: Siddesh Patil
void logical() {
    uint32_t funct3 = FUNCT3(instruction);
    uint32_t funct7 = FUNCT7(instruction);
    uint32_t rs1 = RS1(instruction);
    uint32_t rd = RD(instruction);
    uint32_t immediate = IMMEDIATE(instruction);
    uint32_t shamt = RS2(instruction);

    switch (funct3) {
        case 0:
            display_pc_instruction("addi");
            gpr[rd] = gpr[rs1] + immediate;
            break;
        case 1:
            display_pc_instruction("slli");
            gpr[rd] = gpr[rs1] << shamt;
            break;
        case 2:
            display_pc_instruction("slti");
            gpr[rd] = ((int32_t)gpr[rs1] < (int32_t)immediate);
            break;
        case 3:
            display_pc_instruction("sltiu");
            gpr[rd] = (gpr[rs1] < immediate);
            break;
        case 4:
            display_pc_instruction("xori");
            gpr[rd] = gpr[rs1] ^ immediate;
            break;
        case 5:
            if (!funct7) {
                display_pc_instruction("srli");
                gpr[rd] = gpr[rs1] >> shamt;
                break;
            } else {
                display_pc_instruction("srai");
                gpr[rd] = (gpr[rs1] & 0x80000000) ? (gpr[rs1] >> shamt | ~(0xFFFFFFFF >> shamt)) : gpr[rs1] >> shamt;
                break;
            }
        case 6:
            display_pc_instruction("ori");
            gpr[rd] = gpr[rs1] | immediate;
            break;
        case 7:
            display_pc_instruction("andi");
            gpr[rd] = gpr[rs1] & immediate;
            break;
        default:
            break;
    }
}
// End of Author: Siddesh Patil

// Start of Author: Satyajit Deokar
void arithmetic(){
    uint32_t funct3 = FUNCT3(instruction);
    uint32_t funct7 = FUNCT7(instruction);
    uint32_t rs1 = RS1(instruction);
    uint32_t rs2 = RS2(instruction);
    uint32_t rd = RD(instruction);
    uint32_t shamt = RS2(instruction);
    int32_t rs1_signed = (int32_t) gpr[rs1];

    if(funct7 == 32){
        switch (funct3){
            case 0:
                display_pc_instruction("sub");
                gpr[rd] = gpr[rs1] - gpr[rs2];
                break;
            case 5:
                display_pc_instruction("sra");
                gpr[rd] = (uint32_t)(rs1_signed >> gpr[shamt]);
                break;
        }
    } else if (funct7 == 0) {
        switch(funct3){
            case 0:
                display_pc_instruction("add");
                gpr[rd] = gpr[rs1] + gpr[rs2];
                break;
            case 1:
                display_pc_instruction("sll");
                gpr[rd] = gpr[rs1] << gpr[shamt];
                break;
            case 2:
                display_pc_instruction("slt");
                gpr[rd] = ((int32_t)gpr[rs1] < (int32_t)gpr[rs2])? 1 : 0;
                break;
            case 3:
                display_pc_instruction("sltu");
                gpr[rd] = (gpr[rs1] < gpr[rs2]) ? 1 : 0;
                break;
            case 4:
                display_pc_instruction("xor");
                gpr[rd] = gpr[rs1] ^ gpr[rs2];
                break;
            case 5:
                display_pc_instruction("srl");
                gpr[rd] = gpr[rs1] >> gpr[shamt];
                break;
            case 6:
                display_pc_instruction("or");
                gpr[rd] = gpr[rs1] | gpr[rs2];
                break;
            case 7:
                display_pc_instruction("and");
                gpr[rd] = gpr[rs1] & gpr[rs2];
                break;
        }
    }
}
// End of Author: Satyajit Deokar

// Start of Author: Neil Austin Tauro
void jumpAndLink(){
    uint32_t rd = RD(instruction);
    uint32_t immediate = JAL_IMMEDIATE(instruction);
	
	display_pc_instruction("jal");
	gpr[rd] = pc + 4;
	pc = pc + immediate;
}

void jumpAndLinkReg(){
    uint32_t rs1 = RS1(instruction);
    uint32_t rd = RD(instruction);
    uint32_t immediate = IMMEDIATE(instruction);
	
	display_pc_instruction("jalr");
	gpr[rd] = pc + 4;
	pc = (gpr[rs1] + immediate) & 0xFFFFFFFE;
}

void conditionalBranch(){
    uint32_t funct3 = FUNCT3(instruction);
    uint32_t rs1 = RS1(instruction);
    uint32_t rs2 = RS2(instruction);
	uint32_t immediate = IMMEDIATE_BRANCH(instruction);
	
	switch(funct3)
	{
		case 0: {	
			display_pc_instruction("beq");
            pc = (gpr[rs1] == gpr[rs2]) ? (pc + immediate) : (pc + 4);
			break;
		}	
		case 1: {	
			display_pc_instruction("bne");
			pc = (gpr[rs1] != gpr[rs2]) ? (pc + immediate) : (pc + 4);
		  break;
		}
		case 4: {
			display_pc_instruction("blt");
			pc = ((int32_t)gpr[rs1] < (int32_t)gpr[rs2]) ? (pc + immediate) : (pc + 4);
			break;
		}
		case 5: {
			display_pc_instruction("bge");
			pc = ((int32_t)gpr[rs1] >= (int32_t)gpr[rs2]) ? (pc + immediate) : (pc + 4);
			break;
		}
		case 6: {
			display_pc_instruction("bltu");
            pc = (gpr[rs1] < gpr[rs2]) ? (pc + immediate) : (pc + 4);
			break;
		}
		case 7: {
			display_pc_instruction("bgeu");
            pc = (gpr[rs1] >= gpr[rs2]) ? (pc + immediate) : (pc + 4);
			break;
		}
	}
}
// End of Author: Neil Austin Tauro

// Start of Author: Sanjeev Krishnan
void lui() {
    uint32_t rd = UPPER_IMMEDIATE(instruction);
    display_pc_instruction("lui");
    gpr[rd] = instruction & 0xFFFFFF000;
}

void auipc() {
    uint32_t rd = UPPER_IMMEDIATE(instruction);
    display_pc_instruction("auipc");
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
    SP = argc > 3 ? atoi(argv[3]) : MEMORY_SIZE;

    while (fscanf(memFile, "%x: %x", &address, &instruction) == 2) {
        if (address + 3 >= MEMORY_SIZE) {
            printf("Error: Address out of bounds (0x%hx)\n", address);
            continue;
        }
        writeMem(address, LW, instruction);
    }

    fclose(memFile);

    while (1) {
        instruction = readMem(pc, LW);
        if (!instruction) {
            break;
        } else {
            opcode = instruction & 0x7F;
            switch (opcode) {
                case 0x23:
                    store();
                    pc += PC_INCREMENT;
                    break;
                case 0x3:
                    load();
                    pc += PC_INCREMENT;
                    break;
                case 0x13:
                    logical();
                    pc += PC_INCREMENT;
                    break;
                case 0x37:
                    lui();
                    pc += PC_INCREMENT;
                    break;
                case 0x17:
                    auipc();
                    pc += PC_INCREMENT;
                    break;
                case 0x33:
                    arithmetic();
                    pc += PC_INCREMENT;
                    break;
                case 0x6f:
                    jumpAndLink();
                    break;
                case 0x67:
                    jumpAndLinkReg();
                    break;
                case 0x63:
                    conditionalBranch();
                    break;
                default:
                    pc += PC_INCREMENT;
                    break;
            }
            displayRegisterFile();
        }
    }

    return 0;
}
// End of Author: Sanjeev krishnan