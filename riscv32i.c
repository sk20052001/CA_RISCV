#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "defines.c"

FILE *memFile;

uint8_t memory[MEMORY_SIZE];
uint32_t address, instruction, pc, gpr[32] = {0}, opcode, prevInstruction, prevPC;
char prevInstructionType[10];

// Start of Author: Sanjeev Krishnan
void display_pc_instruction(char *instructionType) {
    #ifdef VERBOSE
        printf("PC: %08x\n", pc);
        printf("Instruction type: %s\tInstruction: %08x\n\n", instructionType, instruction);
    #endif
    strcpy(prevInstructionType, instructionType);
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
        case BYTE:
            memory[byteAddress] = data;
            break;
        case HALF_WORD:
            memory[byteAddress] = data;
            memory[byteAddress + 1] = data >> 8;
            break;
        case WORD:
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
        case BYTE:
            return memory[byteAddress] & 0xFF;
            break;
        case HALF_WORD:
            return (memory[byteAddress] | (memory[byteAddress + 1] << 8)) & 0xFFFF;
            break;
        case WORD:
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
            writeMem(byteAddress, BYTE, gpr[rs2]);
            break;
        case 1:
            display_pc_instruction("sh");
            writeMem(byteAddress, HALF_WORD, gpr[rs2]);
            break;
        case 2:
            display_pc_instruction("sw");
            writeMem(byteAddress, WORD, gpr[rs2]);
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
            loadedValue = readMem(byteAddress, BYTE);
            gpr[rd] = rd ? ((loadedValue & 0x80) ? (loadedValue | 0xFFFFFF00) : loadedValue) : 0;
            break;
        case 1:
            display_pc_instruction("lh");
            loadedValue = readMem(byteAddress, HALF_WORD);
            gpr[rd] = rd ? ((loadedValue & 0x8000) ? (loadedValue | 0xFFFF0000) : loadedValue) : 0;
            break;
        case 2:
            display_pc_instruction("lw");
            gpr[rd] = rd ? (readMem(byteAddress, WORD)) : 0;
            break;
        case 4:
            display_pc_instruction("lbu");
            gpr[rd] = rd ? (readMem(byteAddress, BYTE)) : 0;
            break;
        case 5:
            display_pc_instruction("lhu");
            gpr[rd] = rd ? (readMem(byteAddress, HALF_WORD)) : 0;
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
            gpr[rd] = rd ? (gpr[rs1] + immediate) : 0;
            break;
        case 1:
            display_pc_instruction("slli");
            gpr[rd] = rd ? (gpr[rs1] << shamt) : 0;
            break;
        case 2:
            display_pc_instruction("slti");
            gpr[rd] = rd ? (((int32_t)gpr[rs1] < (int32_t)immediate)) : 0;
            break;
        case 3:
            display_pc_instruction("sltiu");
            gpr[rd] = rd ? ((gpr[rs1] < immediate)) : 0;
            break;
        case 4:
            display_pc_instruction("xori");
            gpr[rd] = rd ? (gpr[rs1] ^ immediate) : 0;
            break;
        case 5:
            if (!funct7) {
                display_pc_instruction("srli");
                gpr[rd] = rd ? (gpr[rs1] >> shamt) : 0;
                break;
            } else {
                display_pc_instruction("srai");
                gpr[rd] = rd ? ((gpr[rs1] & 0x80000000) ? (gpr[rs1] >> shamt | ~(0xFFFFFFFF >> shamt)) : gpr[rs1] >> shamt) : 0;
                break;
            }
        case 6:
            display_pc_instruction("ori");
            gpr[rd] = rd ? (gpr[rs1] | immediate) : 0;
            break;
        case 7:
            display_pc_instruction("andi");
            gpr[rd] = rd ? (gpr[rs1] & immediate) : 0;
            break;
        default:
            break;
    }
}

