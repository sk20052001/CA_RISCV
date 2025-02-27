#define MEMORY_SIZE 65536
#define PC_INCREMENT 4

#define ZERO gpr[0]
#define RA gpr[1]
#define SP gpr[2]
#define GP gpr[3]
#define TP gpr[4]
#define T0 gpr[5]
#define T1 gpr[6]
#define T2 gpr[7]
#define S0 gpr[8]
#define S1 gpr[9]
#define A0 gpr[10]
#define A1 gpr[11]
#define A2 gpr[12]
#define A3 gpr[13]
#define A4 gpr[14]
#define A5 gpr[15]
#define A6 gpr[16]
#define A7 gpr[17]
#define S2 gpr[18]
#define S3 gpr[19]
#define S4 gpr[20]
#define S5 gpr[21]
#define S6 gpr[22]
#define S7 gpr[23]
#define S8 gpr[24]
#define S9 gpr[25]
#define S10 gpr[26]
#define S11 gpr[27]
#define T3 gpr[28]
#define T4 gpr[29]
#define T5 gpr[30]
#define T6 gpr[31]

#define BYTE 1
#define HALF_WORD 2
#define WORD 4

#define RS1(instruction) ((instruction >> 15) & 0x1F)
#define RS2(instruction) ((instruction >> 20) & 0x1F)
#define RD(instruction) ((instruction >> 7) & 0x1F)
#define FUNCT3(instruction) ((instruction >> 12) & 0x7)
#define FUNCT7(instruction) ((instruction >> 25) & 0x7F)
#define IMMEDIATE(instruction) (instruction & 0x80000000) ? (0xFFFFF000 | ((instruction >> 20) & 0xFFF)) : (instruction >> 20) & 0xFFF
#define STORE_IMMEDIATE(instruction) ((instruction >> 31 ? 0xFFFFF000 : 0) | ((instruction >> 20) & 0xFE0) | ((instruction >> 7) & 0x1F))
#define JAL_IMMEDIATE(instruction) ((instruction >> 31 ? 0xFFFFF000 : 0) | (instruction & 0x000FF000) | ((instruction  & 0x00100000) & 9) | ((instruction & 0x40000000) >> 20))
#define BRANCH_IMMEDIATE(instruction) ((instruction >> 31 ? 0xFFFFF000 : 0) | ((instruction >> 20) & 0xFE0) | ((instruction >> 7) & 0x1F)) | ((instruction << 4) & 0x800)
