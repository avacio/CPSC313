#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "printRoutines.h"

#define ERROR_RETURN -1
#define SUCCESS 0

// Function Prototypes
int binaryToY86(char *file, int length, int start);
int condMove(char begin, char end, char r);
int immToReg(char begin, char end, char r, char v1, char v2, char v3, char v4);
int regToMem(char begin, char end, char r, char d1, char d2, char d3, char d4);
int memToReg(char begin, char end, char r, char d1, char d2, char d3, char d4);
int OPq(char begin, char end, char r);
int jump(char begin, char end, char d1, char d2, char d3, char d4);
int call(char begin, char end, char d1, char d2, char d3, char d4);
int ret(char begin, char end);
int pushq(char begin, char end, char r);
int popq(char begin, char end, char r);

void printCondMove(char* name, int r1, int r2, int lowB, int highB);
void printOPq(char* name, int r1, int r2, int lowB, int highB);
char** regRep(char** str, int r, int regNum);


// Global Variables
int start;
int res;
long currAddr = 0;
FILE *inputFilePtr, *outputFile;

int main(int argc, char **argv) {

  FILE *machineCode; //, *outputFile;

  // Verify that the command line has an appropriate number
  // of arguments

  if (argc < 2 || argc > 4) {
    fprintf(stderr, "Usage: %s InputFilename [OutputFilename] [startingOffset]\n", argv[0]);
    return ERROR_RETURN;
  }

  // First argument is the file to read, attempt to open it
  // for reading and verify that the open did occur.
  machineCode = fopen(argv[1], "rb");

  if (machineCode == NULL) {
    fprintf(stderr, "Failed to open %s: %s\n", argv[1], strerror(errno));
    return ERROR_RETURN;
  }

  // Second argument is the file to write, attempt to open it for
  // writing and verify that the open did occur. Use standard output
  // if not provided.
  outputFile = argc <= 2 ? stdout : fopen(argv[2], "w");

  if (outputFile == NULL) {
    fprintf(stderr, "Failed to open %s: %s\n", argv[2], strerror(errno));
    fclose(machineCode);
    return ERROR_RETURN;
  }

  // If there is a 3rd argument present it is an offset so convert it
  // to a numeric value.
  if (4 == argc) {
    errno = 0;
    currAddr = strtol(argv[3], NULL, 0);
    if (errno != 0) {
      perror("Invalid offset on command line");
      fclose(machineCode);
      fclose(outputFile);
      return ERROR_RETURN;
    }
    //
    start = strtol(argv[3], NULL, 10); ///////
  } else {start = 0;}

  fprintf(stderr, "Opened %s, starting offset 0x%lX\n", argv[1], currAddr);
  fprintf(stderr, "Saving output to %s\n", argc <= 2 ? "standard output" : argv[2]);

  // Comment or delete the following lines and this comment before
  // handing in your final version.
  // if (currAddr) printPosition(outputFile, currAddr);
  // printInstruction(outputFile);

////////////////////////////
  // Your code starts here.

  long fileSize;
  char *file; // necessary?

  // machineCode pointer (binary file)
  inputFilePtr = fopen(argv[1], "rb");
  outputFile = fopen(argv[2], "w");

  fseek(inputFilePtr, 0, SEEK_END); // go to end
  fileSize = ftell(inputFilePtr);

  // allocate fileSize space for file
  file = (char*) malloc((fileSize+1)*sizeof(char));

  rewind(inputFilePtr); // begin at start

  fread(file, fileSize, 1, inputFilePtr);
  fclose(inputFilePtr); // close pointer

  // call helper function to convert the binary to Y86
  binaryToY86(file, fileSize+1, start);

  free(file);

  fclose(machineCode);
  fclose(outputFile);
  return SUCCESS;
}

