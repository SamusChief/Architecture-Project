// Daniel Franchy (franchy1@umbc.edu)
// Dylan Chu
// Tristan Adams
// Project Simulator

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <string.h>
#define NUM_REGISTERS 32
#define MEMORY 1024

struct System {
    int mem[MEMORY];
    int reg[NUM_REGISTERS];
    int PC;
};
struct Functions {
    std::map<int, void (*)(int, System&, Functions)> opcodes;
    std::map<int, void (*)(int, System&, Functions)> r; // R-Types have subfunctions
    std::map<int, void (*)(int, System&, Functions)> c; // C-Types have subfunctions
};

// Delegate R-Type operations
void rTypes(int operation, System &system, Functions functions) {
    int opcode = operation & 0x0000003F;
    if (functions.r.find(opcode) != functions.r.end()) { // Only attempt to run known operations
        functions.r[opcode](operation, system, functions);
    }
}

void j(int operation, System &system, Functions functions) {
    // Implement jump operation
    int imm = operation & 0x03FFFFFF;
    system.PC = imm;
    system.PC -= 1; // This counteracts the auto increment in main loop
}

void jal(int operation, System &system, Functions functions) {
    // Implement jump and link operation
    int imm = operation & 0x03FFFFFF;
    system.reg[31] = system.PC + 1;
    system.PC = imm;
    system.PC -= 1; // This counteracts the auto increment in main loop
}

void beq(int operation, System &system, Functions functions) {
    // Implement beq operation
    int rs = (operation & 0x03E00000) >> 21;
    int rt = (operation & 0x001F0000) >> 16;
    short imm = (operation & 0x0000FFFF);
    if (system.reg[rt] == system.reg[rs]) {
        system.PC += imm;
        // The main loop auto increment takes care of the offset being from the next instruction
    }
}

void bne(int operation, System &system, Functions functions) {
    // Implement bne operation
    int rs = (operation & 0x03E00000) >> 21;
    int rt = (operation & 0x001F0000) >> 16;
    short imm = (operation & 0x0000FFFF);
    if (system.reg[rt] != system.reg[rs]) {
        system.PC += imm;
        // the main loop auto increment takes care of the offset being from the next instruction
    }
}

void blez(int operation, System &system, Functions functions) {
    // implement blez operation
    int rs = (operation & 0x03E00000) >> 21;
    short imm = (operation & 0x0000FFFF);
    if (system.reg[rs] <= 0) {
        system.PC += imm;  
        // the main loop auto increment takes care of the offset being from the next instruction
    }
}

void bgtz(int operation, System &system, Functions functions) {
    // implement bgtz operation
    int rs = (operation & 0x03E00000) >> 21;
    short imm = (operation & 0x0000FFFF);
    if (system.reg[rs] > 0) {
        system.PC += imm;
	    // the main loop auto increment takes care of the offset being from the next instruction
    }
}

void addi(int operation, System &system, Functions functions) {
    // Implement addi function
    int rs = (operation & 0x03E00000) >> 21;
    int rt = (operation & 0x001F0000) >> 16;
    short imm = (operation & 0x0000FFFF);
    if ((imm & 0x00008000) != 0) {
        imm |= 0xFFFF0000;
    }
    system.reg[rt] = system.reg[rs] + imm;
}

void addiu(int operation, System &system, Functions functions) {
    // Implement addi function
    int rs = (operation & 0x03E00000) >> 21;
    int rt = (operation & 0x001F0000) >> 16;
    int imm = (operation & 0x0000FFFF); /* int not short to not sign extend */
    system.reg[rt] = system.reg[rs] + imm;
}

void andi(int operation, System &system, Functions functions) {
    // Implement andi function
    int rs = (operation & 0x03E00000) >> 21;
    int rt = (operation & 0x001F0000) >> 16;
    int imm = (operation & 0x0000FFFF);
    if ((imm & 0x00008000) != 0) {
        imm |= 0xFFFF0000;
    }
    system.reg[rt] = system.reg[rs] & imm;
}

void ori(int operation, System &system, Functions functions) {
    // Implement ori function
    int rs = (operation & 0x03E00000) >> 21;
    int rt = (operation & 0x001F0000) >> 16;
    int imm = (operation & 0x0000FFFF);
    system.reg[rt] = system.reg[rs] | imm;
}

void xori(int operation, System &system, Functions functions) {
    // Implement xori function
    int rs = (operation & 0x03E00000) >> 21;
    int rt = (operation & 0x001F0000) >> 16;
    int imm = (operation & 0x0000FFFF);
    system.reg[rt] = system.reg[rs] ^ imm;
}

