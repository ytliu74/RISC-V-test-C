#include "conio.h"
#include "console.h"
#include "mmio.h"
#include "riscv_test.h"
#include "vsprintf.h"

/* Variable to close execution on testbench */
__attribute__((section(".tdata"))) volatile unsigned int *test_pntr;
__attribute__((section(".tdata"))) volatile unsigned int *hartID_pntr;

const static int N = 8;
const static int M = N;

// bias left shift,
const static int IsRelu = 1;
const static int BiasShift = 6;
const static int AccumShift = 10;
const static int AccumMul = 93;

// base address
const static unsigned int w_rd_base = 0x80064000;
const static unsigned int d_rd_base = 0x80068000;
const static unsigned int d_wr_base = 0x8006C000;

#define write_data(addr, data)                                                 \
  do {                                                                         \
    unsigned int *p = (unsigned int *)(addr);                                  \
    *p = data;                                                                 \
  } while (0)

#define read_data(addr, data) data = *(unsigned int *)(addr)

#define read_interrupt(interrupt) interrupt = *(unsigned int *)(0xA0000070)

// #define check_data(addr, data_ref)                                             \
//   do {                                                                         \
//     unsigned int data_read = 0;                                                \
//     read_data(addr, data_read);                                                \
//     if (data_read == data_ref) {                                               \
//       cprintf("Succeed\n");                                                    \
//     } else {                                                                   \
//       cprintf("Failed\n");                                                     \
//     }                                                                          \
//   } while (0)

/** Main C Entry **/
int main() {
  // Init Console
  con_init();

  // --- Configuration write test ---
  // Computation Configuration
  unsigned int tmp = 0;

  unsigned int config = 0;
  config += (M - 1);
  config += IsRelu << 8;
  config += BiasShift << 16;
  config += AccumShift << 20;
  config += AccumMul << 24;
  write_data(0xA0000020, config);
  read_data(0xA0000020, tmp);

  // Address Configuration
  // cprintf("Write Weight Read Base Address\n");
  write_data(0xA0000030, w_rd_base);
  read_data(0xA0000030, tmp);

  // cprintf("Write Data Read Base Address\n");
  write_data(0xA0000040, d_rd_base);
  read_data(0xA0000040, tmp);

  // cprintf("Write Data Write Base Address\n");
  write_data(0xA0000050, d_wr_base);
  read_data(0xA0000050, tmp);


  // Exit Tb Simulation
  hartID_pntr = (unsigned int *)(0x10001024); // Magic Exit address
  *hartID_pntr = 0xCAFEDEED;                  // data must be !=0

  while (1)
    ;
  // return 0;
}