int binaryToY86(char *file, int length, int start) {
  int valid = 0;  // is 1 when instructions other than 0 are used
                  // to initialize halt
  for (int i = start; i < length; i++) {
    char a = file[i];
    // shifting and masking
    char begin = a & 0xF;
    char end = (a >> 4) & 0xF;

        // res = fprintf(outputFile, "    %x            # \n", end);
    if (end == 0x1 || end == 0x2 || end == 0x3 || end == 0x4 || end == 0x5 || end == 0x6
      || end == 0x7 || end == 0x8 || end == 0x9 || end == 0xA || end == 0xB) {

      if (currAddr != 0 && valid == 0) {res = fprintf(outputFile, "    .pos %lx\n", currAddr);
      valid = 1;
      }
    }
    switch (end) {
      case 0x0: // halt
        // if  (valid == 1) // BUGS WHEN TRYING TO PRINT HALT, so commented out
         // {res = fprintf(outputFile, "    halt                        # 00\n");
        // currAddr++;
       // i++;
      // }
        currAddr++;
        // i++;//
        continue;
      case 0x1: // nop
        res = fprintf(outputFile, "    nop                         # 10\n");
        currAddr++;
        // i++;
        // valid = 1;
        continue;
      case 0x2: // rrmovq, cmov
        condMove(begin, end, file[i+1]);
        currAddr+=2;
        i+=5;
        // valid = 1;
        continue;
      case 0x3: // irmovq
        immToReg(begin, end, file[i+1], file[i+2], file[i+3], file[i+4], file[i+5]);
        currAddr+=6;
        i+=5; // +5
        // valid = 1;
        continue;
      case 0x4: // rmmovq
        regToMem(begin, end, file[i+1], file[i+2], file[i+3], file[i+4], file[i+5]);
        currAddr+=6;
        i+=5; // +5
        // valid = 1;
        continue;
      case 0x5: // mrmovq
        memToReg(begin, end, file[i+1], file[i+2], file[i+3], file[i+4], file[i+5]);
        currAddr+=6;
        i+=5; // +5
        // valid = 1;
        continue;
      case 0x6: // OPq
        if (OPq(begin, end, file[i+1]) > 0) {
          currAddr+=2;
          i++;
          // valid = 1;
        } else { printf("# Invalid instruction.");}
        continue;
      case 0x7: // jXX
        jump(begin, end, file[i+1], file[i+2], file[i+3], file[i+4]);
        currAddr+=5;
        i+=4;
        // valid = 1;
        continue;
      case 0x8: // call
        call(begin, end, file[i+1], file[i+2], file[i+3], file[i+4]);
        currAddr+=5;
        i+=4;
        // valid = 1;
        continue;
      case 0x9: // return
        res = fprintf(outputFile, "    ret                         # 90\n");
        currAddr++;
        // i++;
        // valid = 1;
        continue;
      case 0xA: // pushq
        pushq(begin, end, file[i+1]);
        currAddr+=2;
        i++;
        // valid = 1;
        continue;
      case 0xB: // popq
        popq(begin, end, file[i+1]);
        currAddr+=2;
        i++;
        // valid = 1;
        continue;
      default:
        printf("# Invalid instruction.");
        if(currAddr%8 != 0){
        res = fprintf(outputFile, "    %-8s %dx\n", "byte.", end);
        break;
        } else {
        res = fprintf(outputFile, "    %-8s %dx\n", "quad.", end);
        break;
        }
    }
  }
  return 1;
}

// conditional move
int condMove(char begin, char end, char r) {
  char rA = (r >> 4) & 0xF;
  char rB = r & 0xF;

  int r1 = rA; // convert to ints
  int r2 = rB;
  int lowB = (int) begin;
  int highB = (int) end;

  switch (begin) {
    case 0x0:
      printCondMove("rrmovq", r1, r2, lowB, highB);
      break;
    case 0x1:
      printCondMove("cmovle", r1, r2, lowB, highB);
      break;
    case 0x2:
      printCondMove("cmovl", r1, r2, lowB, highB);
      break;
    case 0x3:
      printCondMove("cmove", r1, r2, lowB, highB);
      break;
    case 0x4:
      printCondMove("cmovne", r1, r2, lowB, highB);
      break;
    case 0x5:
      printCondMove("cmovge", r1, r2, lowB, highB);
      break;
    case 0x6:
      printCondMove("cmovg", r1, r2, lowB, highB);
      break;
    default:
      printf("# Invalid instruction.");
      if(currAddr%8 != 0){
        res = fprintf(outputFile, "%-8s %dx\n", "byte.", end);
        break;
        } else {
        res = fprintf(outputFile, "%-8s %dx\n", "quad.", end);
        break;
        }

  }
  return 1;
}

void printCondMove(char* name, int r1, int r2, int lowB, int highB) {
 char** registers = malloc(2*sizeof(char*));
  registers[0] = malloc(4*sizeof(char*));
  registers[1] = malloc(4*sizeof(char*));
  registers = regRep(registers, r1, 0);
  registers = regRep(registers, r2, 1);

  char* rA = registers[0];
  char* rB = registers[1];

  res = fprintf(outputFile, "    %-8s%s, %s          # %d%d%x%x\n", name, rA, rB, highB, lowB, r1, r2);
}

