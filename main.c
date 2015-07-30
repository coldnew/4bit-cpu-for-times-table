#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define BYTE_LENGTH 4
#define BYTE_LIMIT ((1 << BYTE_LENGTH) - 1)
#define MEMORY_LENGTH 16

#define TRUNCATE_BYTE(x) ((x) % (BYTE_LIMIT + 1))

// Registers:
//
//   R1     -   tmp data
//   R2     -   tmp data
//   R3     -   ALU result (High)
//   R4     -   ALU result (Low)
//   PC_L   -   Program Counter (Low)
//   PC_H   -   Program Counter (High)
//   Z       - flag for cmp (equal: z = 1)

// Instructions:
//
//  opcode       -     description
//       0       -
//       1       -
//       2       -
//       3       -
//       4       -
//       5       -
//       6       -
//       7       -
//       8       -
//       9       -
//      10       -
//      11       -
//      12       -
//      13       -
//      14       -
//      15       -


//  3 nibble ?
//  MOV Rx ??
//  ADD Rx ??
//  MUL R1 R2
//  INC Rx    /// Rx += 1
//  DEC Rx    /// Rx -= 1
//  JMP H L   /// Jump to ?
//  Print
//  cmp Rx ??
//  beq H L
//  bne H L

///////
///
/// _INIT:
//  mov r1 1 ; i = 1
//
//  _for_i:
//  mov r2 1 ; j = 1
//  inc r1   ; i++
//
//  _for_j:
//  mul r1 r2 ; i * j
//  print
//  inc r2 ; j++
//  cmp r2 9
//  bne _for_j
//
//  cmp r1 9
//  bne _for_i


void execute(char *memory) {
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

  execute(p);

  return 0;
}
