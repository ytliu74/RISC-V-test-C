#include "conio.h"
#include "console.h"
#include "mmio.h"
#include "riscv_test.h"
#include "vsprintf.h"

/* Variable to close execution on testbench */
__attribute__((section(".tdata"))) volatile unsigned int *interrupt_pntr;
__attribute__((section(".tdata"))) volatile unsigned int *hartID_pntr;
// __attribute__((section(".tdata"))) volatile unsigned int
// *trap_handler_address

#define SET_SMEM_INTERRUPT                                                     \
  interrupt_pntr = (unsigned int *)(0x800e0110);                               \
  *interrupt_pntr = 0x1;                                                       \
  interrupt_pntr = (unsigned int *)(0x800e0120);                               \
  *interrupt_pntr = 0x0;

#define UNSET_SMEM_INTERRUPT                                                   \
  interrupt_pntr = (unsigned int *)(0x800e0120);                               \
  *interrupt_pntr = 0x1;                                                       \
  interrupt_pntr = (unsigned int *)(0x800e0110);                               \
  *interrupt_pntr = 0x0;

void trap_handler(void) {
  cprintf("Trap Handler\n");

  UNSET_SMEM_INTERRUPT;

  asm volatile("ld ra,8(sp) \n"
               "addi sp,sp,16 \n"
               "mret");
}

#define set_MEIE asm volatile("csrs mie, %0" : : "r"((unsigned int)0x88B));

#define set_MSTATUS asm volatile("csrs mstatus, %0" : : "r"((unsigned int)0x8));

#define set_MTVEC asm volatile("csrw mtvec, %0" : : "r"(&trap_handler));

/** Main C Entry **/
int main() {
  // Init Console
  con_init();

  set_MEIE;
  set_MSTATUS;
  set_MTVEC;

  // Behavior
  int a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int b[10] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

  int sum =  0;
  
  for (int i = 0; i < 5; i++) {
    sum += a[i] * b[i];
  }
  // insert the interrupt_pntr
  SET_SMEM_INTERRUPT;

  for (int i = 5; i < 10; i++) {
    sum += a[i] * b[i];
  }

  cprintf("sum = %d\n", sum);

  // Exit Tb Simulation
  hartID_pntr = (unsigned int *)(0x10001024); // Magic Exit address
  *hartID_pntr = 0xCAFEDEED;                  // data must be !=0

  while (1)
    ;
  // return 0;
}