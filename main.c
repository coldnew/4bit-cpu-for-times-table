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

  char program[] = {
    4, 0, 0,  // mov r1 0      ; i = 0
              //
              // _for_i: 3
    4, 1, 1,  //   mov r1 1    ; j = 1
    3, 0,     //   inc r1      ; i++
              //
              // _for_j: 8
    2, 0, 1,  //   mul r1 r2   ; i * j
    1,        //   print       ; print("R1 x R2 = Result")
    3, 1,     //   inc r2      ; j++
    5, 1, 10, //   cmp r2 10   ; if (j <= 10)
    6, 0, 8,  //   bne _for_j  ;   goto _for_j
              //
    5, 0, 10, //   cmp r1 10   ; if (i <= 10)
    6, 0, 3,  //   bnc _for_i  ;   goto _for_i
              //
    0,        //   exit        ; Exit application
  };

  execute(program);

  return 0;
}
