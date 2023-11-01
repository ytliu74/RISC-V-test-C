#include "conio.h"
#include "console.h"
#include "vsprintf.h"
#include "mmio.h"
#include "riscv_test.h"

/* Variable to close execution on testbench */
__attribute__ ((section(".tdata"))) volatile unsigned int *hartID_pntr;
__attribute__ ((section(".tdata"))) volatile unsigned int *interrupt_pntr;

#define N 4

/** Main C Entry **/
int main()
{
  // Init Console
  con_init();

  // Behavior
  int mat1[N][N]; // Initialize with your values
  int mat2[N][N]; // Initialize with your values
  int res[N][N]; // To store result
  int i, j, k;
  // Initialize mat1 and mat2 with some values
  for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
          mat1[i][j] = i + j;
          mat2[i][j] = i - j;
      }
  }
  
  // insert the interrupt_pntr
  interrupt_pntr = (unsigned int *)(0x800e0110);
  *interrupt_pntr = 0x00000001;

  for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
          res[i][j] = 0;
          for (k = 0; k < N; k++)
              res[i][j] += mat1[i][k] * mat2[k][j];
      }
  }
  cprintf("Result matrix is \n");
  for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
          cprintf("%d ", res[i][j]);
      }
      cprintf("\n");
  }

  // Exit Tb Simulation
  hartID_pntr = (unsigned int *)(0x10001024); // Magic Exit address
  *hartID_pntr = 0xCAFEDEED;                  // data must be !=0

  while(1);
  // return 0;
}
