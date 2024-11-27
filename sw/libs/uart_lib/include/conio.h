/*
 * Formatted console I/O
 *
 * Copyright (C) 2014 Intel corp. All rights reserved.
 *
 * Author: Stepan Karpenko <stepan.karpenko@intel.com>
 *
 */
#ifndef _CONIO_H_
#define _CONIO_H_

#ifdef __cplusplus
extern "C" {
#endif


/* same as standard printf() */
int cprintf(const char *fmt, ...);
/* same as standard scanf() */
int cscanf(const char *fmt, ...);


#ifdef __cplusplus
} /* "C" */
#endif

#endif /* _CONIO_H_ */
