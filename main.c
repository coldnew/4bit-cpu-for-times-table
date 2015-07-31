#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// Registers:
//
//   R0     -   tmp data
//   R1     -   tmp data
//   R2     -   ALU result (High)
//   R3     -   ALU result (Low)
//   PC_L   -   Program Counter (Low)
//   PC_H   -   Program Counter (High)
//   Z       - flag for cmp (equal: z = 1)

struct CPU {
  char r[4]; // r0 ~ r3
  char pc_h;
  char pc_l;
  char z;
};

char nibble_to_byte(char high, char low)
{
  return (high << 4) | (low & 0x0f);
}

// Increase pc value, if pc_l greater than 10, pc_h += 1
struct CPU cpu_pc_add(struct CPU cpu, char val)
{
  char tmp = cpu.pc_l + val;

  cpu.pc_h = tmp >> 4;
  cpu.pc_l = (tmp & 0x0f);

  assert(tmp == nibble_to_byte(cpu.pc_h, cpu.pc_l));
  return cpu;
}

// Get the real pc value
char cpu_pc_get(struct CPU cpu)
{
  return nibble_to_byte(cpu.pc_h, cpu.pc_l);
}

void execute(char *memory) {

  // create new cpu and init
  struct CPU cpu = {
    .pc_h = 0,
    .pc_l = 0,
    .z = 0
  };

  int pc = 0, tmp1 = 0, tmp2 = 0;
  bool _exit = false;

  while (!_exit) {
    pc = cpu_pc_get(cpu);
//    printf("PC = %d, pc[h] = %d, pc[l] = %d\n", pc, cpu.pc_h, cpu.pc_l);

    switch (memory[pc]) {
    case 0:
      _exit = true;
      printf("Exit application.\n");
      break;

    case 1:
      printf("%d x %d = %d\n", cpu.r[0], cpu.r[1], nibble_to_byte(cpu.r[2], cpu.r[3]));
      cpu = cpu_pc_add(cpu, 1);
      break;

    case 2:
      tmp1 = memory[pc + 1];
      assert((tmp1 < 2) && "Whoops, we only support r1,r2 register in INC command.");
      cpu.r[tmp1] += 1;
      cpu = cpu_pc_add(cpu, 2);
      break;

    case 3:
      // NOTE: we cheat here, just made MUL command as R1 * R2
      tmp1 = cpu.r[0] * cpu.r[1];
      cpu.r[2] = tmp1 >> 4;
      cpu.r[3] = tmp1 & 0x0f;
      cpu = cpu_pc_add(cpu, 3);
      break;

    case 4:
      tmp1 = memory[pc + 1]; // rx
      tmp2 = memory[pc + 2]; // value
      assert((tmp1 < 2) && "Whoops, we only support r1,r2 register in MOV command.");
      cpu.r[tmp1] = tmp2;
      cpu = cpu_pc_add(cpu, 3);
      break;

    case 5:
      tmp1 = memory[pc + 1]; // rx
      tmp2 = memory[pc + 2]; // value
      assert((tmp1 < 2) && "Whoops, we only support r1,r2 register in CMP command.");
      cpu.z = (cpu.r[tmp1] == cpu.r[tmp2]) ? 1 : 0;
      cpu = cpu_pc_add(cpu, 3);
      break;

    case 6:
      if (0 == cpu.z) {
        cpu.pc_h = memory[pc + 1]; // PC_H
        cpu.pc_l = memory[pc + 2]; // PC_L
      }
      else {
        cpu = cpu_pc_add(cpu, 3);
      }
      cpu.z = 0;
      break;
    }
  }

  printf("\n");
}

int main(int argc, char *argv[]) {

  char program[] = {
    4, 0, 0,  // mov r0 0      ; i = 0
              //
              // _for_i: 3
    4, 1, 1,  //   mov r1 1    ; j = 1
    2, 0,     //   inc r0      ; i++
              //
              // _for_j: 8
    3, 0, 1,  //   mul r0 r1   ; i * j
    1,        //   print       ; print("R1 x R2 = Result")
    2, 1,     //   inc r1      ; j++
    5, 1, 10, //   cmp r1 9   ; if (j <= 9)
    6, 0, 8,  //   bne _for_j  ;   goto _for_j
              //
    5, 0, 9,  //   cmp r0 9   ; if (i <= 9)
    6, 0, 3,  //   bne _for_i  ;   goto _for_i
              //
    0,        //   exit        ; Exit application
  };

  execute(program);

  return 0;
}
