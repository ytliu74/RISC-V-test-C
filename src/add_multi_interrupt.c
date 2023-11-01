#include "conio.h"
#include "console.h"
#include "vsprintf.h"
#include "mmio.h"
#include "riscv_test.h"

/* Variable to close execution on testbench */
__attribute__ ((section(".tdata"))) volatile unsigned int *interrupt_pntr;
__attribute__ ((section(".tdata"))) volatile unsigned int *hartID_pntr;

/** Main C Entry **/
int main()
{
  // Init Console
  con_init();

  // Behavior
  int a, b;
  a = 10;
  cprintf("a = %d\n", a);

  // insert the interrupt_pntr
  interrupt_pntr = (unsigned int *)(0x800e0110);
  *interrupt_pntr = 0x00000001;

  b = 15;
  cprintf("b = %d\n", b);

  // Exit Tb Simulation
  hartID_pntr = (unsigned int *)(0x10001024); // Magic Exit address
  *hartID_pntr = 0xCAFEDEED;                  // data must be !=0

  while(1);
 //return 0;
}