// representation of registers
// regNum is 0-based
char** regRep(char** str, int r, int regNum) {
  switch (r) {
    case 0x0:
      strcpy(str[regNum], "%rax");
      break;
    case 0x1:
      strcpy(str[regNum], "%rcx");
      break;
    case 0x2:
      strcpy(str[regNum], "%rdx");
      break;
    case 0x3:
      strcpy(str[regNum], "%rbx");
      break;
    case 0x4:
      strcpy(str[regNum], "%rsp");
      break;
    case 0x5:
      strcpy(str[regNum], "%rbp");
      break;
    case 0x6:
      strcpy(str[regNum], "%rsi");
      break;
    case 0x7:
      strcpy(str[regNum], "%rdi");
      break;
    case 0x8:
      strcpy(str[regNum], "%r8");
      break;
    case 0x9:
      strcpy(str[regNum], "%r9");
      break;
    case 0xA:
      strcpy(str[regNum], "%r10");
      break;
    case 0xB:
      strcpy(str[regNum], "%r11");
      break;
    case 0xC:
      strcpy(str[regNum], "%r12");
      break;
    case 0xD:
      strcpy(str[regNum], "%r13");
      break;
    case 0xE:
      strcpy(str[regNum], "%r14");
      break;
    default:
      strcpy(str[regNum], "No register");
      break;
  }
  return str;
}

int immToReg(char begin, char end, char r, char v1, char v2, char v3, char v4) {
  char rB = r & 0xF;

  int v1a = (v1 >> 4) & 0xF; int v1b = v1 & 0xF;
  int v2a = (v2 >> 4) & 0xF; int v2b = v2 & 0xF;
  int v3a = (v3 >> 4) & 0xF; int v3b = v3 & 0xF;
  int v4a = (v4 >> 4) & 0xF; int v4b = v4 & 0xF;

  int r2 = rB;
  int highB = (int) end; // high bounds
  int lowB = (int) begin; // low bounds

  char** registers = malloc(2*sizeof(char*));
  registers[0] = malloc(4*sizeof(char*));
  registers[1] = malloc(4*sizeof(char*));

  registers = regRep(registers, r2, 1);

  char* regB = registers[1];
  res = fprintf(outputFile, "    %-8s$0x%x%x%x%x%x%x%x%x, %s       # %d%d%s%x%0x%0x%0x%0x%0x%0x%0x%0x\n", "irmovq", v4a, v4b, v3a, v3b, v2a, v2b, v1a, v1b, regB, highB, lowB, "f", r2, v1a, v1b, v2a, v2b, v3a, v3b, v4a, v4b); // double bytes

  return 1;
}

int regToMem(char begin, char end, char r, char d1, char d2, char d3, char d4) {
  char rA = (r >> 4) & 0xF;
  char rB = r & 0xF;
  char d1a = (d1 >> 4) & 0xF; char d1b = d1 & 0xF;
  char d2a = (d2 >> 4) & 0xF; char d2b = d2 & 0xF;
  char d3a = (d3 >> 4) & 0xF; char d3b = d3 & 0xF;
  char d4a = (d4 >> 4) & 0xF; char d4b = d4 & 0xF;

  int r1 = rA; // to ints
  int r2 = rB;
  int highB = (int) end;
  int lowB = (int) begin;

  char** registers = malloc(2*sizeof(char*));
  registers[0] = malloc(4*sizeof(char*));
  registers[1] = malloc(4*sizeof(char*));
  registers = regRep(registers, r1, 0);
  registers = regRep(registers, r2, 1);

  char* regA = registers[0];
  char* regB = registers[1];

  res = fprintf(outputFile, "    %-8s%s, $0x%x%x%x%x%x%x%x%x(%s) # %d%d%x%x%0x%0x%0x%0x%0x%0x%0x%0x\n", "rmmovq", regA, d4a, d4b, d3a, d3b, d2a, d2b, d1a, d1b, regB, highB, lowB, r1, r2, d1a, d1b, d2a, d2b, d3a, d3b, d4a, d4b);

  return 1;
}

