#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define DEBUG 0
#define dprintf(fmt, ...) do {                                  \
                if (DEBUG) {                                    \
                        fprintf(stdout, fmt, ##__VA_ARGS__);    \
                }                                               \
        } while (0)

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
        char tmp = nibble_to_byte(cpu.pc_h, cpu.pc_l) + val;

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

void execute(char *memory)
{
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

                dprintf("\n------------------------------------------------------------\n");
                dprintf("PC = %d, pc[h] = %d, pc[l] = %d ", pc, cpu.pc_h, cpu.pc_l);
                dprintf("r0 = %d, r1 = %d, r2 = %d, r3 = %d, z = %d\n ", cpu.r[0], cpu.r[1], cpu.r[2], cpu.r[3], cpu.z);

                switch (memory[pc]) {
                case 0:
                        _exit = true;
                        printf("Exit application.\n");
                        break;

                case 1:
                        dprintf("print\n");
                        printf("%d x %d = %d\n", cpu.r[0], cpu.r[1], nibble_to_byte(cpu.r[2], cpu.r[3]));
                        cpu = cpu_pc_add(cpu, 1);
                        break;

                case 2:
                        tmp1 = memory[pc + 1];
                        assert((tmp1 < 2) && "Whoops, we only support r0,r1 register in INC command.");
                        dprintf("inc %s\n", (tmp1 == 0)? "r0" : "r1");
                        cpu.r[tmp1] += 1;
                        cpu = cpu_pc_add(cpu, 2);
                        break;

                case 3:
                        // NOTE: we cheat here, just made MUL command as R1 * R2
                        dprintf("mul r0 r1\n");
                        tmp1 = cpu.r[0] * cpu.r[1];
                        cpu.r[2] = tmp1 >> 4;
                        cpu.r[3] = tmp1 & 0x0f;
                        cpu = cpu_pc_add(cpu, 3);
                        break;

                case 4:
                        tmp1 = memory[pc + 1]; // rx
                        tmp2 = memory[pc + 2]; // value
                        assert((tmp1 < 2) && "Whoops, we only support r0,r1 register in MOV command.");
                        dprintf("mov %s %d\n", (tmp1 == 0)? "r0" : "r1", tmp2);
                        cpu.r[tmp1] = tmp2;
                        cpu = cpu_pc_add(cpu, 3);
                        break;

                case 5:
                        tmp1 = memory[pc + 1]; // rx
                        tmp2 = memory[pc + 2]; // value
                        assert((tmp1 < 2) && "Whoops, we only support r0,r1 register in CMP command.");
                        dprintf("cmp %s %d\n", (tmp1 == 0)? "r0" : "r1", tmp2);
                        cpu.z = (cpu.r[tmp1] >= tmp2) ? 1 : 0;
                        cpu = cpu_pc_add(cpu, 3);
                        break;

                case 6:
                        dprintf("blt %d %d\n", memory[pc + 1], memory[pc + 2]);
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
                //           _init:
                4, 0, 1,  //   mov r0 1      ; i = 1
                //
                //            _for_i: 3
                4, 1, 1,  //   mov r1 1    ; j = 1
                //
                //            _for_j: 6
                3, 0, 1,  //   mul r0 r1   ; i * j
                1,        //   print       ; print("R1 x R2 = Result")
                2, 1,     //   inc r1      ; j++
                5, 1, 10, //   cmp r1 10   ; if (j < 10)
                6, 0, 6,  //   blt _for_j  ;   goto _for_j
                //
                2, 0,     //   inc r0      ; i++
                5, 0, 10, //   cmp r0 10   ; if (i < 10)
                6, 0, 3,  //   blt _for_i  ;   goto _for_i
                //
                0,        //   exit        ; Exit application
        };

        execute(program);

        return 0;
}
