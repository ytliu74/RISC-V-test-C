/*
 * Simplistic console driver
 *
 * Copyright (C) 2015 Intel corp. All rights reserved.
 *
 * Author: Stepan Karpenko <stepan.karpenko@intel.com>
 *
 */
#ifndef _CONSOLE_H_
#define _CONSOLE_H_


#ifdef __cplusplus
extern "C" {
#endif



int con_init(void);	/* init console */

int con_putc(int ch);	/* returns 0 on success, -1 otherwise */
int con_getc(void);	/* returns char value (0-255), -1 otherwise */
int con_puts(const char *str);			/* returns number of transferred chars */
int con_gets(char *str, int n, int flags);	/* returns number of transferred chars */

/* con_gets flags */
enum {
	CON_GETSF_NONBLK = 0x01,	/* Non-blocking input */
	CON_GETSF_CRSTOP = 0x02,	/* Stop on carriage return */
	CON_GETSF_ECHO   = 0x04		/* Produce echo */
};



#ifdef __cplusplus
} /* "C" */
#endif

#endif /* _CONSOLE_H_ */
