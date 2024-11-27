#include "conio.h"
#include "console.h"
#include "mmio.h"
#include "riscv_test.h"
#include "vsprintf.h"
#include <stdint.h>

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

unsigned long seed = 1;

void my_srand(unsigned long s) { seed = s; }

int my_rand() {
  seed = (1103515245 * seed + 12345) % (1 << 31);
  return (int)(seed & 0x7FFFFFFF);
}

// Function to generate a matrix with random values
void get_W_mat(int mat[N][N], unsigned seed) {
  my_srand(seed); // Seed the random number generator

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      int tmp = (my_rand() % 256) -
                128; // Generate a random number between -128 and 127
      mat[i][j] = tmp;
    }
  }
}

// Function to generate a matrix with random values for non-square matrices
void get_I_mat(int mat[N][M], unsigned seed) {
  my_srand(seed); // Seed the random number generator

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      int tmp = (my_rand() % 256) -
                128; // Generate a random number between -128 and 127
      mat[i][j] = tmp;
    }
  }
}

void get_B_mat(int mat[N][1], unsigned seed) {
  my_srand(seed); // Seed the random number generator

  for (int i = 0; i < N; ++i) {
    int tmp = (my_rand() % 256) -
              128; // Generate a random number between -128 and 127
    mat[i][0] = tmp;
  }
}

// Matrix multiplication
void mat_mul(int A[N][N], int B[N][M], int C[N][M]) {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      C[i][j] = 0;
      for (int k = 0; k < N; ++k) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

#define write_data(addr, data)                                                 \
  do {                                                                         \
    unsigned int *p = (unsigned int *)(addr);                                  \
    *p = data;                                                                 \
  } while (0)

#define write_data_64(addr, data)                                              \
  do {                                                                         \
    uint64_t *p = (uint64_t *)(addr);                                          \
    *p = data;                                                                 \
  } while (0)

#define read_data(addr, data) data = *(unsigned int *)(addr)

#define read_interrupt(interrupt) interrupt = *(unsigned int *)(0xA0000070)

#define wait_for_interrupt                                                     \
  do {                                                                         \
    unsigned interrupt = 0;                                                    \
    do {                                                                       \
      read_interrupt(interrupt);                                               \
    } while (interrupt == 0);                                                  \
  } while (0)

/** Main C Entry **/
int main() {
  // Init Console
  con_init();

  // --- Generate Input Matrix ---
  int W_mat[N][N];
  get_W_mat(W_mat, 0);
  int I_mat[N][M];
  get_I_mat(I_mat, 1);
  int B_mat[N][1];
  get_B_mat(B_mat, 2);

  int O_mat[N][M];
  mat_mul(I_mat, W_mat, O_mat);

  for (int j = 0; j < M; ++j) {
    for (int i = 0; i < N; ++i) {
      int tmp = B_mat[i][0];
      O_mat[i][j] = O_mat[i][j] + (tmp << BiasShift);
      if (IsRelu && O_mat[i][j] < 0)
        O_mat[i][j] = 0;
      else {
        O_mat[i][j] = O_mat[i][j] * AccumMul;
        O_mat[i][j] = O_mat[i][j] >> AccumShift;
        if (O_mat[i][j] > 127)
          O_mat[i][j] = 127;
        if (O_mat[i][j] < -128)
          O_mat[i][j] = -128;
      }
    }
  }

  // --- Load Input Matrix to SMEM ---
  uint64_t addr = w_rd_base;

  // --- data chart ---
  // (0x80064000)

  // Store bias
  // 64 bit CPU, write 8 bytes at a time
  for (int j = 0; j < N; j += 8) {
    uint64_t data = 0;
    for (int k = 0; k < 8; ++k) {
      if (j + k < N)
        data += (uint64_t)B_mat[j + k][0] << (k * 8);
    }
    write_data_64(addr, data);
    addr += 8;
  }

  // Store weight
  for (int j = N - 1; j >= 0; j--) {
    for (int i = 0; i < N; i += 8) {
      uint64_t data = 0;
      for (int k = 0; k < 8; ++k) {
        if (i + k < N)
          data += (uint64_t)W_mat[i + k][j] << (k * 8);
      }
      write_data_64(addr, data);
      addr += 8;
    }
  }

  // Store input
  // (0x80068000) 75c6_1665_b606_55a6_f645_95e6_3585_d626
  // (0x80068010) 7686_96а6_b6c6_d6e7_f707_1727_3747_5767

  addr = d_rd_base;
  for (int j = 0; j < M; j++) {
    for (int i = 0; i < N; i += 8) {
      uint64_t data = 0;
      for (int k = 0; k < 8; ++k) {
        if (i + k < N)
          data += (uint64_t)I_mat[i + k][j] << (k * 8);
      }
      write_data_64(addr, data);
      addr += 8;
    }
  }

  // --- Configuration write test ---
  // Computation Configuration
  unsigned int tmp = 0;
  unsigned int interrupt = 0;

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

  // 1--0x10: start signal (write only), fire interrupt upon completion
  // ----data=1 -> master weight read
  // ----data=2 -> master input read
  // ----data=3 -> master input write
  // ----data=4 -> start systolic array
  // master input read
  write_data(0xA0000010, 0x02);
  read_data(0xA0000010, tmp);
  wait_for_interrupt;

  // master weight read
  write_data(0xA0000010, 0x01);
  read_data(0xA0000010, tmp);
  wait_for_interrupt;

  // start computation
  write_data(0xA0000010, 0x04);
  read_data(0xA0000010, tmp);
  wait_for_interrupt;

  // master input write
  write_data(0xA0000010, 0x03);
  read_data(0xA0000010, tmp);
  wait_for_interrupt;

  // Exit Tb Simulation
  hartID_pntr = (unsigned int *)(0x10001024); // Magic Exit address
  *hartID_pntr = 0xCAFEDEED;                  // data must be !=0

  while (1)
    ;
  // return 0;
}
