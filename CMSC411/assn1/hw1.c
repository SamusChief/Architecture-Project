#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>

// struct used for map string comparisons
struct cmp_str
{
  bool operator()(char const *a, char const *b)
  {
    return strcmp(a, b) < 0;
  }
};

int main(int args, char* argv[]) {

  // Validation: must have input and output file args
  if(args != 3) {
    printf("Incorrect number of arguments.\nPlease specify an input and an output file name.\n");
    return 1;
  }

  // declare input and output files
  FILE* infile = fopen(argv[1], "r");
  FILE* outfile = fopen(argv[2], "w");
  
  //set up maps
  std::map <char*, int, cmp_str> labels;
  std::map <char*, int, cmp_str> jTypes;
  std::map <char*, int, cmp_str> rTypes;
  std::map <char*, int, cmp_str> iTypes;
  // j type
  jTypes.insert(std::make_pair(strdup("j"), 134217728));
  jTypes.insert(std::make_pair(strdup("jal"), 201326592));
  // i type
  iTypes.insert(std::make_pair(strdup("beq"), 268435456));
  iTypes.insert(std::make_pair(strdup("bne"), 335544320));
  iTypes.insert(std::make_pair(strdup("blez"), 402653184));
  iTypes.insert(std::make_pair(strdup("bgtz"), 469762048));
  iTypes.insert(std::make_pair(strdup("addi"), 536870912));
  iTypes.insert(std::make_pair(strdup("addiu"), 603979776));
  iTypes.insert(std::make_pair(strdup("andi"), 805306368));
  iTypes.insert(std::make_pair(strdup("ori"), 872415232));
  iTypes.insert(std::make_pair(strdup("xori"), 939524096));
  iTypes.insert(std::make_pair(strdup("lui"), 1006632960));
  iTypes.insert(std::make_pair(strdup("lw"), 2348810240));
  iTypes.insert(std::make_pair(strdup("sw"), 2885681152));
  // r type
  rTypes.insert(std::make_pair(strdup("jr"), 8));
  rTypes.insert(std::make_pair(strdup("jalr"), 9));
  rTypes.insert(std::make_pair(strdup("addu"), 33));
  rTypes.insert(std::make_pair(strdup("add"), 32));
  rTypes.insert(std::make_pair(strdup("sub"), 34));
  rTypes.insert(std::make_pair(strdup("subu"), 35));
  rTypes.insert(std::make_pair(strdup("and"), 36));
  rTypes.insert(std::make_pair(strdup("or"), 37));
  rTypes.insert(std::make_pair(strdup("xor"), 38));
  rTypes.insert(std::make_pair(strdup("nor"), 39));

  char line[256];
  int counter = 0;
  //first loop; detect labels
  while(fgets(line, 256, infile) != NULL) {
    char* token = strtok(line, " \t\v\r\n\f,()");
    // blank line
    if(token == NULL){
      continue;
    }
    // comment
    if(token[0] == ';') {
      continue;
    }
    // label
    if(token[0] == ':') {
      labels.insert(std::make_pair(strdup(token), counter));
      // get the next token
      token = strtok(NULL, " \t\v\r\n\f,()");
    }
    
    // line is blank besides label
    if(token == NULL){
      continue;
    }
    // label line has comment
    if(token[0] == ';') {
      continue;
    }
    //this is now an ordinary line, incrememt line counter
    counter++;
  }

  // labels found, rewind file to do second pass
  rewind(infile);

  // second pass of file, build hex values and output them
  counter = 0;
  while(fgets(line, 256, infile) != NULL) {
    
    char* token = strtok(line, " \t\v\r\n\f,()");
    // blank line
    if(token == NULL){
      continue;
    }
    // comment
    if(token[0] == ';') {
      continue;
    }
    // label
    if(token[0] == ':') {
      token = strtok(NULL, " \t\v\r\n\f,()");
    }
    // line is blank besides label
    if(token == NULL){
      continue;
    }
    // label line has comment
    if(token[0] == ';') {
      continue;
    }
    
    // this is a legitimate line
    counter++;

    // There are 4 super types of instructions:
    // J types, R types, I types, and regular integers
    // need to get relevant rs, rt, rd, etc parts and make one hex code each

    // j type?
    if(jTypes.find(token) != jTypes.end()) {
      // just get the label and opcode then or them
      int bits = jTypes[token];
      
      token = strtok(NULL, " \t\v\r\n\f,()");
      int label = labels[token];
      label = label & 0x3FFFFFF;
      bits = bits | label;
      
      fprintf(outfile, "0x%08x,\n", bits);
      continue;
    }
    // i type?
    if(iTypes.find(token) != iTypes.end()) {
      int bits = iTypes[token];
      // there are 5 different cases for i type instructions
      // we need to get the relevant rs, rt, offsets, and immediates as needed

      // b opcodes use line difference (label - counter)
      // beq, bne
      if(strcmp(token, "beq") == 0 || strcmp(token, "bne") == 0) {
	token = strtok(NULL, " \t\v\r\n\f,()");
	int rs;
	sscanf(token, "$%i", &rs);
	rs = rs << 21;
	rs = rs & 0x3E00000;
	bits = bits | rs;
	
	token = strtok(NULL, " \t\v\r\n\f,()");
	int rt;
	sscanf(token, "$%i", &rt);
	rt = rt << 16;
	rt = rt & 0x1F0000;
	bits = bits | rt;
	
	token = strtok(NULL, " \t\v\r\n\f,()");
	int imm;
	imm = labels[token] - counter;
	imm = imm & 0xFFFF;
	bits = bits | imm;

	fprintf(outfile, "0x%08x,\n", bits);
      }
      // blez, bgtz
      if(strcmp(token, "blez") == 0 || strcmp(token, "bgtz") == 0) {
	token = strtok(NULL, " \t\v\r\n\f,()");
	int rs;
	sscanf(token, "$%i", &rs);
	rs = rs << 21;
	rs = rs & 0x3E00000;
	bits = bits | rs;
	
	token = strtok(NULL, " \t\v\r\n\f,()");
	int imm;
	imm = labels[token] - counter;
	imm = imm & 0xFFFF;
	bits = bits | imm;
	
	fprintf(outfile, "0x%08x,\n", bits);
      }
      // addi, addiu, andi, ori, xori
      if(strcmp(token, "addi") == 0 || strcmp(token, "addiu") == 0 ||
	 strcmp(token, "andi") == 0 || strcmp(token, "ori") == 0 ||
	 strcmp(token, "xori") == 0) {
	token = strtok(NULL, " \t\v\r\n\f,()");
	int rt;
	sscanf(token, "$%i", &rt);
	rt = rt << 16;
	rt = rt & 0x1F0000;
	bits = bits | rt;

	token = strtok(NULL, " \t\v\r\f\n,()");
	int rs;
	sscanf(token, "$%i", &rs);
	rs = rs << 21;
	rs = rs & 0x3E00000;
	bits = bits | rs;

	token = strtok(NULL, " \t\v\r\f\n,()");
	int imm;
	sscanf(token, "#%i", &imm);
	imm = imm & 0xFFFF;
	bits = bits | imm;

	fprintf(outfile, "0x%08x,\n", bits);
	
      }
      // lui
      if(strcmp(token, "lui") == 0) {
	token = strtok(NULL, " \t\v\r\n\f,()");
	int rt;
	sscanf(token, "$%i", &rt);
	rt = rt << 16;
	rt = rt & 0x1F0000;
	bits = bits | rt;

	token = strtok(NULL, " \t\v\r\f\n,()");
	int imm;
	sscanf(token, "#%i", &imm);
	imm = imm & 0xFFFF;
	bits = bits | imm;

	fprintf(outfile, "0x%08x,\n", bits);
      }
      // lw, sw
      if(strcmp(token, "lw") == 0 || strcmp(token, "sw") == 0) {
	token = strtok(NULL, " \t\v\r\n\f,()");
	int rt;
	sscanf(token, "$%i", &rt);
	rt = rt << 16;
	rt = rt & 0x1F0000;
	bits = bits | rt;

	token = strtok(NULL, " \t\v\r\f\n,()");
	int off;
	off = labels[token] * 4;
	off = off & 0xFFFF;
	bits = bits | off;

	token = strtok(NULL, " \t\v\f\r\n,()");
	int rs;
	sscanf(token, "$%i", &rs);
	rs = rs << 21;
	rs = rs & 0x3E00000;
	bits = bits | rs;
	fprintf(outfile, "0x%08x,\n", bits);
      }

      continue;
    }
    // r type?
    if(rTypes.find(token) != rTypes.end()) {
      int bits = rTypes[token];
      
      token = strtok(NULL, " \t\v\r\n\f,()");
      int rd;
      sscanf(token, "$%i", &rd);
      rd = rd << 11;
      rd = rd & 0xF800;
      bits = bits | rd;
      
      token = strtok(NULL, " \t\v\f\r\n,()");
      int rs;
      sscanf(token, "$%i", &rs);
      rs = rs << 21;
      rs = rs & 0x3E00000;
      bits = bits | rs;
      
      token = strtok(NULL, " \t\v\r\f\n,()");
      int rt;
      sscanf(token, "$%i", &rt);
      rt = rt << 16;
      rt = rt & 0x1F0000;
      bits = bits | rt;
      
      fprintf(outfile, "0x%08x,\n", bits);
      continue;
    }
    // just an integer
    if(strcmp(token, "int") == 0) {
      token = strtok(NULL, " \t\v\r\n\f,()");
      int bits;
      sscanf(token, "#%i", &bits);
      fprintf(outfile, "0x%08x,\n", bits);
      continue;
    }
  }

  fclose(infile);
  fclose(outfile);
}
