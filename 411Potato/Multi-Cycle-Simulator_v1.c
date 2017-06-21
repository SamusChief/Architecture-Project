// Daniel Franchy (franchy1@umbc.edu)
// Dylan Chu
// Tristan Adams
// Project Simulator - Multi-Cycle

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <string.h>
#define NUM_REGISTERS 32
#define MEMORY 1024
#define CLOCK_SPEED 520


int getState(int IR) {
    //fprintf(stdout, "Instruction: 0x%08x\n", IR & 0xFC000000);
    //fprintf(stdout, "Primary: %u\n", (IR & 0xFC000000) >> 26);
    switch ((IR & 0xFC000000) >> 26) {
        case 0: // R-Type
            //fprintf(stdout, "R-Type: %u\n", IR & 0x0000003F);
            switch (IR & 0x0000003F) {
                case 8: // jr
                    return 4;
                case 9: // jalr
                    return 5;
                case 32: // add
                    return 6;
                case 33: // addu
                    return 7;
                case 34: // sub
                    return 10;
                case 35: // subu
                    return 12;
                case 36: // and
                    return 14;
                case 37: // or
                    return 16;
                case 38: // xor
                    return 18;
                case 39: // nor
                    return 20;
                default:
                    return 0; // Invalid
            }
        case 2: // j
            return 2;
        case 3: // jal
            return 3;
        case 4: // beq
            return 31;
        case 5: // bne
            return 33;
        case 6: // blez
            return 35;
        case 7: // bgtz
            return 37;
        case 8: // addi
            return 22;
        case 9: // addiu
            return 0; // TODO: addiu Not in RTL for some reason tristan
        case 12: // andi
            return 24;
        case 13: // ori
            return 26;
        case 14: // xori
            return 28;
        case 15: // lui
            return 30;
        case 19: // C-Type
            //fprintf(stdout, "C-Type: %u\n", (IR & 0x03E00000) >> 21);
            switch ((IR & 0x03E00000) >> 21) {
                case 0: // spd
                    return 43;
                case 1: // Rot
                    return 45;
                case 2: // Ascn
                    return 49;
                case 3: // Dscn
                    return 51;
                case 4: // Fir
                    return 47;
                default:
                    return 0; // Invalid
            }
        case 35: // lw
            return 39;
        case 43: // sw
            return 41;
        default:
            return 0; // Invalid
    }
}
int main(int argc, char *argv[]) {
    char line[254];
	fprintf(stdout, "%u\n", CLOCK_SPEED);

    // Initiate system variables
	unsigned int IMem[MEMORY] = {
        #include "code.h"
	};
	int PC = 0;
    int NPC = 0;
    int R[NUM_REGISTERS];
    int IR;  // Reset every "Read instruction" state
    int R1;  // Reset every "Decode instruction" state
    int R2;  // Reset every "Decode instruction" state
    int IMM; // Reset every "Decode instruction" state
    int X;   // Reset every "Decode instruction" state
    int state = 0;
    while (true) { // Cycle
        fgets(line, sizeof(line), stdin);
        //fprintf(stdout, "State: %u\n", state);
        switch (state) {
            case 0:
                // force register 0 to always be 0
                R[0] = 0;
                // 0,IF,IR = IMem[PC]; NPC = PC + 4; next = 1
                IR = IMem[(PC / 4)]; // Divide by word size
                NPC = PC + 4;
                fprintf(stdout, "N\n");
                state = 1;
                continue;
            case 1:
                // 1,ID,"R1 = R[IR(25-21)], R2 = R[IR(20-16)]; IMM = signExt(IR(15-0)); next = decode(IR).state"
                R1 = R[(IR & 0x03E00000) >> 21];
                R2 = R[(IR & 0x001F0000) >> 16];
                IMM = (IR & 0x0000FFFF);
                if ((IMM & 0x00008000) != 0) { // Sign extend
                    IMM |= 0xFFFF0000;
                }
                X = 0;
                fprintf(stdout, "N\n");
                state = getState(IR);
                continue;
            case 2:
                // 2,j0,PC = IR(25-0) << 2; next = 0
                PC = (IR & 0x03FFFFFF) << 2;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 3:
                // 3,jal0,PC = IR(25-0) << 2; R[31] = NPC; next = 0
                PC = (IR & 0x03FFFFFF) << 2;
                R[31] = NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 4:
                // 4,jr0,PC = R1; next = 0
                PC = R1;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 5:
                // 5,jalr0,PC = R1; R[31] = NPC; next = 0
                PC = R1;
                R[31] = NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 6:
                // 6,add0,X = R1 + R2; PC = NPC; next = 7
                X = R1 + R2;
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 7;
                continue;
            case 7:
                // 7,add1,R[IR(15-11)] = X; next = 0
                R[(IR & 0x0000F800) >> 11] = X;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 8:
                // 8,addu0,X = R1 + R2; PC = NPC; next = 9
                X = R1 + R2;
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 9;
                continue;
            case 9:
                // 9,addu1,R[IR(15-11)] = X; next = 0
                R[(IR & 0x0000F800) >> 11] = X;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 10:
                // 10,sub0,X = R1 - R2; PC = NPC; next = 11
                X = R1 - R2;
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 11;
                continue;
            case 11:
                // 11,sub1,R[IR(15-11)] = X; next = 0
                R[(IR & 0x0000F800) >> 11] = X;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 12:
                // 12,subu0,X = R1 - R2; PC = NPC; next = 13
                X = R1 - R2;
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 13;
                continue;
            case 13:
                // 13,subu1,R[IR(15-11)] = X; next = 0
                R[(IR & 0x0000F800) >> 11] = X;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 14:
                // 14,and0,X = R1 & R2; PC = NPC; next = 15
                X = R1 & R2;
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 15;
                continue;
            case 15:
                // 15,and1,R[IR(15-11)] = X; next = 0
                R[(IR & 0x0000F800) >> 11] = X;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 16:
                // 16,or0,X = R1 | R2; PC = NPC; next = 17
                X = R1 | R2;
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 17;
                continue;
            case 17:
                // 17,or1,R[IR(15-11)] = X; next = 0
                R[(IR & 0x0000F800) >> 11] = X;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 18:
                // 18,xor0,X = R1 ^ R2; PC = NPC; next = 19
                X = R1 ^ R2;
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 19;
                continue;
            case 19:
                // 19,xor1,R[IR(15-11)] = X; next = 0
                R[(IR & 0x0000F800) >> 11] = X;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 20:
                // 20,nor0,X = ~(R1 | R2); PC = NPC; next = 21
                X = ~(R1 | R2);
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 21;
                continue;
            case 21:
                // 21,nor1,R[IR(15-11)] = X; next = 0
                R[(IR & 0x0000F800) >> 11] = X;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 22:
                // 22,addi0,X = R1 + IMM; PC = NPC; next = 23
                X = R1 + IMM;
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 23;
                continue;
            case 23:
                // 23,addi1,R[IR(20-16)] = X; next = 0
                R[(IR & 0x001F0000) >> 16] = X;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 24:
                // 24,andi0,X = R1 & IMM; PC = NPC; next = 25
                X = R1 & IMM;
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 25;
                continue;
            case 25:
                // 25,andi1,R[IR(20-16)] = X; next = 0
                R[(IR & 0x001F0000) >> 16] = X;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 26:
                // 26,ori0,X = R1 | IMM; PC = NPC; next = 27
                X = R1 | IMM;
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 27;
                continue;
            case 27:
                // 27,ori1,R[IR(20-16)] = X; next = 0
                R[(IR & 0x001F0000) >> 16] = X;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 28:
                // 28,xori0,X = R1 ^ IMM; PC = NPC; next = 29
                X = R1 ^ IMM;
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 29;
                continue;
            case 29:
                // 29,xori1,R[IR(20-16)] = X; next = 0
                R[(IR & 0x001F0000) >> 16] = X;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 30:
                // 30,lui0,R[IR(20-16)] = IMM << 16; PC = NPC; next = 0;
                R[(IR & 0x001F0000) >> 16] = IMM << 16;
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 31:
                // 31,beq0,X = R1 - R2; next = 32
                X = R1 - R2;
                fprintf(stdout, "N\n");
                state = 32;
                continue;
            case 32:
                // 32,beq1,PC = (X == 0) ? NPC + imm<<2 : NPC; next = 0;
                PC = (X == 0) ? (NPC + (IMM << 2)) : NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 33:
                // 33,bne0,X = R1 - R2; next = 34
                X = R1 - R2;
                fprintf(stdout, "N\n");
                state = 34;
                continue;
            case 34:
                // 34,bne1,PC = (X != 0) ? NPC + imm<<2 : NPC; next = 0;
                PC = (X != 0) ? (NPC + (IMM << 2)) : NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 35:
                // 35,blez0,X = R1; next = 36
                X = R1;
                fprintf(stdout, "N\n");
                state = 36;
                continue;
            case 36:
                // 36,blez1,PC = (X <= 0) ? NPC + imm<<2 : NPC; next = 0;
                PC = (X <= 0) ? (NPC + (IMM << 2)) : NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 37:
                // 37,bgez0,X = R1; next = 38
                X = R1;
                fprintf(stdout, "N\n");
                state = 38;
                continue;
            case 38:
                // 38,bgez1,PC = (X >= 0) ? NPC + imm<<2 : NPC; next = 0;
                PC = (X >= 0) ? (NPC + (IMM << 2)) : NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 39:
                // 39,lw0,X = IMM + R1; next = 40
                X = IMM + R1;
                fprintf(stdout, "N\n");
                state = 40;
                continue;
            case 40:
                // 40,lw1,R[IR(20-16)] = IMem[X]; PC = NPC; next = 0
                R[(IR & 0x001F0000) >> 16] = IMem[X];
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 41:
                // 41,sw0,X = IMM + R1; next = 42
                X = IMM + R1;
                fprintf(stdout, "N\n");
                state = 42;
                continue;
            case 42:
                // 42,sw1,IMem[X] = R[IR(20-16)]; PC = NPC; next = 0
                IMem[X] = R[(IR & 0x001F0000) >> 16];
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 43:
                // 43,spd0,X = R2 + IMM; next = 44
                X = R2 + IMM;
                fprintf(stdout, "N\n");
                state = 44;
                continue;
            case 44:
                // 44,spd1,SPEED = X; PC = NPC; next = 0
                fprintf(stdout, "M %i\n", X);
                PC = NPC;
                state = 0;
                continue;
            case 45:
                // 45,rot0,X = R2 + IMM; next = 46
                X = R2 + IMM;
                fprintf(stdout, "N\n");
                state = 46;
                continue;
            case 46:
                // 46,rot1,ROT_RATE = X; PC = NPC; next = 0
                fprintf(stdout, "R %i\n", X);
                PC = NPC;
                state = 0;
                continue;
            case 47:
                // 47,fir0,X = R2 + IMM; next = 48
                X = R2 + IMM;
                fprintf(stdout, "N\n");
                state = 48;
                continue;
            case 48:
                // 48,fir1,FIRE = X; PC = NPC; next = 0
                fprintf(stdout, "F %i\n", X);
                PC = NPC;
                state = 0;
                continue;
            case 49:
                // 49,ascn0,RobotFunc=A; next = 50
                fprintf(stdout, "A\n");
                state = 50;
                continue;
            case 50:
                // 50,ascn1,R[IR(20-16)] = ANGLE_SCAN(); PC = NPC; next = 0
                R[(IR & 0x001F0000) >> 16] = atoi(line);
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 51:
                // 51,dscn0,RobotFunc=D; next = 52
                fprintf(stdout, "D\n");
                state = 50;
                continue;
            case 52:
                // 52,dscn1,R[IR(20-16)] = DIST_SCAN(); PC = NPC; next = 0
                R[(IR & 0x001F0000) >> 16] = atoi(line);
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            default:
                continue;
        }
    }
}
