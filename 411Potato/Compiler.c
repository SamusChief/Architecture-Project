// Daniel Franchy
// franchy1@umbc.edu
// Project

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <string.h>

int main(int argc, char *argv[]) {
    std::map<std::string, int> labels;
    if (argc < 3) {
        printf("Usage: %s [input] [output]\n", argv[0]);
        return 1;
    }
    std::map<std::string, int> jTypes;
    std::map<std::string, int> idTypes; // 1 arg
    std::map<std::string, int> ibsTypes; // Branching 1 arg
    std::map<std::string, int> ibdsTypes; // Branching 2 arg
    std::map<std::string, int> idsTypes; // 2 arg
    std::map<std::string, int> idlsTypes; // 2 arg with special address case
    std::map<std::string, int> ccidTypes;
    std::map<std::string, int> ccdTypes;
    std::map<std::string, int> rTypes;
    jTypes.insert(std::make_pair("j", 8));
    jTypes.insert(std::make_pair("jal", 12));
    ibdsTypes.insert(std::make_pair("beq", 16));
    ibdsTypes.insert(std::make_pair("bne", 20));
    ibsTypes.insert(std::make_pair("blez", 24));
    ibsTypes.insert(std::make_pair("bgtz", 28));
    idsTypes.insert(std::make_pair("addi", 32));
    idsTypes.insert(std::make_pair("addiu", 36));
    idsTypes.insert(std::make_pair("andi", 48));
    idsTypes.insert(std::make_pair("ori", 52));
    idsTypes.insert(std::make_pair("xori", 56));
    idTypes.insert(std::make_pair("lui", 60));
    idlsTypes.insert(std::make_pair("lw", 140));
    idlsTypes.insert(std::make_pair("sw", 172));
    ccidTypes.insert(std::make_pair("spd", 0));
    ccidTypes.insert(std::make_pair("rot", 1));
    ccidTypes.insert(std::make_pair("fir", 4));
    ccdTypes.insert(std::make_pair("ascn", 2));
    ccdTypes.insert(std::make_pair("dscn", 3));
    rTypes.insert(std::make_pair("jr", 8));
    rTypes.insert(std::make_pair("jalr", 9));
    rTypes.insert(std::make_pair("add", 32));
    rTypes.insert(std::make_pair("addu", 33));
    rTypes.insert(std::make_pair("sub", 34));
    rTypes.insert(std::make_pair("subu", 35));
    rTypes.insert(std::make_pair("and", 36));
    rTypes.insert(std::make_pair("or", 37));
    rTypes.insert(std::make_pair("xor", 38));
    rTypes.insert(std::make_pair("nor", 39));
    char* inputFile = argv[1];
    FILE* input = fopen(inputFile, "r");
    char* outputFile = argv[2];
    FILE* output = fopen(outputFile, "w");
    char lineBuffer[256];
    int address = 0;
    while (fgets(lineBuffer, 256, input) != NULL) {
        char* token = strtok(lineBuffer, " \t\v\r\n\f,()");
        if (token == NULL) { // Blank Line
            continue;
        }
        if (strncmp(token, ";", 1) == 0) { // Comment
            continue;
        }
        while(token != NULL && strncmp(token, ":", 1) == 0) { // Multiple labels?
            labels.insert(std::make_pair(strdup(token), address));
            token = strtok(NULL, " \t\v\r\n\f,()");
        }
        if (token == NULL) { // End of Line
            continue;
        }
        if (strncmp(token, ";", 1) == 0) { // Comment
            continue;
        }
        address++; // Only increment for lines with instructions
    }
    rewind(input); // Rewind to prepare for re-read
    int next = 0;
    while (fgets(lineBuffer, 256, input) != NULL) {
        char* token = strtok(lineBuffer, " \t\v\r\n\f,()");
        if (token == NULL) { // Blank line
            continue;
        }
        while (token != NULL && strncmp(token, ":", 1) == 0) { // Skip labels
            token = strtok(NULL, " \t\v\r\n\f,()");
        }
        if (token == NULL) {
            continue;
        }
        if (strncmp(token, ";", 1) == 0) { // Comment only line
            continue;
        }
        next++;
        int line = 0x00000000; // Start with a clean slate
        if (jTypes.find(token) != jTypes.end()) { // J type opcode
            int opCode = jTypes[token];
            opCode = opCode << 24; // We have the 8 bit number stored, we shift it 24 to preserve that
            line = line | opCode;
            token = strtok(NULL, " \t\v\r\n\f,()");
            if (strncmp(token, ":", 1) == 0) { // Use label
                address = labels[token];
            } else {
                sscanf(token, "#%i", &address); // Use literal
            }
            address = address & 0x3FFFFFF; // Mask first 6 bits
            line = line | address;
            fprintf(output, "0x%08x,\n", line);
            continue;
        }
        if (ibsTypes.find(token) != ibsTypes.end()) {
            int opCode = ibsTypes[token];
            opCode = opCode << 24;
            line = line | opCode;
            token = strtok(NULL, " \t\v\r\n\f,()");
            sscanf(token, "$%i", &address); // rs
            address = address << 3;
            address = address & 0xFF;
            address = address << 18;
            line = line | address;
            token = strtok(NULL, " \t\v\r\n\f,()"); // imm
            if (strncmp(token, ":", 1) == 0) {
                address = labels[token] - next; // Offset between next instruction and label
            } else {
                sscanf(token, "#%i", &address);
            }
            address = address & 0xFFFF;
            line = line | address;
            fprintf(output, "0x%08x,\n", line);
            continue;
        }
        if (idTypes.find(token) != idTypes.end()) {
            int opCode = idTypes[token];
            opCode = opCode << 24;
            line = line | opCode;
            token = strtok(NULL, " \t\v\r\n\f,()");
            sscanf(token, "$%i", &address); //rd
            address = address << 3;
            address = address & 0xFF;
            address = address << 13;
            line = line | address;
            token = strtok(NULL, " \t\v\r\n\f,()"); // imm
            if (strncmp(token, ":", 1) == 0) {
                   address = labels[token];
            } else {
                   sscanf(token, "#%i", &address);
            }
            address = address & 0xFFFF;
            line = line | address;
            fprintf(output, "0x%08x,\n", line);
            continue;
        }
        if (idsTypes.find(token) != idsTypes.end()) {
            int opCode = idsTypes[token];
            opCode = opCode << 24;
            line = line | opCode;
            token = strtok(NULL, " \t\v\r\n\f,()"); // rd
            sscanf(token, "$%i", &address);
            address = address << 3;
            address = address & 0xFF;
            address = address << 13;
            line = line | address;
            token = strtok(NULL, " \t\v\r\n\f,()"); // rs
            sscanf(token, "$%i", &address);
            address = address << 3;
            address = address & 0xFF;
            address = address << 18;
            line = line | address;
            token = strtok(NULL, " \t\v\r\n\f,()"); // imm
            if (strncmp(token, ":", 1) == 0) {
                address = labels[token];
            } else {
                sscanf(token, "#%i", &address);
            }
            address = address & 0xFFFF;
            line = line | address;
            fprintf(output, "0x%08x,\n", line);
            continue;
        }
        if (ibdsTypes.find(token) != ibdsTypes.end()) {
            int opCode = ibdsTypes[token];
            opCode = opCode << 24;
            line = line | opCode;
            token = strtok(NULL, " \t\v\r\n\f,()"); // rd
            sscanf(token, "$%i", &address);
            address = address << 3;
            address = address & 0xFF;
            address = address << 13;
            line = line | address;
            token = strtok(NULL, " \t\v\r\n\f,()"); // rs
            sscanf(token, "$%i", &address);
            address = address << 3;
            address = address & 0xFF;
            address = address << 18;
            line = line | address;
            token = strtok(NULL, " \t\v\r\n\f,()"); // imm
            if (strncmp(token, ":", 1) == 0) {
                address = labels[token] - next; // Offset between next instruction and label
            } else {
                sscanf(token, "#%i", &address);
            }
            address = address & 0xFFFF;
            line = line | address;
            fprintf(output, "0x%08x,\n", line);
            continue;
        }
        if (idlsTypes.find(token) != idlsTypes.end()) {
            int opCode = idlsTypes[token];
            opCode = opCode << 24;
            line = line | opCode;
            token = strtok(NULL, " \t\v\r\n\f,()"); // rd
            sscanf(token, "$%i", &address);
            address = address << 3;
            address = address & 0xFF;
            address = address << 13;
            line = line | address;
            token = strtok(NULL, " \t\v\r\n\f,()"); // label
            if (strncmp(token, ":", 1) == 0) {
                address = labels[token];
                address = address * 4; // Memory address instead?
            } else {
                sscanf(token, "#%i", &address);
            }
            address = address & 0xFFFF;
            line = line | address;
            token = strtok(NULL, " \t\v\r\n\f,()"); // rs
            sscanf(token, "$%i", &address);
            address = address << 3;
            address = address & 0xFF;
            address = address << 18;
            line = line | address;
            fprintf(output, "0x%08x,\n", line);
            continue;
        }
        if (rTypes.find(token) != rTypes.end()) {
            int opCode = rTypes[token];
            opCode = opCode << 2;
            opCode = opCode & 0xFF;
            opCode = opCode >> 2;
            line = line | opCode;
            token = strtok(NULL, " \t\v\r\n\f,()"); // rd
            sscanf(token, "$%i", &address);
            address = address << 3;
            address = address & 0xFF;
            address = address << 8;
            line = line | address;
            token = strtok(NULL, " \t\v\r\n\f,()");  // rs
            sscanf(token, "$%i", &address);
            address = address << 3;
            address = address & 0xFF;
            address = address << 18;
            line = line | address;
            token = strtok(NULL, " \t\v\r\n\f,()"); // rt
            sscanf(token, "$%i", &address);
            address = address << 3;
            address = address & 0xFF;
            address = address << 13;
            line = line | address;
            fprintf(output, "0x%08x,\n", line);
            continue;
        }
        if (ccidTypes.find(token) != ccidTypes.end()) {
            line = line | 0x4C000000;
            int opCode = ccidTypes[token];
            opCode = opCode << 21;
            line = line | opCode;
            token = strtok(NULL, " \t\v\r\n\f,()"); // label/immediate
            if (strncmp(token, ":", 1) == 0) {
                address = labels[token];
            } else {
                sscanf(token, "#%i", &address);
            }
            address = address & 0xFFFF;
            line = line | address;
            token = strtok(NULL, " \t\v\r\n\f,()"); // rd
            sscanf(token, "$%i", &address);
            address = address << 3;
            address = address & 0xFF;
            address = address << 13;
            line = line | address;
            fprintf(output, "0x%08x,\n", line);
            continue;
        }
        if (ccdTypes.find(token) != ccdTypes.end()) {
            line = line | 0x4C000000;
            int opCode = ccdTypes[token];
            opCode = opCode << 21;
            line = line | opCode;
            token = strtok(NULL, " \t\v\r\n\f,()"); // rd
            sscanf(token, "$%i", &address);
            address = address << 3;
            address = address & 0xFF;
            address = address << 13;
            line = line | address;
            fprintf(output, "0x%08x,\n", line);
            continue;
        }
        if (strcmp(token, "int") == 0) {
            token = strtok(NULL, " \t\v\r\n\f,()"); // value
            sscanf(token, "#%i", &address);
            line = line | address;
            fprintf(output, "0x%08x,\n", line);
            continue;
        }
    }
    fclose(input);
    fclose(output);
}