int memToReg(char begin, char end, char r, char d1, char d2, char d3, char d4) { //
  char rA = (r >> 4) & 0xF;
  char rB = r & 0xF;
  char d1a = (d1 >> 4) & 0xF; char d1b = d1 & 0xF;
  char d2a = (d2 >> 4) & 0xF; char d2b = d2 & 0xF;
  char d3a = (d3 >> 4) & 0xF; char d3b = d3 & 0xF;
  char d4a = (d4 >> 4) & 0xF; char d4b = d4 & 0xF;

  int r1 = rA; // to ints
  int r2 = rB;
  int highB = (int) end;
  int lowB = (int) begin;

  char** registers = malloc(2*sizeof(char*));
  registers[0] = malloc(4*sizeof(char*));
  registers[1] = malloc(4*sizeof(char*));
  registers = regRep(registers, r1, 0);
  registers = regRep(registers, r2, 1);

  char* regA = registers[0];
  char* regB = registers[1];

  res = fprintf(outputFile, "    %-8s$0x%x%x%x%x%x%x%x%x(%s), %s # %d%d%x%x%0x%0x%0x%0x%0x%0x%0x%0x\n", "mrmovq", d4a, d4b, d3a, d3b, d2a, d2b, d1a, d1b, regB, regA, highB, lowB, r1, r2, d1a, d1b, d2a, d2b, d3a, d3b, d4a, d4b);

  return 1;
}

int OPq(char begin, char end, char r) {
  char rA = (r >> 4) & 0xF;
  char rB = r & 0xF;

  int r1 = rA;
  int r2 = rB;
  int highB = (int) end;
  int lowB = (int) begin;

  switch (begin) {
    case 0x0:
      printOPq("addq", r1, r2, lowB, highB);
      break;
    case 0x1:
      printOPq("subq", r1, r2, lowB, highB);
      break;
    case 0x2:
      printOPq("andq", r1, r2, lowB, highB);
      break;
    case 0x3:
      printOPq("xorq", r1, r2, lowB, highB);
      break;
    default:
      printf("# Invalid instruction.");
      if(currAddr%8 != 0){
        res = fprintf(outputFile, "    %-8s %dx\n", "byte.", end);
        break;
        } else {
        res = fprintf(outputFile, "    %-8s %dx\n", "quad.", end);
        break;
        }

  }
  return 1;
}

void printOPq(char* name, int r1, int r2, int lowB, int highB) {
  char** registers = malloc(2*sizeof(char*));
  registers[0] = malloc(4*sizeof(char*));
  registers[1] = malloc(4*sizeof(char*));
  registers = regRep(registers, r1, 0);
  registers = regRep(registers, r2, 1);

  char* regA = registers[0];
  char* regB = registers[1];

  res = fprintf(outputFile, "    %-8s%s, %s          # %d%d%x%x\n", name, regA , regB, highB, lowB, r1, r2);
}

