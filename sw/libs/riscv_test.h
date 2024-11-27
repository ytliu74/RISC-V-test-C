// Add user macros, routines in this file

#ifndef _CARYSTUS_CORE_H
#define _CARYSTUS_CORE_H

# define CHECK_XLEN                                                     \
  li a0, 1;                                                             \
  slli a0, a0, 63;                                                      \
  bne zero, a0, xlen_pass;                                              \
xlen_fail:                                                              \
  j fail;                                                               \
xlen_pass:

#define INIT_XREG                                                       \
  li x1, 0;                                                             \
  li x2, 0x80080000;                                                             \
  li x3, 0;                                                             \
  li x4, 0;                                                             \
  li x5, 0;                                                             \
  li x6, 0;                                                             \
  li x7, 0;                                                             \
  li x8, 0;                                                             \
  li x9, 0;                                                             \
  li x10, 0;                                                            \
  li x11, 0;                                                            \
  li x12, 0;                                                            \
  li x13, 0;                                                            \
  li x14, 0;                                                            \
  li x15, 0;                                                            \
  li x16, 0;                                                            \
  li x17, 0;                                                            \
  li x18, 0;                                                            \
  li x19, 0;                                                            \
  li x20, 0;                                                            \
  li x21, 0;                                                            \
  li x22, 0;                                                            \
  li x23, 0;                                                            \
  li x24, 0;                                                            \
  li x25, 0;                                                            \
  li x26, 0;                                                            \
  li x27, 0;                                                            \
  li x28, 0;                                                            \
  li x29, 0;                                                            \
  li x30, 0;                                                            \
  li x31, 0

#define RVTEST_CODE_BEGIN                                               \
        .section .text.init;                                            \
        .option norvc;                                                  \
        .globl rvtest_entry_point;                                      \
reset_vector:                                                           \
        INIT_XREG;                                                      \
        j main

//-----------------------------------------------------------------------
// End Macro
//-----------------------------------------------------------------------

#define RVTEST_CODE_END                                                 \
        unimp

//-----------------------------------------------------------------------
// Pass/Fail Macro
//-----------------------------------------------------------------------

#define TEST_PASSFAIL                                                   \
        bne zero, gp, pass;                                             \
        j fail

//-----------------------------------------------------------------------
// Data Section Macro
//-----------------------------------------------------------------------

#define TEST_DATA                                                       \
        .align 4;                                                       \
rvtest_data:                                                            \
        .word 0xbabecafe;                                               \
        .word 0x00000000;                                               \
        .word 0x00000000;                                               \
        .word 0x00000000

#define EXTRA_DATA

#define RVTEST_DATA_BEGIN                                               \
        EXTRA_DATA                                                      \
        .pushsection .tohost,"aw",@progbits;                            \
        .align 6; .global tohost; tohost: .dword 0;                     \
        .align 6; .global fromhost; fromhost: .dword 0;                 \
        .popsection;                                                    \
        .align 4; .global begin_signature; begin_signature:

#define RVTEST_DATA_END .align 4; .global end_signature; end_signature:

#endif // _CARYSTUS_CORE_H