void lui(int operation, System &system, Functions functions){
    // Implement lui 
    int rt = (operation & 0x001F0000) >> 16;
    int imm = (operation & 0x0000FFFF);
    int upper = imm << 16;
    system.reg[rt] = upper;
}

// Delegate C-Type operations
void cTypes(int operation, System &system, Functions functions) {
    int opcode = (operation & 0x03E00000) >> 21;
    if (functions.c.find(opcode) != functions.c.end()) { // Only attempt to run known operations
        functions.c[opcode](operation, system, functions);
    }
}

void lw(int operation, System &system, Functions functions) {
    // Implement lw operation
    int rs = (operation & 0x03E00000) >> 21;
    int rt = (operation & 0x001F0000) >> 16;
    short imm = (operation & 0x0000FFFF);
    int address = imm / 4;
    address += system.reg[rs];
    system.reg[rt] = system.mem[address];
}

void sw(int operation, System &system, Functions functions) {
    // Implement lw operation
    int rs = (operation & 0x03E00000) >> 21;
    int rt = (operation & 0x001F0000) >> 16;
    short imm = (operation & 0x0000FFFF);
    int address = imm / 4;
    address += system.reg[rs];
    system.mem[address] = system.reg[rt];
}

void jr(int operation, System &system, Functions functions) {
    // implement jr operation
    int rs = (operation & 0x03E00000) >> 21;
    system.PC = system.reg[rs];
    system.PC -= 1; //this counteracts the incrementer in the main loop
}

void jalr(int operation, System &system, Functions functions) {
    // implement jalr operation
    int rs = (operation & 0x03E00000) >> 21;
    int rd = (operation & 0x0000F800) >> 11;
	int target = system.reg[rs];
    system.reg[rd] = system.PC + 1;
    system.PC = target;
    system.PC -= 1; //the counteracts the incrementer in the main loop
}
void add(int operation, System &system, Functions functions) {
    // Implement add function
    int rs = (operation & 0x03E00000) >> 21;
    int rt = (operation & 0x001F0000) >> 16;
    int rd = (operation & 0x0000F800) >> 11;
    int sh = (operation & 0x000007C0) >> 6;
    system.reg[rd] = system.reg[rs] + system.reg[rt];
}

void addu(int operation, System &system, Functions functions) {
    // Implement addu function
    int rs = (operation & 0x03E00000) >> 21;
    int rt = (operation & 0x001F0000) >> 16;
    int rd = (operation & 0x0000F800) >> 11;
    int sh = (operation & 0x000007C0) >> 6;
    system.reg[rd] = system.reg[rs] + system.reg[rt];
}

void sub(int operation, System &system, Functions functions) {
    // Implement sub function
    int rs = (operation & 0x03E00000) >> 21;
    int rt = (operation & 0x001F0000) >> 16;
    int rd = (operation & 0x0000F800) >> 11;
    int sh = (operation & 0x000007C0) >> 6;
    system.reg[rd] = system.reg[rs] - system.reg[rt];
}

void subu(int operation, System &system, Functions functions) {
    // Implement subu function
    int rs = (operation & 0x03E00000) >> 21;
    int rt = (operation & 0x001F0000) >> 16;
    int rd = (operation & 0x0000F800) >> 11;
    int sh = (operation & 0x000007C0) >> 6;
    system.reg[rd] = system.reg[rs] - system.reg[rt];
}

void andfunc(int operation, System &system, Functions functions) { // "and" is a reserved c keyword
    // implement and operation
    int rs = (operation & 0x03E00000) >> 21;
    int rt = (operation & 0x001F0000) >> 16;
    int rd = (operation & 0x0000F800) >> 11;
    int sh = (operation & 0x000007C0) >> 6;
    system.reg[rd] = system.reg[rs] & system.reg[rt];
}

void orfunc(int operation, System &system, Functions functions) { // "or" is a reserved c keyword
    // implement or operation
    int rs = (operation & 0x03E00000) >> 21;
    int rt = (operation & 0x001F0000) >> 16;
    int rd = (operation & 0x0000F800) >> 11;
    int sh = (operation & 0x000007C0) >> 6;
    system.reg[rd] = system.reg[rs] | system.reg[rt];
}

void xorfunc(int operation, System &system, Functions functions) { // "xor" is a reserved c keyword
    // implement xor operation
    int rs = (operation & 0x03E00000) >> 21;
    int rt = (operation & 0x001F0000) >> 16;
    int rd = (operation & 0x0000F800) >> 11;
    int sh = (operation & 0x000007C0) >> 6;
    system.reg[rd] = system.reg[rs] ^ system.reg[rt];
}

