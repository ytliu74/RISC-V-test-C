#include "conio.h"
#include "console.h"
#include "mmio.h"
#include "riscv_test.h"
#include "vsprintf.h"
#include <stdint.h>

void enable_dcache(void) {
  // Write 1 to bit 0 to enable the data cache.
  __asm__ volatile("csrw 0x701, %0" ::"r"(0x1));
}

void disable_dcache(void) {
  // Write 0 to disable the data cache.
  __asm__ volatile("csrw 0x701, %0" ::"r"(0x0));
}

/* Variable to close execution on testbench */
__attribute__((section(".tdata"))) volatile unsigned int *hartID_pntr;

#define N 11
#define BEATS 3

uint64_t INPUT_DATA[11][3] = {
    {0x0000000000000000, 0x0110000000000000, 0x00000016000001fd},
    {0x0000000000000000, 0x0000000000000000, 0x0008401800000045},
    {0x0000020007000000, 0x0003800000000001, 0x0008400600000000},
    {0x0100000000100071, 0x0000600100000000, 0x0008400600000000},
    {0x0000000000000000, 0x0000000000000000, 0x0088401800000007},
    {0x0000000000000000, 0x0007400100200000, 0x0088400600000000},
    {0x0000000000000000, 0x0000000000000000, 0x0088400e00000002},
    {0x0000000000000000, 0x0000000000000000, 0x0008400000000000},
    {0x0000000000000000, 0x0000000000000000, 0x0088400000000000},
    {0x0000000000000000, 0x0110000000000000, 0x0000001200000000},
    {0x0000000000000000, 0x0018000000000000, 0x0000001200000000}};

#define write_data(addr, data)                                                 \
  do {                                                                         \
    volatile unsigned *p = (volatile unsigned *)(addr);                        \
    *p = data;                                                                 \
  } while (0)

#define write_data_64(addr, data)                                              \
  do {                                                                         \
    uint64_t *p = (uint64_t *)(addr);                                          \
    *p = data;                                                                 \
  } while (0)

#define read_data(addr, data) data = *((volatile unsigned *)(addr))
#define read_data_64(addr, data) data = *((volatile uint64_t *)(addr))

/** Main C Entry **/
int main() {
  // Init Console
  con_init();

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < BEATS; j++) {
      write_data_64(0xA0000000, INPUT_DATA[i][j]);
    }
  }

  disable_dcache();

  uint64_t data;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < BEATS; j++)
      read_data_64(0xA0000000, data);

  enable_dcache();

  // Exit Tb Simulation
  hartID_pntr = (unsigned int *)(0x10001024); // Magic Exit address
  *hartID_pntr = 0xCAFEDEED;                  // data must be !=0

  while (1)
    ;
  // return 0;
}
