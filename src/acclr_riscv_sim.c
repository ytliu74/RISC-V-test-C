#include "conio.h"
#include "console.h"
#include "vsprintf.h"
#include "mmio.h"
#include "riscv_test.h"

/* Variable to close execution on testbench */
__attribute__ ((section(".tdata"))) volatile unsigned int *hartID_pntr;
__attribute__ ((section(".tdata"))) volatile unsigned int *test_pntr;
__attribute__ ((section(".tdata"))) volatile unsigned int *test_pntr1;


/** Main C Entry **/
int main()
{
  // Init Console
  con_init();

  // generate start for accelerator
  test_pntr = (unsigned int *)(0x800e0110);
  *test_pntr = 0x00000001;
  
  //check done
  test_pntr1 = (unsigned int *)(0x8006FFFE);  

   while(*test_pntr1 != 0xDEADBEEF)
  {
     test_pntr1 = (unsigned int *)(0x8006FFFE);
  }         
  
  // Exit Tb Simulation
  hartID_pntr = (unsigned int *)(0x10001024); // Magic Exit address
  *hartID_pntr = 0xCAFEDEED;                  // data must be !=0

  while(1);
 //return 0;
}
