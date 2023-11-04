#define SMEM_INTERRUPT_SET_REG_ADDR 0x800e0110
#define SMEM_INTERRUPT_CLR_REG_ADDR 0x800e0120

#define SET_SMEM_INTERRUPT(interrupt_pntr)                                     \
  interrupt_pntr = (unsigned int *)(SMEM_INTERRUPT_SET_REG_ADDR);              \
  *interrupt_pntr = 0x1;                                                       \
  interrupt_pntr = (unsigned int *)(SMEM_INTERRUPT_CLR_REG_ADDR);              \
  *interrupt_pntr = 0x0;

#define UNSET_SMEM_INTERRUPT(interrupt_pntr)                                   \
  interrupt_pntr = (unsigned int *)(SMEM_INTERRUPT_CLR_REG_ADDR);              \
  *interrupt_pntr = 0x1;                                                       \
  interrupt_pntr = (unsigned int *)(SMEM_INTERRUPT_SET_REG_ADDR);              \
  *interrupt_pntr = 0x0;

#define set_MEIE asm volatile("csrs mie, %0" : : "r"((unsigned int)0x88B));

#define set_MSTATUS asm volatile("csrs mstatus, %0" : : "r"((unsigned int)0x8));

#define set_MTVEC(trap_handler)                                                \
  asm volatile("csrw mtvec, %0" : : "r"(&trap_handler));