int jump(char begin, char end, char d1, char d2, char d3, char d4) {
  char d1a = (d1 >> 4) & 0xF; char d1b = d1 & 0xF;
  char d2a = (d2 >> 4) & 0xF; char d2b = d2 & 0xF;
  char d3a = (d3 >> 4) & 0xF; char d3b = d3 & 0xF;
  char d4a = (d4 >> 4) & 0xF; char d4b = d4 & 0xF;

  int lowB = (int) begin;

  switch (begin) {
    case 0x0:
      res = fprintf(outputFile, "    %-8s0x%x%x%x%x%x%x%x%x             # 7%d%0x%0x%x%x%x%x%x%x\n", "jmp", d4a, d4b, d3a, d3b, d2a, d2b, d1a, d1b, lowB, d1a, d1b, d2a, d2b, d3a, d3b, d4a, d4b);
      if (res <= 0) {
        printf("# Invalid instruction.");
        return -1;
      }
      break;
    case 0x1:
      res = fprintf(outputFile, "    %-8s0x%x%x%x%x%x%x%x%x             # 7%d%0x%0x%x%x%x%x%x%x\n", "jle", d4a, d4b, d3a, d3b, d2a, d2b, d1a, d1b, lowB, d1a, d1b, d2a, d2b, d3a, d3b, d4a, d4b);
      if (res <= 0) {
        printf("# Invalid instruction.");
        return -1;
      }
      break;
    case 0x2:
      res = fprintf(outputFile, "    %-8s0x%x%x%x%x%x%x%x%x             # 7%d%0x%0x%x%x%x%x%x%x\n", "jl", d4a, d4b, d3a, d3b, d2a, d2b, d1a, d1b, lowB, d1a, d1b, d2a, d2b, d3a, d3b, d4a, d4b);
      if (res <= 0) {
        printf("# Invalid instruction.");
        return -1;
      }
      break;
    case 0x3:
      res = fprintf(outputFile, "    %-8s0x%x%x%x%x%x%x%x%x             # 7%d%0x%0x%x%x%x%x%x%x\n", "je", d4a, d4b, d3a, d3b, d2a, d2b, d1a, d1b, lowB, d1a, d1b, d2a, d2b, d3a, d3b, d4a, d4b);
      if (res <= 0) {
        printf("# Invalid instruction.");
        return -1;
      }
      break;
    case 0x4:
      res = fprintf(outputFile, "    %-8s0x%x%x%x%x%x%x%x%x             # 7%d%0x%0x%x%x%x%x%x%x\n", "jne", d4a, d4b, d3a, d3b, d2a, d2b, d1a, d1b, lowB, d1a, d1b, d2a, d2b, d3a, d3b, d4a, d4b);
      if (res <= 0) {
        printf("# Invalid instruction.");
        return -1;
      }
      break;
    case 0x5:
      res = fprintf(outputFile, "    %-8s0x%x%x%x%x%x%x%x%x             # 7%d%0x%0x%x%x%x%x%x%x\n", "jge", d4a, d4b, d3a, d3b, d2a, d2b, d1a, d1b, lowB, d1a, d1b, d2a, d2b, d3a, d3b, d4a, d4b);
      if (res <= 0) {
        printf("# Invalid instruction.");
        return -1;
      }
      break;
    case 0x6:
      res = fprintf(outputFile, "    %-8s0x%x%x%x%x%x%x%x%x             # 7%d%0x%0x%x%x%x%x%x%x\n", "jg", d4a, d4b, d3a, d3b, d2a, d2b, d1a, d1b, lowB, d1a, d1b, d2a, d2b, d3a, d3b, d4a, d4b);
      if (res <= 0) {
        printf("# Invalid instruction.");
        return -1;
      }
      break;
    default:
      printf("# Invalid instruction.");
      if(currAddr%8 != 0){
        res = fprintf(outputFile, "    %-8s %dx\n", "byte.", end);
        break;
        } else {
        res = fprintf(outputFile, "    %-8s %dx\n", "quad.", end);
        break;
        }
  }
  return 1;
}

int call(char begin, char end, char d1, char d2, char d3, char d4) {
  char d1a = (d1 >> 4) & 0xF; char d1b = d1 & 0xF;
  char d2a = (d2 >> 4) & 0xF; char d2b = d2 & 0xF;
  char d3a = (d3 >> 4) & 0xF; char d3b = d3 & 0xF;
  char d4a = (d4 >> 4) & 0xF; char d4b = d4 & 0xF;

  res = fprintf(outputFile, "    %-8s0x%x%x%x%x%x%x%x%x              # 80%0x%0x%0x%0x%0x%0x%0x%0x\n", "call", d4a, d4b, d3a, d3b, d2a, d2b, d1a, d1b, d1a, d1b, d2a, d2b, d3a, d3b, d4a, d4b);

  if (res <= 0) {
        printf("# Invalid instruction.");
        return -1;
      }

  return 1;
}

int pushq(char begin, char end, char r) {
  char rA = (r >> 4) & 0xF;

  int r1 = rA;

  char** registers = malloc(2*sizeof(char*));
  registers[0] = malloc(4*sizeof(char*));
  registers[1] = malloc(4*sizeof(char*));
  registers = regRep(registers, r1, 0);

  char* regA = registers[0];

  res = fprintf(outputFile, "    %-8s%s                # A0%xF\n", "pushq", regA, r1);

  if (res <= 0) {
        printf("# Invalid instruction.");
        return -1;
        if(currAddr%8 != 0){
        res = fprintf(outputFile, "    %-8s %dx\n", "byte.", end);

        } else {
        res = fprintf(outputFile, "    %-8s %dx\n", "quad.", end);
        }
      }
  return 1;
}

int popq(char begin, char end, char r) {
  char rA = (r >> 4) & 0xF;

  int r1 = rA;

  char** registers = malloc(2*sizeof(char*));
  registers[0] = malloc(4*sizeof(char*));
  registers[1] = malloc(4*sizeof(char*));
  registers = regRep(registers, r1, 0);

  char* regA = registers[0];

  res = fprintf(outputFile, "    %-8s%s                # B0%xF\n", "popq", regA, r1);

  if (res <= 0) {
        printf("# Invalid instruction.");
        return -1;
        if(currAddr%8 != 0){
        res = fprintf(outputFile, "    %-8s %dx\n", "byte.", end);
        } else {
        res = fprintf(outputFile, "    %-8s %dx\n", "quad.", end);
        }
      }
  return 1;
}



