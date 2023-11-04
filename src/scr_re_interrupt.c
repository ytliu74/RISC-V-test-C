#include "conio.h"
#include "console.h"
#include "mmio.h"
#include "riscv_test.h"
#include "vsprintf.h"

#include "interrupt.h"

/* Variable to close execution on testbench */
__attribute__((section(".tdata"))) volatile unsigned int *interrupt_pntr;
__attribute__((section(".tdata"))) volatile unsigned int *hartID_pntr;
// __attribute__((section(".tdata"))) volatile unsigned int
// *trap_handler_address

void trap_handler(void) {
  cprintf("Trap Handler\n");

  UNSET_SMEM_INTERRUPT(interrupt_pntr);

  asm volatile("ld ra,8(sp) \n"
               "addi sp,sp,16 \n"
               "mret");
}

/** Main C Entry **/
int main() {
  // Init Console
  con_init();

  set_MEIE;
  set_MSTATUS;
  set_MTVEC(trap_handler);

  // Behavior
  int a;
  a = 0;
  cprintf("a = %d\n", a);

  // insert the interrupt_pntr
  SET_SMEM_INTERRUPT(interrupt_pntr);

  for (int i = 0; i < 3; i++)
    cprintf("i = %d\n", i);

  // insert the interrupt_pntr
  SET_SMEM_INTERRUPT(interrupt_pntr);

  for (int i = 0; i < 3; i++)
    cprintf("i = %d\n", i);

  // Exit Tb Simulation
  hartID_pntr = (unsigned int *)(0x10001024); // Magic Exit address
  *hartID_pntr = 0xCAFEDEED;                  // data must be !=0

  while (1)
    ;
  // return 0;
}