#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define BYTE_LENGTH 4
#define BYTE_LIMIT ((1 << BYTE_LENGTH) - 1)
#define MEMORY_LENGTH 16

#define TRUNCATE_BYTE(x) ((x) % (BYTE_LIMIT + 1))

void run_program(char *memory) {
  int ip = 0;
  int is = 0;
  int r0 = 0;
  int r1 = 0;
  int operand;

  int pc = 0;

  bool _exit = false;
  while (!_exit) {
    printf("mem[%d] = %d\n", pc, memory[pc]);

    switch (memory[pc]) {
    case 0:
      _exit = true;
      break;

    case 1:
      r0 = TRUNCATE_BYTE(r0 + r1);
      pc += 1;
      break;

    case 2:
      r0 = TRUNCATE_BYTE(r0 - r1);
      pc += 1;
      break;

    case 3:
      r0 = TRUNCATE_BYTE(r0 + 1);
      pc += 1;
      break;

    case 4:
      r1 = TRUNCATE_BYTE(r1 + 1);
      pc += 1;
      break;

    case 5:
      r0 = TRUNCATE_BYTE(r0 - 1);
      pc += 1;
      break;

    case 6:
      r1 = TRUNCATE_BYTE(r1 - 1);
      pc += 1;
      break;

    case 7:
      printf("DING \n");
      pc += 1;
      break;

    case 8:
      printf("%d ", memory[ip - 1]);
      pc += 2;
      break;

    case 9:
      r0 = memory[operand];
      pc += 2;
      break;

    case 10:
      r1 = memory[operand];
      pc += 2;
      break;

    case 11:
      memory[operand] = r0;
      pc += 2;
      break;

    case 12:
      memory[operand] = r1;
      pc += 2;
      break;

    case 13:
      ip = operand;
      pc += 2;
      break;

    case 14:
      if (r0 == 0) {
        ip = operand;
      }
      pc += 2;
      break;

    case 15:
      if (r0 != 0) {
        ip = operand;
      }
      pc += 2;
      break;
    }
  }

  printf("\n");
}
int main(int argc, char *argv[]) {
  char program[MEMORY_LENGTH];
  char p[16] = {7, 7, 7, 0};

  run_program(p);

  return 0;
}
