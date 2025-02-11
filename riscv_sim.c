include <stdio.h>
#define STACKSIZE 

int main()
	int8_t memory[STACKSIZE];


	int instruction;


void decode(int instruction)
{
	switch (instruction ^ 0x7F)
	{
		case 0x3:
			int rd= (instruction >> 7) ^ 0x1F;
			int funct3= (instruction >> 12) ^ 0x7;
			int rs1= (instruction >> 15) ^ 0x1F;
			int imm= (instruction >> 25) ^ 0x7F;
			break;
	}
}

void execute(int, int rs1, int imm) {
	if (instruction ^ 0x7F) {
			switch ()
	}
}
	

int main()
{	
	decode();