void nor(int operation, System &system, Functions functions) {
    // implement nor operation
    int rs = (operation & 0x03E00000) >> 21;
    int rt = (operation & 0x001F0000) >> 16;
    int rd = (operation & 0x0000F800) >> 11;
    int sh = (operation & 0x000007C0) >> 6;
    system.reg[rd] = ~(system.reg[rs] | system.reg[rt]);
}

void spd(int operation, System &system, Functions functions) {
    int rd = (operation & 0x001F0000) >> 16;
    short imm = (operation & 0x0000FFFF);
    int spd = system.reg[rd] + imm;
    fprintf(stdout, "M %d\n", spd);
}

void rot(int operation, System &system, Functions functions) {
    int rd = (operation & 0x001F0000) >> 16;
    short imm = (operation & 0x0000FFFF);
    int rot = system.reg[rd] + imm;
    fprintf(stdout, "R %d\n", rot);
}

void ascn(int operation, System &system, Functions functions) {
    int rd = (operation & 0x001F0000) >> 16;
    fprintf(stdout, "A\n");
    char line[254];
    fgets(line, sizeof(line), stdin);
    int scan;
    sscanf(line, "%d", &scan);
    system.reg[rd] = scan;
}

void dscn(int operation, System &system, Functions functions) {
    int rd = (operation & 0x001F0000) >> 16;
    fprintf(stdout, "D\n");
    char line[254];
    fgets(line, sizeof(line), stdin);
    int scan;
    sscanf(line, "%d", &scan);
    system.reg[rd] = scan;
}

void fir(int operation, System &system, Functions functions) {
    int rd = (operation & 0x001F0000) >> 16;
    short imm = (operation & 0x0000FFFF);
    int fir = system.reg[rd] + imm;
    fprintf(stdout, "F %d\n", fir);
}

void initFunctionMaps(Functions &functions) {
    // Add our functions to our function maps
    // opcode functions
    functions.opcodes.insert(std::make_pair(0, &rTypes));
    functions.opcodes.insert(std::make_pair(2, &j));
    functions.opcodes.insert(std::make_pair(3, &jal));
    functions.opcodes.insert(std::make_pair(4, &beq));
    functions.opcodes.insert(std::make_pair(5, &bne));
    functions.opcodes.insert(std::make_pair(6, &blez));
    functions.opcodes.insert(std::make_pair(7, &bgtz));
    functions.opcodes.insert(std::make_pair(8, &addi));
    functions.opcodes.insert(std::make_pair(9, &addiu));
    functions.opcodes.insert(std::make_pair(12, &andi));
    functions.opcodes.insert(std::make_pair(13, &ori));
    functions.opcodes.insert(std::make_pair(14, &xori));
    functions.opcodes.insert(std::make_pair(15, &lui));
    functions.opcodes.insert(std::make_pair(19, &cTypes));
    functions.opcodes.insert(std::make_pair(35, &lw));
    functions.opcodes.insert(std::make_pair(43, &sw));
    
    // R-Type functions
    functions.r.insert(std::make_pair(8, &jr));
    functions.r.insert(std::make_pair(9, &jalr));
    functions.r.insert(std::make_pair(32, &add));
    functions.r.insert(std::make_pair(33, &addu));
    functions.r.insert(std::make_pair(34, &sub));
    functions.r.insert(std::make_pair(35, &subu));
    functions.r.insert(std::make_pair(36, &andfunc));
    functions.r.insert(std::make_pair(37, &orfunc));
    functions.r.insert(std::make_pair(38, &xorfunc));
    functions.r.insert(std::make_pair(39, &nor));

    // C-Type functions
    functions.c.insert(std::make_pair(0, &spd));
    functions.c.insert(std::make_pair(1, &rot));
    functions.c.insert(std::make_pair(2, &ascn));
    functions.c.insert(std::make_pair(3, &dscn));
    functions.c.insert(std::make_pair(4, &fir));
}

int main(int argc, char *argv[]) {
    Functions functions;
    initFunctionMaps(functions);

    System system;
	unsigned int uMem[MEMORY] = {
        #include "code.h"
	};
    memcpy(system.mem, uMem, sizeof(uMem));
	system.PC = 0;

    char line[254];
    while (system.PC < MEMORY) {
        fgets(line, sizeof(line), stdin);

		// force register 0 to always be 0
		system.reg[0] = 0;

        int operation = system.mem[system.PC];
        int opcode = (operation & 0xFC000000) >> 26; // Mask and shift
        if (functions.opcodes.find(opcode) != functions.opcodes.end()) { // Only attempt to run known operations
            functions.opcodes[opcode](operation, system, functions);
        }
        if (opcode != 19) {
            fprintf(stdout, "N\n"); // No robot command
        }
        system.PC++;
    }
}
