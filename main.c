#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BYTE_LENGTH 4
#define BYTE_LIMIT ((1 << BYTE_LENGTH) - 1)
#define MEMORY_LENGTH 16

#define TRUNCATE_BYTE(x) ((x) % (BYTE_LIMIT + 1))

#define I_HALT 0
#define I_ADD 1
#define I_SUB 2
#define I_INC_R0 3
#define I_INC_R1 4
#define I_DEC_R0 5
#define I_DEC_R1 6
#define I_BELL 7
#define I_PRINT 8
#define I_LOAD_R0 9
#define I_LOAD_R1 10
#define I_STORE_0 11
#define I_STORE_1 12
#define I_JUMP 13
#define I_JUMP_ZERO 14
#define I_JUMP_NOT_ZERO 15

// if defined, print "DING" instead of actually ringing a bell
//#define FAKE_BELL

void load_program(FILE *in, char *memory);
void run_program(char *memory);

int main(int argc, char *argv[]) {
  char program[MEMORY_LENGTH];
  FILE *in = stdin;

  if (argc == 2) {
    if ((in = fopen(argv[1], "r")) == NULL) {
      fprintf(stderr, "couldn't open file '%s'\n", argv[1]);
      exit(1);
    }
  } else if (argc > 2) {
    fprintf(stderr, "usage: %s [filename]\n", argv[0]);
    exit(1);
  }

  load_program(in, program);
  run_program(program);

  if (in != stdin) {
    fclose(in);
  }

  return 0;
}

void load_program(FILE *in, char *memory) {
  char instruct[3];
  int instruct_idx;
  int memory_idx;
  int ch;

  memset(memory, 0, MEMORY_LENGTH);

  instruct_idx = 0;
  memory_idx = 0;
  while (ch = fgetc(in)) {
    if (isdigit(ch)) {
      instruct[instruct_idx++] = ch;

      if (instruct_idx == sizeof(instruct)) {
        fprintf(stderr, "Error: program contains codes too large for 4-bit machine\n");
        fclose(in);
        exit(1);
      }
    } else if (instruct_idx > 0) {
      if (memory_idx == MEMORY_LENGTH) {
        fprintf(stderr, "Error: program too big\n");
        fclose(in);
        exit(1);
      }

      instruct[instruct_idx] = '\0';
      memory[memory_idx++] = atoi(instruct);

      if (memory[memory_idx - 1] < 0) {
        fprintf(stderr, "Error: program contains negative instructions\n");
        fclose(in);
        exit(1);
      } else if (memory[memory_idx - 1] > BYTE_LIMIT) {
        fprintf(stderr, "Error: program contains instructions that are too large\n");
        fclose(in);
        exit(1);
      }

      instruct_idx = 0;
    }

    if (ch == EOF) {
      break;
    }
  }

  fclose(in);
}

void run_program(char *memory) {
  int ip = 0;
  int is = 0;
  int r0 = 0;
  int r1 = 0;
  int operand;

  do {
    if (ip > BYTE_LIMIT) {
      is = 0;
    } else {
      is = memory[ip++];
    }

    if (is > 7) {
      operand = memory[ip++];
    }

    switch (is) {
    case I_HALT:
      break;

    case I_ADD:
      r0 = TRUNCATE_BYTE(r0 + r1);
      break;

    case I_SUB:
      r0 = TRUNCATE_BYTE(r0 - r1);
      break;

    case I_INC_R0:
      r0 = TRUNCATE_BYTE(r0 + 1);
      break;

    case I_INC_R1:
      r1 = TRUNCATE_BYTE(r1 + 1);
      break;

    case I_DEC_R0:
      r0 = TRUNCATE_BYTE(r0 - 1);
      break;

    case I_DEC_R1:
      r1 = TRUNCATE_BYTE(r1 - 1);
      break;

    case I_BELL:
#ifdef FAKE_BELL
      printf("DING ");
#else
      printf("\a");
#endif
      break;

    case I_PRINT:
      printf("%d ", memory[ip - 1]);
      break;

    case I_LOAD_R0:
      r0 = memory[operand];
      break;

    case I_LOAD_R1:
      r1 = memory[operand];
      break;

    case I_STORE_0:
      memory[operand] = r0;
      break;

    case I_STORE_1:
      memory[operand] = r1;
      break;

    case I_JUMP:
      ip = operand;
      break;

    case I_JUMP_ZERO:
      if (r0 == 0) {
        ip = operand;
      }
      break;

    case I_JUMP_NOT_ZERO:
      if (r0 != 0) {
        ip = operand;
      }
      break;
    }
  } while (is != 0);

  printf("\n");
}
