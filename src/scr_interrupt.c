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

void trap_handler(void) {
  cprintf("Trap Handler\n");

  interrupt_pntr = (unsigned int *)(0x800e0110);
  *interrupt_pntr = 0x00000000;

  interrupt_pntr = (unsigned int *)(0x800e0120);
  *interrupt_pntr = 0x00000001;

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
  int a;
  a = 0;
  cprintf("a = %d\n", a);

  // insert the interrupt_pntr
  interrupt_pntr = (unsigned int *)(0x800e0110);
  *interrupt_pntr = 0x00000001;

  for (int i = 0; i < 5; i++)
    cprintf("i = %d\n", i);

  // Exit Tb Simulation
  hartID_pntr = (unsigned int *)(0x10001024); // Magic Exit address
  *hartID_pntr = 0xCAFEDEED;                  // data must be !=0

  while (1)
    ;
  // return 0;
}