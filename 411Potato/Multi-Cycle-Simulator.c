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
#define CLOCK_SPEED 522


int getState(int IR) {
    //fprintf(stdout, "Instruction: 0x%08x\n", IR & 0xFC000000);
    //fprintf(stdout, "Primary: %u\n", (IR & 0xFC000000) >> 26);
    switch ((IR & 0xFC000000) >> 26) {
        case 0: // R-Type
            //fprintf(stdout, "R-Type: %u\n", IR & 0x0000003F);
            switch (IR & 0x0000003F) {
                case 8: // jr
                    return 5;
                case 9: // jalr
                    return 6;
                case 32: // add
                    return 7;
                case 33: // addu
                    return 8;
                case 34: // sub
                    return 9;
                case 35: // subu
                    return 10;
                case 36: // and
                    return 11;
                case 37: // or
                    return 12;
                case 38: // xor
                    return 13;
                case 39: // nor
                    return 14;
                default:
                    return 0; // Invalid
            }
        case 2: // j
            return 3;
        case 3: // jal
            return 4;
        case 4: // beq
            return 21;
        case 5: // bne
            return 23;
        case 6: // blez
            return 25;
        case 7: // bgtz
            return 27;
        case 8: // addi
            return 15;
        case 9: // addiu
            return 16; // TODO: addiu Not in RTL for some reason tristan
        case 12: // andi
            return 17;
        case 13: // ori
            return 18;
        case 14: // xori
            return 19;
        case 15: // lui
            return 20;
        case 19: // C-Type
            //fprintf(stdout, "C-Type: %u\n", (IR & 0x03E00000) >> 21);
            switch ((IR & 0x03E00000) >> 21) {
                case 0: // spd
                    return 33;
                case 1: // Rot
                    return 35;
                case 2: // Ascn
                    return 39;
                case 3: // Dscn
                    return 41;
                case 4: // Fir
                    return 37;
                default:
                    return 0; // Invalid
            }
        case 35: // lw
            return 29;
        case 43: // sw
            return 31;
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
    int IR;  // IR; Reset every "Read instruction" state
    int R1;  // Register 1; Reset every "Decode instruction" state
    int R2;  // Register 2; Reset every "Decode instruction" state
    int RD;  // Register write location;
    int IMM; // Immediate; Reset every "Decode instruction" state
    int X;   // X;
    int RF;  // RF;
    int state = 0;
    while (true) { // Cycle
        // force register 0 to always be 0
        R[0] = 0;
        fgets(line, sizeof(line), stdin);
        int read = atoi(line);
        //fprintf(stdout, "State: %u\n", state);
        switch (state) {
            case 0:
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
                fprintf(stdout, "N\n");
                state = getState(IR);
                continue;
            case 2:
                // 2,WB,R[RD] = X; PC = NPC; state = 0
                R[RD] = X;
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 3:
                // 3,j,PC = IR(25-0) << 2; next = 0
                PC = (IR & 0x03FFFFFF) << 2;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 4:
                // 4,jal,PC = IR(25-0) << 2; R[31] = NPC; next = 0
                PC = (IR & 0x03FFFFFF) << 2;
                R[31] = NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 5:
                // 5,jr,PC = R1; next = 0
                PC = R1;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 6:
                // 6,jalr,PC = R1; R[31] = NPC; next = 0
                PC = R1;
                R[31] = NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 7:
                // 7,add,X = R1 + R2; RD = IR(15-11); next = 2
                X = R1 + R2;
                RD = (IR & 0x0000F800) >> 11;
                fprintf(stdout, "N\n");
                state = 2;
                continue;
            case 8:
                // 8,addu,X = R1 + R2; RD = IR(15-11); next = 2
                X = R1 + R2;
                RD = (IR & 0x0000F800) >> 11;
                fprintf(stdout, "N\n");
                state = 2;
                continue;
            case 9:
                // 9,sub,X = R1 - R2; RD = IR(15-11); next = 2
                X = R1 - R2;
                RD = (IR & 0x0000F800) >> 11;
                fprintf(stdout, "N\n");
                state = 2;
                continue;
            case 10:
                // 10,subu,X = R1 - R2; RD = IR(15-11); next = 2
                X = R1 - R2;
                RD = (IR & 0x0000F800) >> 11;
                fprintf(stdout, "N\n");
                state = 2;
                continue;
            case 11:
                // 11,and,X = R1 & R2; RD = IR(15-11); next = 2
                X = R1 & R2;
                RD = (IR & 0x0000F800) >> 11;
                fprintf(stdout, "N\n");
                state = 2;
                continue;
            case 12:
                // 12,or,X = R1 | R2; RD = IR(15-11); next = 2
                X = R1 | R2;
                RD = (IR & 0x0000F800) >> 11;
                fprintf(stdout, "N\n");
                state = 2;
                continue;
            case 13:
                // 13,xor,X = R1 ^ R2; RD = IR(15-11); next = 2
                X = R1 ^ R2;
                RD = (IR & 0x0000F800) >> 11;
                fprintf(stdout, "N\n");
                state = 2;
                continue;
            case 14:
                // 14,nor,X = ~(R1 | R2); RD = IR(15-11); next = 2
                X = ~(R1 | R2);
                RD = (IR & 0x0000F800) >> 11;
                fprintf(stdout, "N\n");
                state = 2;
                continue;
            case 15:
                // 15,addi,X = R1 + IMM; RD = IR(20-16); next = 2
                X = R1 + IMM;
                RD = (IR & 0x001F0000) >> 16;
                fprintf(stdout, "N\n");
                state = 2;
                continue;
            case 16:
                // 16,addiu,X = R1 + IMM; RD = IR(20-16); next = 2
                X = R1 + IMM;
                RD = (IR & 0x001F0000) >> 16;
                fprintf(stdout, "N\n");
                state = 2;
                continue;
            case 17:
                // 17,andi,X = R1 & IMM; RD = IR(20-16); next = 2
                X = R1 & IMM;
                RD = (IR & 0x001F0000) >> 16;
                fprintf(stdout, "N\n");
                state = 2;
                continue;
            case 18:
                // 18,ori,X = R1 | IMM; RD = IR(20-16); next = 2
                X = R1 | IMM;
                RD = (IR & 0x001F0000) >> 16;
                fprintf(stdout, "N\n");
                state = 2;
                continue;
            case 19:
                // 19,xori,X = R1 ^ IMM; RD = IR(20-16); next = 2
                X = R1 ^ IMM;
                RD = (IR & 0x001F0000) >> 16;
                fprintf(stdout, "N\n");
                state = 2;
                continue;
            case 20:
                // 20,lui,X = IMM << 16; RD = IR(20-16); next = 2
                X = IMM << 16;
                RD = (IR & 0x001F0000) >> 16;
                fprintf(stdout, "N\n");
                state = 2;
                continue;
            case 21:
                // 21,beq0,X = R1 - R2; next = 22
                X = R1 - R2;
                fprintf(stdout, "N\n");
                state = 22;
                continue;
            case 22:
                // 22,beq1,PC = (X == 0) ? NPC + imm<<2 : NPC; next = 0;
                PC = (X == 0) ? (NPC + (IMM << 2)) : NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 23:
                // 23,bne0,X = R1 - R2; next = 24
                X = R1 - R2;
                fprintf(stdout, "N\n");
                state = 24;
                continue;
            case 24:
                // 24,bne1,PC = (X != 0) ? NPC + imm<<2 : NPC; next = 0;
                PC = (X != 0) ? (NPC + (IMM << 2)) : NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 25:
                // 25,blez0,X = R1; next = 26
                X = R1;
                fprintf(stdout, "N\n");
                state = 26;
                continue;
            case 26:
                // 26,blez1,PC = (X <= 0) ? NPC + imm<<2 : NPC; next = 0;
                PC = (X <= 0) ? (NPC + (IMM << 2)) : NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 27:
                // 27,bgtz0,X = R1; next = 28
                X = R1;
                fprintf(stdout, "N\n");
                state = 28;
                continue;
            case 28:
                // 28,bgtz1,PC = (X >= 0) ? NPC + imm<<2 : NPC; next = 0;
                PC = (X > 0) ? (NPC + (IMM << 2)) : NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 29:
                // 29,lw0,X = IMM + R1; next = 30
                X = IMM + R1;
                fprintf(stdout, "N\n");
                state = 30;
                continue;
            case 30:
                // 30,lw1,R[IR(20-16)] = IMem[X]; PC = NPC; next = 0
                R[(IR & 0x001F0000) >> 16] = IMem[X];
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 31:
                // 31,sw0,X = IMM + R1; next = 32
                X = IMM + R1;
                fprintf(stdout, "N\n");
                state = 32;
                continue;
            case 32:
                // 32,sw1,IMem[X] = R[IR(20-16)]; PC = NPC; next = 0
                IMem[X] = R[(IR & 0x001F0000) >> 16];
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            case 33:
                // 33,spd0,X = R2 + IMM; next = 34
                X = R2 + IMM;
                fprintf(stdout, "N\n");
                state = 34;
                continue;
            case 34:
                // 34,spd1,SPEED = X; PC = NPC; next = 0
                fprintf(stdout, "M %i\n", X);
                PC = NPC;
                state = 0;
                continue;
            case 35:
                // 35,rot0,X = R2 + IMM; next = 36
                X = R2 + IMM;
                fprintf(stdout, "N\n");
                state = 36;
                continue;
            case 36:
                // 36,rot1,ROT_RATE = X; PC = NPC; next = 0
                fprintf(stdout, "R %i\n", X);
                PC = NPC;
                state = 0;
                continue;
            case 37:
                // 37,fir0,X = R2 + IMM; next = 38
                X = R2 + IMM;
                fprintf(stdout, "N\n");
                state = 38;
                continue;
            case 38:
                // 38,fir1,FIRE = X; PC = NPC; next = 0
                fprintf(stdout, "F %i\n", X);
                PC = NPC;
                state = 0;
                continue;
            case 39:
                // 39,ascn0,RobotFunc=A; next = 40
                fprintf(stdout, "A\n");
                state = 40;
                continue;
            case 40:
                // 40,ascn1,RF = ANGLE_SCAN(); RD = IR(20-16); next = 43
                RF = read;
                RD = (IR & 0x001F0000) >> 16;
                fprintf(stdout, "N\n");
                state = 43;
                continue;
            case 41:
                // 41,dscn0,RobotFunc=D; next = 42
                fprintf(stdout, "D\n");
                state = 42;
                continue;
            case 42:
                // 42,dscn1,RF = DIST_SCAN(); RD = IR(20-16); next = 43
                RF = read;
                RD = (IR & 0x001F0000) >> 16;
                fprintf(stdout, "N\n");
                state = 43;
                continue;
            case 43:
                // 43,RWB,R[RD] = RF; PC = NPC; next = 0
                R[RD] = RF;
                PC = NPC;
                fprintf(stdout, "N\n");
                state = 0;
                continue;
            default:
                continue;
        }
    }
}