void ecall() {
    switch (gpr[A7]) { 
        case 63:                // read ecall
            display_pc_instruction("System Call - Read");
            gpr[A0] = read(0, &memory[gpr[A1]], gpr[A2]);
            break;

        case 64:                    // write ecall
            display_pc_instruction("System Call - Write");
            gpr[A0] = write(1, &memory[gpr[A1]], gpr[A2]);
            break;

        case 94:                    // exit ecall
            display_pc_instruction("System Call - Exit");
            exit(gpr[A0]);
            break;

        default:
            printf("Unknown System Call (ecall): %d\n", gpr[A7]);
            exit(1);
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
                gpr[rd] = rd ? (gpr[rs1] - gpr[rs2]) : 0;
                break;
            case 5:
                display_pc_instruction("sra");
                gpr[rd] = rd ? ((uint32_t)(rs1_signed >> gpr[shamt])) : 0;
                break;
            }
    } else if (funct7 == 0) {
        switch(funct3){
            case 0:
                display_pc_instruction("add");
                gpr[rd] = rd ? (gpr[rs1] + gpr[rs2]) : 0;
                break;
            case 1:
                display_pc_instruction("sll");
                gpr[rd] = rd ? (gpr[rs1] << gpr[shamt]) : 0;
                break;
            case 2:
                display_pc_instruction("slt");
                gpr[rd] = rd ? (((int32_t)gpr[rs1] < (int32_t)gpr[rs2])? 1 : 0) : 0;
                break;
            case 3:
                display_pc_instruction("sltu");
                gpr[rd] = rd ? ((gpr[rs1] < gpr[rs2]) ? 1 : 0) : 0;
                break;
            case 4:
                display_pc_instruction("xor");
                gpr[rd] = rd ? (gpr[rs1] ^ gpr[rs2]) : 0;
                break;
            case 5:
                display_pc_instruction("srl");
                gpr[rd] = rd ? (gpr[rs1] >> gpr[shamt]) : 0;
                break;
            case 6:
                display_pc_instruction("or");
                gpr[rd] = rd ? (gpr[rs1] | gpr[rs2]) : 0;
                break;
            case 7:
                display_pc_instruction("and");
                gpr[rd] = rd ? (gpr[rs1] & gpr[rs2]) : 0;
                break;
            }
    } else if (funct7 == 1){
        switch(funct3){
            case 0:
                display_pc_instruction("mul");
                gpr[rd] = rd ? (gpr[rs1] * gpr[rs2]) : 0; 
                break; //mul
            case 1://mulh
                display_pc_instruction("mulh");
                // int32_t rs1 = 
                int64_t s_rs1 = (int64_t) gpr[rs1];
                int64_t s_rs2 = (int64_t) gpr[rs2];
                int64_t result = s_rs1 * s_rs2;
                gpr[rd] = rd ? ((int32_t)(result >> 32)) : 0; 
                break; 
            case 2: //mulhsu Multiply High Signed/Unsigned
                display_pc_instruction("mulhsu");
                uint64_t unsigned_rs1 = (uint64_t) gpr[rs1];
                uint64_t unsigned_rs2 = (uint64_t) gpr[rs2];
                uint64_t m64 = unsigned_rs1 * unsigned_rs2;    
                uint64_t m64rsh = (m64 >> 32);
                gpr[rd] = rd ? ((uint32_t) m64rsh) : 0;
                break;
            case 3: //mulhu
                display_pc_instruction("mulhu");
                uint64_t u_rs1 = (uint64_t) gpr[rs1];
                uint64_t u_rs2 = (uint64_t) gpr[rs2];
                // Left shift rs1 by 32 bits to prepare for signed extension
                int64_t leftshift_rs1 = u_rs1 << 32;
                // Perform signed extension
                int64_t signedtemp_rs1 = (int64_t) leftshift_rs1;
                int64_t signextend_rs1 = signedtemp_rs1 >> 32;
                // Sign-extend rs1 to 64-bit signed value
                int64_t signed_rs1 = (int64_t) signextend_rs1;
                // Perform the multiplication (signed rs1 * unsigned rs2)
                uint64_t mult64 = signed_rs1 * u_rs2;
                // Extract the upper 32 bits of the result
                uint64_t mult64rsh = (mult64 >> 32);
                // Store the upper 32 bits in the destination register (rd)
                gpr[rd] = rd ? ((uint32_t) mult64rsh) : 0;
                break;
            case 4: //div
                display_pc_instruction("div");
                if ((int32_t) gpr[rs1] == 0x80000000 && (int32_t) gpr[rs2]==0xffffffff) gpr[rd] = 0x80000000;
                    else gpr[rd] = rd ? ((int32_t) gpr[rs2] == 0 ? 0xFFFFFFFF : (int32_t) gpr[rs1] / (int32_t) gpr[rs2]) : 0;
                break;
            case 5: //divu
                display_pc_instruction("divu");
                gpr[rd] = rd ? (rs2==0 ? 0xFFFFFFFF : rs1 / rs2) : 0;
                break;
            case 6: //rem
                if ((int32_t) gpr[rs2] == 0) { 	
                    gpr[rd] = rd ? ((int32_t) gpr[rs1]) : 0;
                } else {
                    gpr[rd] = rd ? ((int32_t) gpr[rs1] % (int32_t) gpr[rs2]) : 0;
                }
                break;
            case 7: //remu
                if (gpr[rs2] == 0) {
                    gpr[rd] = rd ? (gpr[rs1]) : 0;
                } else {
                    gpr[rd] = rd ? (gpr[rs1] % gpr[rs2]) : 0;
                }
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
        gpr[rd] = rd ? (pc + PC_INCREMENT) : 0;
    pc = pc + immediate;
}

void jumpAndLinkReg(){
    uint32_t rs1 = RS1(instruction);
    uint32_t rd = RD(instruction);
    uint32_t immediate = IMMEDIATE(instruction);

    display_pc_instruction("jalr");
    gpr[rd] = rd ? (pc + 4) : 0;
    pc = (gpr[rs1] + immediate) & 0xFFFFFFFE;
}

void conditionalBranch(){
    uint32_t funct3 = FUNCT3(instruction);
    uint32_t rs1 = RS1(instruction);
    uint32_t rs2 = RS2(instruction);
	uint32_t immediate = BRANCH_IMMEDIATE(instruction);
	
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
    uint32_t rd = RD(instruction);
    display_pc_instruction("lui");
    gpr[rd] = rd ? (instruction & 0xFFFFFF000) : 0;
}

void auipc() {
    uint32_t rd = RD(instruction);
    display_pc_instruction("auipc");
    gpr[rd] = rd ? ((instruction & 0xFFFFF000) + pc) : 0;
}

int main(int argc, char *argv[4]) {
    if (argc > 4)
	{
        #ifdef DEBUG
		    printf("Usage: %s [mem_file] [starting address] [stack address]\n", argv[0]);
        #endif
        return 0;
	}
    memFile = argc > 1 ? fopen(argv[1], "r") : fopen("./memory_image/test.mem", "r");
    if (!memFile) {
        #ifdef DEBUG
            perror("Error opening file");
        #endif
        memFile = fopen("./memory_image/test.mem", "r");
        if (!memFile) {
            #ifdef DEBUG
                perror("Error opening default file");
            #endif
            return 0;
        }
    }
    pc = argc > 2 ? atoi(argv[2]) : 0;
    SP = argc > 3 ? atoi(argv[3]) : MEMORY_SIZE;

    while (fscanf(memFile, "%x: %x", &address, &instruction) == 2) {
        if (address + 3 >= MEMORY_SIZE) {
            #ifdef DEBUG
                printf("Error: Address out of bounds (0x%hx)\n", address);
            #endif
            break;
        }
        writeMem(address, WORD, instruction);
    }

    fclose(memFile);

    while (1) {
        prevInstruction = instruction;
        instruction = readMem(pc, WORD);
        if (!instruction) {
            #ifndef VERBOSE
                printf("PC: %08x\n", prevPC);
                printf("Instruction type: %s\tInstruction: %08x\n\n", prevInstructionType, prevInstruction);
                displayRegisterFile();
            #endif
            break;
        }
        prevPC = pc;
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
                case 0x73:
                    ecall();
                    pc += PC_INCREMENT;
                    break;
                default:
                    pc += PC_INCREMENT;
                    break;
            }
            #ifdef VERBOSE
                displayRegisterFile();
            #endif
    }

    return 0;
}
// End of Author: Sanjeev krishnan
