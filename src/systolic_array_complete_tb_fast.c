#include "conio.h"
#include "console.h"
#include "mmio.h"
#include "riscv_test.h"
#include "vsprintf.h"
#include <stdint.h>

#include "interrupt.h"

/* Variable to close execution on testbench */
__attribute__((section(".tdata"))) volatile unsigned int *test_pntr;
__attribute__((section(".tdata"))) volatile unsigned int *hartID_pntr;

#define N 16
#define M 16

const static int DATA_WIDTH = 128; // 128 bit AXI

// bias left shift,
const static int IsRelu = 1;
const static int BiasShift = 6;
const static int AccumShift = 10;
const static int AccumMul = 93;

// base address
const static unsigned int w_rd_base = 0x80064000;
const static unsigned int d_rd_base = 0x80068000;
const static unsigned int d_wr_base = 0x8006C000;
const uint64_t LOGGER_BASE_ADDR = 0x80070000;

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

volatile unsigned interrupt = 0;

void interrupt_handler(void) __attribute__((interrupt("machine")));
void interrupt_handler(void) {
  write_data(0xA0000000, 0); // clear interrupt
  interrupt = 1;
}

/** Main C Entry **/
int main() {
  // Init Console
  con_init();

  // Set up the trap handler
  set_MEIE;
  set_MSTATUS;
  set_MTVEC(interrupt_handler);

  // --- Write test matrics ---
  // clang-format off
  // int W_mat[16][16];
  // int I_mat[16][16];
  // int B_mat[16][1];
  // int O_mat[16][16];
  // W_mat[0][0]=4; W_mat[0][1]=-7; W_mat[0][2]=-16; W_mat[0][3]=6; W_mat[0][4]=3; W_mat[0][5]=-5; W_mat[0][6]=6; W_mat[0][7]=10; W_mat[0][8]=-19; W_mat[0][9]=-6; W_mat[0][10]=-11; W_mat[0][11]=-8; W_mat[0][12]=-6; W_mat[0][13]=-15; W_mat[0][14]=11; W_mat[0][15]=2; W_mat[1][0]=5; W_mat[1][1]=-6; W_mat[1][2]=17; W_mat[1][3]=-1; W_mat[1][4]=9; W_mat[1][5]=-17; W_mat[1][6]=9; W_mat[1][7]=-5; W_mat[1][8]=-3; W_mat[1][9]=-3; W_mat[1][10]=4; W_mat[1][11]=-2; W_mat[1][12]=4; W_mat[1][13]=-1; W_mat[1][14]=-2; W_mat[1][15]=5; W_mat[2][0]=-2; W_mat[2][1]=5; W_mat[2][2]=-17; W_mat[2][3]=-1; W_mat[2][4]=-8; W_mat[2][5]=16; W_mat[2][6]=-8; W_mat[2][7]=7; W_mat[2][8]=16; W_mat[2][9]=0; W_mat[2][10]=-6; W_mat[2][11]=3; W_mat[2][12]=-3; W_mat[2][13]=-14; W_mat[2][14]=6; W_mat[2][15]=9; W_mat[3][0]=7; W_mat[3][1]=-8; W_mat[3][2]=-8; W_mat[3][3]=-16; W_mat[3][4]=-8; W_mat[3][5]=3; W_mat[3][6]=-8; W_mat[3][7]=6; W_mat[3][8]=-5; W_mat[3][9]=6; W_mat[3][10]=-13; W_mat[3][11]=7; W_mat[3][12]=-6; W_mat[3][13]=23; W_mat[3][14]=-1; W_mat[3][15]=0; W_mat[4][0]=-1; W_mat[4][1]=-10; W_mat[4][2]=5; W_mat[4][3]=6; W_mat[4][4]=-3; W_mat[4][5]=3; W_mat[4][6]=9; W_mat[4][7]=-8; W_mat[4][8]=8; W_mat[4][9]=4; W_mat[4][10]=0; W_mat[4][11]=-6; W_mat[4][12]=-1; W_mat[4][13]=-10; W_mat[4][14]=-11; W_mat[4][15]=-6; W_mat[5][0]=-2; W_mat[5][1]=6; W_mat[5][2]=-8; W_mat[5][3]=-2; W_mat[5][4]=8; W_mat[5][5]=-3; W_mat[5][6]=-8; W_mat[5][7]=-22; W_mat[5][8]=1; W_mat[5][9]=-5; W_mat[5][10]=-7; W_mat[5][11]=5; W_mat[5][12]=7; W_mat[5][13]=4; W_mat[5][14]=-5; W_mat[5][15]=4; W_mat[6][0]=-5; W_mat[6][1]=-6; W_mat[6][2]=1; W_mat[6][3]=-14; W_mat[6][4]=6; W_mat[6][5]=4; W_mat[6][6]=-5; W_mat[6][7]=-13; W_mat[6][8]=15; W_mat[6][9]=-4; W_mat[6][10]=1; W_mat[6][11]=15; W_mat[6][12]=-2; W_mat[6][13]=2; W_mat[6][14]=0; W_mat[6][15]=1; W_mat[7][0]=5; W_mat[7][1]=1; W_mat[7][2]=-8; W_mat[7][3]=11; W_mat[7][4]=6; W_mat[7][5]=-3; W_mat[7][6]=11; W_mat[7][7]=-12; W_mat[7][8]=0; W_mat[7][9]=-2; W_mat[7][10]=-4; W_mat[7][11]=-2; W_mat[7][12]=3; W_mat[7][13]=-2; W_mat[7][14]=3; W_mat[7][15]=-11; W_mat[8][0]=-10; W_mat[8][1]=-4; W_mat[8][2]=0; W_mat[8][3]=-10; W_mat[8][4]=-5; W_mat[8][5]=-3; W_mat[8][6]=2; W_mat[8][7]=21; W_mat[8][8]=4; W_mat[8][9]=-1; W_mat[8][10]=5; W_mat[8][11]=-13; W_mat[8][12]=6; W_mat[8][13]=2; W_mat[8][14]=-7; W_mat[8][15]=16; W_mat[9][0]=1; W_mat[9][1]=-3; W_mat[9][2]=-16; W_mat[9][3]=6; W_mat[9][4]=-11; W_mat[9][5]=11; W_mat[9][6]=-4; W_mat[9][7]=-5; W_mat[9][8]=11; W_mat[9][9]=-8; W_mat[9][10]=5; W_mat[9][11]=4; W_mat[9][12]=6; W_mat[9][13]=16; W_mat[9][14]=-1; W_mat[9][15]=12; W_mat[10][0]=-20; W_mat[10][1]=11; W_mat[10][2]=11; W_mat[10][3]=2; W_mat[10][4]=10; W_mat[10][5]=-12; W_mat[10][6]=2; W_mat[10][7]=7; W_mat[10][8]=-6; W_mat[10][9]=0; W_mat[10][10]=5; W_mat[10][11]=4; W_mat[10][12]=7; W_mat[10][13]=-10; W_mat[10][14]=-7; W_mat[10][15]=-9; W_mat[11][0]=1; W_mat[11][1]=-16; W_mat[11][2]=-1; W_mat[11][3]=3; W_mat[11][4]=-5; W_mat[11][5]=-12; W_mat[11][6]=2; W_mat[11][7]=2; W_mat[11][8]=-12; W_mat[11][9]=1; W_mat[11][10]=10; W_mat[11][11]=19; W_mat[11][12]=9; W_mat[11][13]=2; W_mat[11][14]=-6; W_mat[11][15]=5; W_mat[12][0]=0; W_mat[12][1]=-4; W_mat[12][2]=14; W_mat[12][3]=-7; W_mat[12][4]=6; W_mat[12][5]=3; W_mat[12][6]=3; W_mat[12][7]=7; W_mat[12][8]=-8; W_mat[12][9]=-1; W_mat[12][10]=1; W_mat[12][11]=-10; W_mat[12][12]=-8; W_mat[12][13]=8; W_mat[12][14]=7; W_mat[12][15]=-8; W_mat[13][0]=5; W_mat[13][1]=3; W_mat[13][2]=-7; W_mat[13][3]=-9; W_mat[13][4]=3; W_mat[13][5]=1; W_mat[13][6]=-1; W_mat[13][7]=-2; W_mat[13][8]=4; W_mat[13][9]=-7; W_mat[13][10]=-7; W_mat[13][11]=0; W_mat[13][12]=22; W_mat[13][13]=-13; W_mat[13][14]=8; W_mat[13][15]=12; W_mat[14][0]=-15; W_mat[14][1]=-2; W_mat[14][2]=0; W_mat[14][3]=10; W_mat[14][4]=5; W_mat[14][5]=-5; W_mat[14][6]=9; W_mat[14][7]=-2; W_mat[14][8]=-4; W_mat[14][9]=0; W_mat[14][10]=-6; W_mat[14][11]=8; W_mat[14][12]=-16; W_mat[14][13]=1; W_mat[14][14]=0; W_mat[14][15]=12; W_mat[15][0]=13; W_mat[15][1]=3; W_mat[15][2]=-5; W_mat[15][3]=-2; W_mat[15][4]=-3; W_mat[15][5]=9; W_mat[15][6]=4; W_mat[15][7]=14; W_mat[15][8]=14; W_mat[15][9]=-1; W_mat[15][10]=1; W_mat[15][11]=-5; W_mat[15][12]=14; W_mat[15][13]=6; W_mat[15][14]=-5; W_mat[15][15]=8; 
  // I_mat[0][0]=3; I_mat[0][1]=16; I_mat[0][2]=-1; I_mat[0][3]=5; I_mat[0][4]=12; I_mat[0][5]=-9; I_mat[0][6]=0; I_mat[0][7]=-3; I_mat[0][8]=17; I_mat[0][9]=-13; I_mat[0][10]=15; I_mat[0][11]=-5; I_mat[0][12]=3; I_mat[0][13]=6; I_mat[0][14]=3; I_mat[0][15]=-16; I_mat[1][0]=-11; I_mat[1][1]=5; I_mat[1][2]=1; I_mat[1][3]=-4; I_mat[1][4]=-3; I_mat[1][5]=-3; I_mat[1][6]=5; I_mat[1][7]=2; I_mat[1][8]=-1; I_mat[1][9]=10; I_mat[1][10]=-7; I_mat[1][11]=-12; I_mat[1][12]=-7; I_mat[1][13]=-2; I_mat[1][14]=11; I_mat[1][15]=8; I_mat[2][0]=-6; I_mat[2][1]=-2; I_mat[2][2]=6; I_mat[2][3]=-2; I_mat[2][4]=-3; I_mat[2][5]=8; I_mat[2][6]=3; I_mat[2][7]=1; I_mat[2][8]=-16; I_mat[2][9]=0; I_mat[2][10]=-2; I_mat[2][11]=2; I_mat[2][12]=9; I_mat[2][13]=7; I_mat[2][14]=10; I_mat[2][15]=14; I_mat[3][0]=-7; I_mat[3][1]=-11; I_mat[3][2]=1; I_mat[3][3]=10; I_mat[3][4]=-3; I_mat[3][5]=-2; I_mat[3][6]=-9; I_mat[3][7]=16; I_mat[3][8]=-9; I_mat[3][9]=7; I_mat[3][10]=0; I_mat[3][11]=-6; I_mat[3][12]=-9; I_mat[3][13]=-20; I_mat[3][14]=2; I_mat[3][15]=2; I_mat[4][0]=7; I_mat[4][1]=-7; I_mat[4][2]=-10; I_mat[4][3]=5; I_mat[4][4]=9; I_mat[4][5]=-1; I_mat[4][6]=11; I_mat[4][7]=-2; I_mat[4][8]=-4; I_mat[4][9]=11; I_mat[4][10]=-12; I_mat[4][11]=4; I_mat[4][12]=-3; I_mat[4][13]=-6; I_mat[4][14]=-3; I_mat[4][15]=2; I_mat[5][0]=-9; I_mat[5][1]=9; I_mat[5][2]=-4; I_mat[5][3]=-3; I_mat[5][4]=3; I_mat[5][5]=-4; I_mat[5][6]=-4; I_mat[5][7]=4; I_mat[5][8]=7; I_mat[5][9]=-5; I_mat[5][10]=-5; I_mat[5][11]=-5; I_mat[5][12]=-14; I_mat[5][13]=14; I_mat[5][14]=1; I_mat[5][15]=8; I_mat[6][0]=3; I_mat[6][1]=4; I_mat[6][2]=5; I_mat[6][3]=5; I_mat[6][4]=-6; I_mat[6][5]=1; I_mat[6][6]=4; I_mat[6][7]=11; I_mat[6][8]=-7; I_mat[6][9]=-7; I_mat[6][10]=3; I_mat[6][11]=7; I_mat[6][12]=15; I_mat[6][13]=-3; I_mat[6][14]=2; I_mat[6][15]=-5; I_mat[7][0]=-8; I_mat[7][1]=6; I_mat[7][2]=-6; I_mat[7][3]=-8; I_mat[7][4]=-7; I_mat[7][5]=-4; I_mat[7][6]=8; I_mat[7][7]=20; I_mat[7][8]=12; I_mat[7][9]=-3; I_mat[7][10]=-24; I_mat[7][11]=-2; I_mat[7][12]=6; I_mat[7][13]=5; I_mat[7][14]=1; I_mat[7][15]=-2; I_mat[8][0]=10; I_mat[8][1]=12; I_mat[8][2]=-3; I_mat[8][3]=-11; I_mat[8][4]=2; I_mat[8][5]=9; I_mat[8][6]=4; I_mat[8][7]=15; I_mat[8][8]=5; I_mat[8][9]=-5; I_mat[8][10]=12; I_mat[8][11]=12; I_mat[8][12]=-4; I_mat[8][13]=10; I_mat[8][14]=-1; I_mat[8][15]=-2; I_mat[9][0]=-20; I_mat[9][1]=-24; I_mat[9][2]=-4; I_mat[9][3]=1; I_mat[9][4]=-2; I_mat[9][5]=-2; I_mat[9][6]=-12; I_mat[9][7]=5; I_mat[9][8]=-5; I_mat[9][9]=10; I_mat[9][10]=3; I_mat[9][11]=4; I_mat[9][12]=4; I_mat[9][13]=-6; I_mat[9][14]=6; I_mat[9][15]=-6; I_mat[10][0]=3; I_mat[10][1]=6; I_mat[10][2]=-7; I_mat[10][3]=2; I_mat[10][4]=-3; I_mat[10][5]=7; I_mat[10][6]=5; I_mat[10][7]=4; I_mat[10][8]=-7; I_mat[10][9]=5; I_mat[10][10]=-8; I_mat[10][11]=4; I_mat[10][12]=8; I_mat[10][13]=1; I_mat[10][14]=3; I_mat[10][15]=3; I_mat[11][0]=-3; I_mat[11][1]=-1; I_mat[11][2]=-15; I_mat[11][3]=4; I_mat[11][4]=-1; I_mat[11][5]=-5; I_mat[11][6]=-1; I_mat[11][7]=-14; I_mat[11][8]=-12; I_mat[11][9]=-17; I_mat[11][10]=0; I_mat[11][11]=-13; I_mat[11][12]=5; I_mat[11][13]=-2; I_mat[11][14]=-6; I_mat[11][15]=-1; I_mat[12][0]=-13; I_mat[12][1]=-10; I_mat[12][2]=7; I_mat[12][3]=-3; I_mat[12][4]=-3; I_mat[12][5]=-13; I_mat[12][6]=-3; I_mat[12][7]=-8; I_mat[12][8]=17; I_mat[12][9]=-10; I_mat[12][10]=10; I_mat[12][11]=0; I_mat[12][12]=10; I_mat[12][13]=-5; I_mat[12][14]=16; I_mat[12][15]=-8; I_mat[13][0]=-1; I_mat[13][1]=-5; I_mat[13][2]=-11; I_mat[13][3]=4; I_mat[13][4]=8; I_mat[13][5]=-8; I_mat[13][6]=-7; I_mat[13][7]=-9; I_mat[13][8]=-16; I_mat[13][9]=4; I_mat[13][10]=7; I_mat[13][11]=8; I_mat[13][12]=-3; I_mat[13][13]=9; I_mat[13][14]=-4; I_mat[13][15]=12; I_mat[14][0]=-2; I_mat[14][1]=1; I_mat[14][2]=5; I_mat[14][3]=6; I_mat[14][4]=1; I_mat[14][5]=-2; I_mat[14][6]=0; I_mat[14][7]=1; I_mat[14][8]=-9; I_mat[14][9]=4; I_mat[14][10]=6; I_mat[14][11]=8; I_mat[14][12]=1; I_mat[14][13]=9; I_mat[14][14]=5; I_mat[14][15]=3; I_mat[15][0]=-7; I_mat[15][1]=6; I_mat[15][2]=2; I_mat[15][3]=-5; I_mat[15][4]=-7; I_mat[15][5]=8; I_mat[15][6]=-22; I_mat[15][7]=1; I_mat[15][8]=-4; I_mat[15][9]=9; I_mat[15][10]=-2; I_mat[15][11]=-5; I_mat[15][12]=-4; I_mat[15][13]=-6; I_mat[15][14]=-8; I_mat[15][15]=1; 
  // B_mat[0][0]=7; B_mat[1][0]=-7; B_mat[2][0]=0; B_mat[3][0]=8; B_mat[4][0]=-5; B_mat[5][0]=12; B_mat[6][0]=3; B_mat[7][0]=-3; B_mat[8][0]=6; B_mat[9][0]=12; B_mat[10][0]=-6; B_mat[11][0]=-8; B_mat[12][0]=8; B_mat[13][0]=12; B_mat[14][0]=-2; B_mat[15][0]=11; 
  // O_mat[0][0]=52; O_mat[0][1]=43; O_mat[0][2]=69; O_mat[0][3]=66; O_mat[0][4]=32; O_mat[0][5]=24; O_mat[0][6]=45; O_mat[0][7]=62; O_mat[0][8]=81; O_mat[0][9]=48; O_mat[0][10]=8; O_mat[0][11]=24; O_mat[0][12]=39; O_mat[0][13]=2; O_mat[0][14]=22; O_mat[0][15]=0; O_mat[1][0]=0; O_mat[1][1]=0; O_mat[1][2]=0; O_mat[1][3]=0; O_mat[1][4]=0; O_mat[1][5]=0; O_mat[1][6]=0; O_mat[1][7]=0; O_mat[1][8]=0; O_mat[1][9]=0; O_mat[1][10]=0; O_mat[1][11]=0; O_mat[1][12]=11; O_mat[1][13]=0; O_mat[1][14]=0; O_mat[1][15]=0; O_mat[2][0]=0; O_mat[2][1]=50; O_mat[2][2]=0; O_mat[2][3]=0; O_mat[2][4]=0; O_mat[2][5]=6; O_mat[2][6]=0; O_mat[2][7]=41; O_mat[2][8]=63; O_mat[2][9]=0; O_mat[2][10]=0; O_mat[2][11]=0; O_mat[2][12]=0; O_mat[2][13]=22; O_mat[2][14]=0; O_mat[2][15]=0; O_mat[3][0]=41; O_mat[3][1]=47; O_mat[3][2]=9; O_mat[3][3]=42; O_mat[3][4]=76; O_mat[3][5]=10; O_mat[3][6]=18; O_mat[3][7]=0; O_mat[3][8]=52; O_mat[3][9]=19; O_mat[3][10]=68; O_mat[3][11]=48; O_mat[3][12]=36; O_mat[3][13]=99; O_mat[3][14]=10; O_mat[3][15]=43; O_mat[4][0]=0; O_mat[4][1]=0; O_mat[4][2]=0; O_mat[4][3]=0; O_mat[4][4]=0; O_mat[4][5]=0; O_mat[4][6]=0; O_mat[4][7]=2; O_mat[4][8]=0; O_mat[4][9]=0; O_mat[4][10]=0; O_mat[4][11]=0; O_mat[4][12]=0; O_mat[4][13]=0; O_mat[4][14]=0; O_mat[4][15]=0; O_mat[5][0]=86; O_mat[5][1]=51; O_mat[5][2]=66; O_mat[5][3]=77; O_mat[5][4]=93; O_mat[5][5]=59; O_mat[5][6]=51; O_mat[5][7]=0; O_mat[5][8]=65; O_mat[5][9]=77; O_mat[5][10]=113; O_mat[5][11]=55; O_mat[5][12]=37; O_mat[5][13]=46; O_mat[5][14]=57; O_mat[5][15]=71; O_mat[6][0]=57; O_mat[6][1]=37; O_mat[6][2]=0; O_mat[6][3]=4; O_mat[6][4]=37; O_mat[6][5]=37; O_mat[6][6]=25; O_mat[6][7]=0; O_mat[6][8]=0; O_mat[6][9]=0; O_mat[6][10]=46; O_mat[6][11]=32; O_mat[6][12]=9; O_mat[6][13]=54; O_mat[6][14]=0; O_mat[6][15]=29; O_mat[7][0]=4; O_mat[7][1]=0; O_mat[7][2]=0; O_mat[7][3]=16; O_mat[7][4]=0; O_mat[7][5]=0; O_mat[7][6]=0; O_mat[7][7]=0; O_mat[7][8]=0; O_mat[7][9]=0; O_mat[7][10]=21; O_mat[7][11]=0; O_mat[7][12]=0; O_mat[7][13]=0; O_mat[7][14]=0; O_mat[7][15]=0; O_mat[8][0]=21; O_mat[8][1]=53; O_mat[8][2]=44; O_mat[8][3]=0; O_mat[8][4]=0; O_mat[8][5]=56; O_mat[8][6]=24; O_mat[8][7]=80; O_mat[8][8]=69; O_mat[8][9]=51; O_mat[8][10]=0; O_mat[8][11]=56; O_mat[8][12]=56; O_mat[8][13]=47; O_mat[8][14]=30; O_mat[8][15]=36; O_mat[9][0]=73; O_mat[9][1]=104; O_mat[9][2]=48; O_mat[9][3]=62; O_mat[9][4]=77; O_mat[9][5]=57; O_mat[9][6]=17; O_mat[9][7]=60; O_mat[9][8]=81; O_mat[9][9]=51; O_mat[9][10]=111; O_mat[9][11]=67; O_mat[9][12]=29; O_mat[9][13]=85; O_mat[9][14]=43; O_mat[9][15]=72; O_mat[10][0]=0; O_mat[10][1]=0; O_mat[10][2]=0; O_mat[10][3]=0; O_mat[10][4]=0; O_mat[10][5]=0; O_mat[10][6]=13; O_mat[10][7]=0; O_mat[10][8]=0; O_mat[10][9]=0; O_mat[10][10]=0; O_mat[10][11]=0; O_mat[10][12]=0; O_mat[10][13]=0; O_mat[10][14]=0; O_mat[10][15]=0; O_mat[11][0]=0; O_mat[11][1]=0; O_mat[11][2]=0; O_mat[11][3]=0; O_mat[11][4]=0; O_mat[11][5]=0; O_mat[11][6]=0; O_mat[11][7]=0; O_mat[11][8]=0; O_mat[11][9]=0; O_mat[11][10]=0; O_mat[11][11]=0; O_mat[11][12]=6; O_mat[11][13]=0; O_mat[11][14]=0; O_mat[11][15]=0; O_mat[12][0]=54; O_mat[12][1]=47; O_mat[12][2]=48; O_mat[12][3]=54; O_mat[12][4]=58; O_mat[12][5]=50; O_mat[12][6]=77; O_mat[12][7]=53; O_mat[12][8]=17; O_mat[12][9]=64; O_mat[12][10]=17; O_mat[12][11]=76; O_mat[12][12]=61; O_mat[12][13]=87; O_mat[12][14]=54; O_mat[12][15]=79; O_mat[13][0]=60; O_mat[13][1]=95; O_mat[13][2]=101; O_mat[13][3]=48; O_mat[13][4]=64; O_mat[13][5]=52; O_mat[13][6]=61; O_mat[13][7]=46; O_mat[13][8]=127; O_mat[13][9]=41; O_mat[13][10]=97; O_mat[13][11]=58; O_mat[13][12]=77; O_mat[13][13]=69; O_mat[13][14]=90; O_mat[13][15]=30; O_mat[14][0]=0; O_mat[14][1]=0; O_mat[14][2]=0; O_mat[14][3]=5; O_mat[14][4]=0; O_mat[14][5]=18; O_mat[14][6]=0; O_mat[14][7]=3; O_mat[14][8]=0; O_mat[14][9]=24; O_mat[14][10]=0; O_mat[14][11]=0; O_mat[14][12]=0; O_mat[14][13]=0; O_mat[14][14]=0; O_mat[14][15]=16; O_mat[15][0]=45; O_mat[15][1]=112; O_mat[15][2]=58; O_mat[15][3]=31; O_mat[15][4]=66; O_mat[15][5]=41; O_mat[15][6]=54; O_mat[15][7]=101; O_mat[15][8]=127; O_mat[15][9]=31; O_mat[15][10]=77; O_mat[15][11]=68; O_mat[15][12]=66; O_mat[15][13]=93; O_mat[15][14]=80; O_mat[15][15]=36; 
  int W_mat[16][16];
W_mat[0][0] = -8; W_mat[0][1] = 7; W_mat[0][2] = 2; W_mat[0][3] = -1; W_mat[0][4] = -11; W_mat[0][5] = 2; W_mat[0][6] = -4; W_mat[0][7] = 0; W_mat[0][8] = -3; W_mat[0][9] = 13; W_mat[0][10] = -6; W_mat[0][11] = -7; W_mat[0][12] = 5; W_mat[0][13] = -3; W_mat[0][14] = 2; W_mat[0][15] = -2; W_mat[1][0] = -4; W_mat[1][1] = -10; W_mat[1][2] = -4; W_mat[1][3] = -8; W_mat[1][4] = 5; W_mat[1][5] = -6; W_mat[1][6] = -4; W_mat[1][7] = 3; W_mat[1][8] = 15; W_mat[1][9] = 5; W_mat[1][10] = -3; W_mat[1][11] = 9; W_mat[1][12] = -2; W_mat[1][13] = 5; W_mat[1][14] = -10; W_mat[1][15] = -11; W_mat[2][0] = 10; W_mat[2][1] = -3; W_mat[2][2] = 3; W_mat[2][3] = -7; W_mat[2][4] = -2; W_mat[2][5] = -3; W_mat[2][6] = 14; W_mat[2][7] = 15; W_mat[2][8] = 7; W_mat[2][9] = -7; W_mat[2][10] = 0; W_mat[2][11] = 17; W_mat[2][12] = 3; W_mat[2][13] = -3; W_mat[2][14] = -8; W_mat[2][15] = -5; W_mat[3][0] = -13; W_mat[3][1] = 13; W_mat[3][2] = 5; W_mat[3][3] = 8; W_mat[3][4] = -2; W_mat[3][5] = -14; W_mat[3][6] = 3; W_mat[3][7] = 2; W_mat[3][8] = -5; W_mat[3][9] = 12; W_mat[3][10] = 7; W_mat[3][11] = -8; W_mat[3][12] = 2; W_mat[3][13] = -4; W_mat[3][14] = -16; W_mat[3][15] = 5; W_mat[4][0] = -2; W_mat[4][1] = 5; W_mat[4][2] = -4; W_mat[4][3] = -8; W_mat[4][4] = 8; W_mat[4][5] = 15; W_mat[4][6] = 4; W_mat[4][7] = -7; W_mat[4][8] = 2; W_mat[4][9] = -7; W_mat[4][10] = 7; W_mat[4][11] = 3; W_mat[4][12] = 2; W_mat[4][13] = 3; W_mat[4][14] = -2; W_mat[4][15] = -13; W_mat[5][0] = -3; W_mat[5][1] = -10; W_mat[5][2] = -1; W_mat[5][3] = 1; W_mat[5][4] = 3; W_mat[5][5] = -11; W_mat[5][6] = -9; W_mat[5][7] = -8; W_mat[5][8] = -11; W_mat[5][9] = -14; W_mat[5][10] = -2; W_mat[5][11] = 8; W_mat[5][12] = 11; W_mat[5][13] = 1; W_mat[5][14] = -7; W_mat[5][15] = -3; W_mat[6][0] = -3; W_mat[6][1] = -5; W_mat[6][2] = 18; W_mat[6][3] = 4; W_mat[6][4] = -1; W_mat[6][5] = 19; W_mat[6][6] = -2; W_mat[6][7] = -14; W_mat[6][8] = -3; W_mat[6][9] = 14; W_mat[6][10] = -8; W_mat[6][11] = 5; W_mat[6][12] = -1; W_mat[6][13] = -9; W_mat[6][14] = 4; W_mat[6][15] = -9; W_mat[7][0] = 10; W_mat[7][1] = 10; W_mat[7][2] = -7; W_mat[7][3] = 3; W_mat[7][4] = 5; W_mat[7][5] = 10; W_mat[7][6] = 12; W_mat[7][7] = 1; W_mat[7][8] = 5; W_mat[7][9] = -4; W_mat[7][10] = 0; W_mat[7][11] = -5; W_mat[7][12] = 1; W_mat[7][13] = 11; W_mat[7][14] = 10; W_mat[7][15] = 9; W_mat[8][0] = -5; W_mat[8][1] = 2; W_mat[8][2] = 6; W_mat[8][3] = -9; W_mat[8][4] = -4; W_mat[8][5] = 3; W_mat[8][6] = 6; W_mat[8][7] = -4; W_mat[8][8] = 2; W_mat[8][9] = 5; W_mat[8][10] = -5; W_mat[8][11] = 2; W_mat[8][12] = -15; W_mat[8][13] = 0; W_mat[8][14] = 0; W_mat[8][15] = 5; W_mat[9][0] = -2; W_mat[9][1] = 5; W_mat[9][2] = -6; W_mat[9][3] = 22; W_mat[9][4] = 6; W_mat[9][5] = 6; W_mat[9][6] = 0; W_mat[9][7] = 4; W_mat[9][8] = 3; W_mat[9][9] = 8; W_mat[9][10] = -8; W_mat[9][11] = 9; W_mat[9][12] = -2; W_mat[9][13] = 8; W_mat[9][14] = 8; W_mat[9][15] = 1; W_mat[10][0] = 6; W_mat[10][1] = -8; W_mat[10][2] = 5; W_mat[10][3] = 17; W_mat[10][4] = -3; W_mat[10][5] = 3; W_mat[10][6] = -4; W_mat[10][7] = -7; W_mat[10][8] = -14; W_mat[10][9] = 9; W_mat[10][10] = 2; W_mat[10][11] = 1; W_mat[10][12] = -13; W_mat[10][13] = 3; W_mat[10][14] = -23; W_mat[10][15] = 12; W_mat[11][0] = 9; W_mat[11][1] = 3; W_mat[11][2] = -4; W_mat[11][3] = -6; W_mat[11][4] = 0; W_mat[11][5] = -16; W_mat[11][6] = 1; W_mat[11][7] = 3; W_mat[11][8] = 2; W_mat[11][9] = -9; W_mat[11][10] = -2; W_mat[11][11] = -10; W_mat[11][12] = -4; W_mat[11][13] = -5; W_mat[11][14] = 2; W_mat[11][15] = -18; W_mat[12][0] = -1; W_mat[12][1] = -3; W_mat[12][2] = 6; W_mat[12][3] = 7; W_mat[12][4] = 18; W_mat[12][5] = 6; W_mat[12][6] = -1; W_mat[12][7] = 4; W_mat[12][8] = -3; W_mat[12][9] = -2; W_mat[12][10] = 9; W_mat[12][11] = -7; W_mat[12][12] = -9; W_mat[12][13] = 5; W_mat[12][14] = -13; W_mat[12][15] = -9; W_mat[13][0] = -6; W_mat[13][1] = 7; W_mat[13][2] = -7; W_mat[13][3] = -2; W_mat[13][4] = -8; W_mat[13][5] = -8; W_mat[13][6] = 0; W_mat[13][7] = -1; W_mat[13][8] = -6; W_mat[13][9] = 9; W_mat[13][10] = 9; W_mat[13][11] = 8; W_mat[13][12] = -4; W_mat[13][13] = 1; W_mat[13][14] = 10; W_mat[13][15] = 1; W_mat[14][0] = 6; W_mat[14][1] = -11; W_mat[14][2] = -6; W_mat[14][3] = 2; W_mat[14][4] = -11; W_mat[14][5] = -19; W_mat[14][6] = 5; W_mat[14][7] = 12; W_mat[14][8] = 5; W_mat[14][9] = -6; W_mat[14][10] = -5; W_mat[14][11] = -21; W_mat[14][12] = 0; W_mat[14][13] = 14; W_mat[14][14] = -7; W_mat[14][15] = 12; W_mat[15][0] = -3; W_mat[15][1] = -12; W_mat[15][2] = -15; W_mat[15][3] = -11; W_mat[15][4] = 10; W_mat[15][5] = 8; W_mat[15][6] = 7; W_mat[15][7] = -6; W_mat[15][8] = 7; W_mat[15][9] = -2; W_mat[15][10] = -2; W_mat[15][11] = 13; W_mat[15][12] = -6; W_mat[15][13] = 13; W_mat[15][14] = -1; W_mat[15][15] = 3; 
int I_mat[16][16];
I_mat[0][0] = -14; I_mat[0][1] = 11; I_mat[0][2] = -11; I_mat[0][3] = 10; I_mat[0][4] = 16; I_mat[0][5] = 10; I_mat[0][6] = 11; I_mat[0][7] = 8; I_mat[0][8] = -8; I_mat[0][9] = -8; I_mat[0][10] = 9; I_mat[0][11] = -3; I_mat[0][12] = -5; I_mat[0][13] = -10; I_mat[0][14] = -11; I_mat[0][15] = 18; I_mat[1][0] = -5; I_mat[1][1] = 9; I_mat[1][2] = -18; I_mat[1][3] = 4; I_mat[1][4] = 3; I_mat[1][5] = 2; I_mat[1][6] = 6; I_mat[1][7] = 8; I_mat[1][8] = 2; I_mat[1][9] = 6; I_mat[1][10] = -1; I_mat[1][11] = -1; I_mat[1][12] = 28; I_mat[1][13] = 2; I_mat[1][14] = -7; I_mat[1][15] = -15; I_mat[2][0] = -4; I_mat[2][1] = -11; I_mat[2][2] = -4; I_mat[2][3] = 7; I_mat[2][4] = -10; I_mat[2][5] = -2; I_mat[2][6] = 4; I_mat[2][7] = -5; I_mat[2][8] = -18; I_mat[2][9] = 1; I_mat[2][10] = 6; I_mat[2][11] = -5; I_mat[2][12] = -5; I_mat[2][13] = 0; I_mat[2][14] = -6; I_mat[2][15] = -4; I_mat[3][0] = 4; I_mat[3][1] = -5; I_mat[3][2] = 7; I_mat[3][3] = -7; I_mat[3][4] = -2; I_mat[3][5] = -11; I_mat[3][6] = 9; I_mat[3][7] = 4; I_mat[3][8] = 8; I_mat[3][9] = 0; I_mat[3][10] = -13; I_mat[3][11] = -2; I_mat[3][12] = -7; I_mat[3][13] = -5; I_mat[3][14] = -4; I_mat[3][15] = -17; I_mat[4][0] = 5; I_mat[4][1] = 6; I_mat[4][2] = 7; I_mat[4][3] = -7; I_mat[4][4] = -2; I_mat[4][5] = 19; I_mat[4][6] = 11; I_mat[4][7] = -8; I_mat[4][8] = -2; I_mat[4][9] = -4; I_mat[4][10] = -12; I_mat[4][11] = -2; I_mat[4][12] = -9; I_mat[4][13] = 4; I_mat[4][14] = -5; I_mat[4][15] = -13; I_mat[5][0] = 2; I_mat[5][1] = 2; I_mat[5][2] = -8; I_mat[5][3] = 10; I_mat[5][4] = 10; I_mat[5][5] = -11; I_mat[5][6] = 0; I_mat[5][7] = -1; I_mat[5][8] = -5; I_mat[5][9] = -4; I_mat[5][10] = 0; I_mat[5][11] = 9; I_mat[5][12] = 3; I_mat[5][13] = 3; I_mat[5][14] = -14; I_mat[5][15] = 4; I_mat[6][0] = 1; I_mat[6][1] = 10; I_mat[6][2] = -2; I_mat[6][3] = -4; I_mat[6][4] = -5; I_mat[6][5] = 0; I_mat[6][6] = 12; I_mat[6][7] = 10; I_mat[6][8] = -14; I_mat[6][9] = -10; I_mat[6][10] = 0; I_mat[6][11] = -7; I_mat[6][12] = -5; I_mat[6][13] = -13; I_mat[6][14] = -1; I_mat[6][15] = -1; I_mat[7][0] = -1; I_mat[7][1] = 4; I_mat[7][2] = -9; I_mat[7][3] = -5; I_mat[7][4] = -8; I_mat[7][5] = -6; I_mat[7][6] = 2; I_mat[7][7] = 7; I_mat[7][8] = 0; I_mat[7][9] = 1; I_mat[7][10] = 3; I_mat[7][11] = -5; I_mat[7][12] = 2; I_mat[7][13] = 1; I_mat[7][14] = -8; I_mat[7][15] = 5; I_mat[8][0] = 1; I_mat[8][1] = 8; I_mat[8][2] = 14; I_mat[8][3] = 0; I_mat[8][4] = 10; I_mat[8][5] = -15; I_mat[8][6] = 4; I_mat[8][7] = 7; I_mat[8][8] = 0; I_mat[8][9] = 9; I_mat[8][10] = -5; I_mat[8][11] = 7; I_mat[8][12] = 7; I_mat[8][13] = -3; I_mat[8][14] = -9; I_mat[8][15] = 2; I_mat[9][0] = -3; I_mat[9][1] = 15; I_mat[9][2] = -3; I_mat[9][3] = 1; I_mat[9][4] = -7; I_mat[9][5] = -3; I_mat[9][6] = -7; I_mat[9][7] = 8; I_mat[9][8] = -8; I_mat[9][9] = 3; I_mat[9][10] = -6; I_mat[9][11] = -1; I_mat[9][12] = 13; I_mat[9][13] = -2; I_mat[9][14] = 3; I_mat[9][15] = 13; I_mat[10][0] = -10; I_mat[10][1] = -14; I_mat[10][2] = 2; I_mat[10][3] = -4; I_mat[10][4] = 7; I_mat[10][5] = 7; I_mat[10][6] = -16; I_mat[10][7] = 5; I_mat[10][8] = 1; I_mat[10][9] = -4; I_mat[10][10] = -2; I_mat[10][11] = 2; I_mat[10][12] = 5; I_mat[10][13] = 8; I_mat[10][14] = -3; I_mat[10][15] = -8; I_mat[11][0] = 4; I_mat[11][1] = -13; I_mat[11][2] = -3; I_mat[11][3] = -7; I_mat[11][4] = -12; I_mat[11][5] = 0; I_mat[11][6] = 7; I_mat[11][7] = 11; I_mat[11][8] = 7; I_mat[11][9] = 16; I_mat[11][10] = 3; I_mat[11][11] = 8; I_mat[11][12] = -4; I_mat[11][13] = -2; I_mat[11][14] = 11; I_mat[11][15] = -3; I_mat[12][0] = -2; I_mat[12][1] = 7; I_mat[12][2] = -15; I_mat[12][3] = -18; I_mat[12][4] = -8; I_mat[12][5] = 6; I_mat[12][6] = 21; I_mat[12][7] = -6; I_mat[12][8] = -2; I_mat[12][9] = 1; I_mat[12][10] = -15; I_mat[12][11] = 5; I_mat[12][12] = -8; I_mat[12][13] = -1; I_mat[12][14] = -20; I_mat[12][15] = -3; I_mat[13][0] = -4; I_mat[13][1] = -9; I_mat[13][2] = -4; I_mat[13][3] = -9; I_mat[13][4] = 4; I_mat[13][5] = 1; I_mat[13][6] = -1; I_mat[13][7] = 0; I_mat[13][8] = -2; I_mat[13][9] = 3; I_mat[13][10] = 5; I_mat[13][11] = 17; I_mat[13][12] = 2; I_mat[13][13] = -16; I_mat[13][14] = 8; I_mat[13][15] = 4; I_mat[14][0] = 6; I_mat[14][1] = -5; I_mat[14][2] = -11; I_mat[14][3] = -2; I_mat[14][4] = -8; I_mat[14][5] = -6; I_mat[14][6] = 7; I_mat[14][7] = -7; I_mat[14][8] = -5; I_mat[14][9] = -5; I_mat[14][10] = 16; I_mat[14][11] = -9; I_mat[14][12] = 11; I_mat[14][13] = 2; I_mat[14][14] = 8; I_mat[14][15] = -9; I_mat[15][0] = -16; I_mat[15][1] = -5; I_mat[15][2] = 1; I_mat[15][3] = -2; I_mat[15][4] = 13; I_mat[15][5] = -16; I_mat[15][6] = 4; I_mat[15][7] = -7; I_mat[15][8] = -4; I_mat[15][9] = 8; I_mat[15][10] = -11; I_mat[15][11] = -5; I_mat[15][12] = -11; I_mat[15][13] = 0; I_mat[15][14] = -18; I_mat[15][15] = -15; 
int B_mat[16][1];
B_mat[0][0] = -9; B_mat[1][0] = 5; B_mat[2][0] = 18; B_mat[3][0] = 2; B_mat[4][0] = 9; B_mat[5][0] = -12; B_mat[6][0] = 7; B_mat[7][0] = -6; B_mat[8][0] = -1; B_mat[9][0] = -3; B_mat[10][0] = -24; B_mat[11][0] = -12; B_mat[12][0] = -3; B_mat[13][0] = -8; B_mat[14][0] = -9; B_mat[15][0] = 7; 
int O_mat[16][16];
O_mat[0][0] = 0; O_mat[0][1] = 0; O_mat[0][2] = 0; O_mat[0][3] = 0; O_mat[0][4] = 0; O_mat[0][5] = 0; O_mat[0][6] = 0; O_mat[0][7] = 0; O_mat[0][8] = 0; O_mat[0][9] = 0; O_mat[0][10] = 0; O_mat[0][11] = 0; O_mat[0][12] = 0; O_mat[0][13] = 0; O_mat[0][14] = 0; O_mat[0][15] = 0; O_mat[1][0] = 52; O_mat[1][1] = 38; O_mat[1][2] = 76; O_mat[1][3] = 14; O_mat[1][4] = 15; O_mat[1][5] = 42; O_mat[1][6] = 10; O_mat[1][7] = 48; O_mat[1][8] = 47; O_mat[1][9] = 55; O_mat[1][10] = 24; O_mat[1][11] = 64; O_mat[1][12] = 23; O_mat[1][13] = 21; O_mat[1][14] = 64; O_mat[1][15] = 77; O_mat[2][0] = 100; O_mat[2][1] = 114; O_mat[2][2] = 90; O_mat[2][3] = 94; O_mat[2][4] = 86; O_mat[2][5] = 116; O_mat[2][6] = 127; O_mat[2][7] = 127; O_mat[2][8] = 92; O_mat[2][9] = 114; O_mat[2][10] = 122; O_mat[2][11] = 107; O_mat[2][12] = 77; O_mat[2][13] = 80; O_mat[2][14] = 94; O_mat[2][15] = 127; O_mat[3][0] = 0; O_mat[3][1] = 23; O_mat[3][2] = 22; O_mat[3][3] = 0; O_mat[3][4] = 0; O_mat[3][5] = 12; O_mat[3][6] = 0; O_mat[3][7] = 25; O_mat[3][8] = 16; O_mat[3][9] = 26; O_mat[3][10] = 0; O_mat[3][11] = 0; O_mat[3][12] = 33; O_mat[3][13] = 22; O_mat[3][14] = 0; O_mat[3][15] = 0; O_mat[4][0] = 71; O_mat[4][1] = 55; O_mat[4][2] = 43; O_mat[4][3] = 57; O_mat[4][4] = 64; O_mat[4][5] = 87; O_mat[4][6] = 50; O_mat[4][7] = 56; O_mat[4][8] = 54; O_mat[4][9] = 37; O_mat[4][10] = 58; O_mat[4][11] = 85; O_mat[4][12] = 72; O_mat[4][13] = 60; O_mat[4][14] = 54; O_mat[4][15] = 54; O_mat[5][0] = 0; O_mat[5][1] = 0; O_mat[5][2] = 0; O_mat[5][3] = 0; O_mat[5][4] = 0; O_mat[5][5] = 0; O_mat[5][6] = 0; O_mat[5][7] = 0; O_mat[5][8] = 0; O_mat[5][9] = 0; O_mat[5][10] = 0; O_mat[5][11] = 0; O_mat[5][12] = 0; O_mat[5][13] = 0; O_mat[5][14] = 0; O_mat[5][15] = 0; O_mat[6][0] = 69; O_mat[6][1] = 39; O_mat[6][2] = 34; O_mat[6][3] = 80; O_mat[6][4] = 8; O_mat[6][5] = 21; O_mat[6][6] = 42; O_mat[6][7] = 29; O_mat[6][8] = 5; O_mat[6][9] = 36; O_mat[6][10] = 49; O_mat[6][11] = 41; O_mat[6][12] = 44; O_mat[6][13] = 52; O_mat[6][14] = 48; O_mat[6][15] = 58; O_mat[7][0] = 0; O_mat[7][1] = 0; O_mat[7][2] = 0; O_mat[7][3] = 0; O_mat[7][4] = 9; O_mat[7][5] = 0; O_mat[7][6] = 10; O_mat[7][7] = 0; O_mat[7][8] = 0; O_mat[7][9] = 0; O_mat[7][10] = 0; O_mat[7][11] = 0; O_mat[7][12] = 0; O_mat[7][13] = 0; O_mat[7][14] = 0; O_mat[7][15] = 0; O_mat[8][0] = 0; O_mat[8][1] = 0; O_mat[8][2] = 6; O_mat[8][3] = 29; O_mat[8][4] = 0; O_mat[8][5] = 0; O_mat[8][6] = 0; O_mat[8][7] = 1; O_mat[8][8] = 0; O_mat[8][9] = 3; O_mat[8][10] = 20; O_mat[8][11] = 0; O_mat[8][12] = 15; O_mat[8][13] = 0; O_mat[8][14] = 20; O_mat[8][15] = 5; O_mat[9][0] = 5; O_mat[9][1] = 0; O_mat[9][2] = 0; O_mat[9][3] = 0; O_mat[9][4] = 0; O_mat[9][5] = 0; O_mat[9][6] = 20; O_mat[9][7] = 1; O_mat[9][8] = 0; O_mat[9][9] = 1; O_mat[9][10] = 0; O_mat[9][11] = 0; O_mat[9][12] = 0; O_mat[9][13] = 0; O_mat[9][14] = 0; O_mat[9][15] = 0; O_mat[10][0] = 0; O_mat[10][1] = 0; O_mat[10][2] = 0; O_mat[10][3] = 0; O_mat[10][4] = 0; O_mat[10][5] = 0; O_mat[10][6] = 0; O_mat[10][7] = 0; O_mat[10][8] = 0; O_mat[10][9] = 0; O_mat[10][10] = 0; O_mat[10][11] = 0; O_mat[10][12] = 0; O_mat[10][13] = 0; O_mat[10][14] = 0; O_mat[10][15] = 0; O_mat[11][0] = 0; O_mat[11][1] = 0; O_mat[11][2] = 0; O_mat[11][3] = 0; O_mat[11][4] = 0; O_mat[11][5] = 0; O_mat[11][6] = 0; O_mat[11][7] = 0; O_mat[11][8] = 0; O_mat[11][9] = 0; O_mat[11][10] = 0; O_mat[11][11] = 0; O_mat[11][12] = 0; O_mat[11][13] = 0; O_mat[11][14] = 0; O_mat[11][15] = 0; O_mat[12][0] = 0; O_mat[12][1] = 0; O_mat[12][2] = 17; O_mat[12][3] = 0; O_mat[12][4] = 0; O_mat[12][5] = 21; O_mat[12][6] = 0; O_mat[12][7] = 0; O_mat[12][8] = 0; O_mat[12][9] = 0; O_mat[12][10] = 0; O_mat[12][11] = 0; O_mat[12][12] = 0; O_mat[12][13] = 0; O_mat[12][14] = 0; O_mat[12][15] = 0; O_mat[13][0] = 0; O_mat[13][1] = 0; O_mat[13][2] = 0; O_mat[13][3] = 0; O_mat[13][4] = 0; O_mat[13][5] = 0; O_mat[13][6] = 0; O_mat[13][7] = 0; O_mat[13][8] = 0; O_mat[13][9] = 0; O_mat[13][10] = 0; O_mat[13][11] = 0; O_mat[13][12] = 2; O_mat[13][13] = 0; O_mat[13][14] = 0; O_mat[13][15] = 0; O_mat[14][0] = 0; O_mat[14][1] = 0; O_mat[14][2] = 0; O_mat[14][3] = 0; O_mat[14][4] = 0; O_mat[14][5] = 0; O_mat[14][6] = 0; O_mat[14][7] = 0; O_mat[14][8] = 0; O_mat[14][9] = 0; O_mat[14][10] = 0; O_mat[14][11] = 0; O_mat[14][12] = 0; O_mat[14][13] = 0; O_mat[14][14] = 0; O_mat[14][15] = 0; O_mat[15][0] = 57; O_mat[15][1] = 33; O_mat[15][2] = 76; O_mat[15][3] = 23; O_mat[15][4] = 60; O_mat[15][5] = 45; O_mat[15][6] = 36; O_mat[15][7] = 44; O_mat[15][8] = 50; O_mat[15][9] = 51; O_mat[15][10] = 43; O_mat[15][11] = 84; O_mat[15][12] = 14; O_mat[15][13] = 19; O_mat[15][14] = 74; O_mat[15][15] = 62; 

  // clang-format on

  // --- Load Input Matrix to SMEM ---
  uint64_t addr = w_rd_base;

  // --- data chart ---
  // (0x80064000)

  // Store bias
  // 64 bit CPU, write 8 bytes at a time
  for (int j = 0; j < N; j += 8) {
    uint64_t data = 0;
    for (int k = 0; k < 8; k++) {
      if (j + k < N)
        data += ((uint64_t)B_mat[j + k][0] & 0xFF) << (k * 8);
    }
    write_data_64(addr, data);
    addr += 8;
  }

  // Store weight
  for (int j = N - 1; j >= 0; j--) {
    for (int i = 0; i < N; i += 8) {
      uint64_t data = 0;
      for (int k = 0; k < 8; k++) {
        if (i + k < N)
          data += ((uint64_t)W_mat[i + k][j] & 0xFF) << (k * 8);
      }
      write_data_64(addr, data);
      addr += 8;
    }
  }

  // Store input
  addr = d_rd_base;
  for (int j = 0; j < M; j++) {
    for (int i = 0; i < N; i += 8) {
      uint64_t data = 0;
      for (int k = 0; k < 8; ++k) {
        if (i + k < N)
          data += ((uint64_t)I_mat[i + k][j] & 0xFF) << (k * 8);
      }
      write_data_64(addr, data);
      addr += 8;
    }
  }

  // --- Configuration write test ---
  // Computation Configuration
  volatile unsigned tmp = 0;

  unsigned config = 0;
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

  // 1--0x10: start signal (write only), fire interrupt upon completion
  // ----data=1 -> master weight read
  // ----data=2 -> master input read
  // ----data=3 -> master input write
  // ----data=4 -> start systolic array
  // master input read
  write_data(0xA0000010, 0x02);
  read_data(0xA0000010, tmp);
  while (!interrupt)
    ;
  interrupt = 0;

  // master weight read
  write_data(0xA0000010, 0x01);
  read_data(0xA0000010, tmp);
  while (!interrupt)
    ;
  interrupt = 0;

  // start computation
  write_data(0xA0000010, 0x04);
  read_data(0xA0000010, tmp);
  while (!interrupt)
    ;
  interrupt = 0;

  // master input write
  write_data(0xA0000010, 0x03);
  read_data(0xA0000010, tmp);
  while (!interrupt)
    ;
  interrupt = 0;

  // Check if the result is correct
  addr = d_wr_base;

  for (int j = 0; j < M; j++) {
    for (int i = 0; i < N; i += 64 / 8) {
      uint64_t data;
      read_data_64(addr, data);
      addr += 8;
      for (int k = 0; k < 64 / 8; ++k) {
        if (i + k < N) {
          int ref = O_mat[i + k][j];
          int result = (data >> (k * 8)) & 0xFF;
          // Set a logger data to check the result
          // The first 16 bits are the reference value
          // The second 16 bits are the result value
          // The third 16 bits are the col index
          // The last 16 bits are the row index
          uint64_t logger_data = 0;
          logger_data |= (uint64_t)ref;
          logger_data |= (uint64_t)result << 16;
          logger_data |= (uint64_t)j << 32;
          logger_data |= (uint64_t)(i + k) << 48;
          write_data_64(LOGGER_BASE_ADDR, logger_data);
        }
      }
    }
  }
  write_data(LOGGER_BASE_ADDR + 0x10, 0);

  // Exit Tb Simulation
  hartID_pntr = (unsigned int *)(0x10001024); // Magic Exit address
  *hartID_pntr = 0xCAFEDEED;                  // data must be !=0

  while (1)
    ;
  // return 0;
}
