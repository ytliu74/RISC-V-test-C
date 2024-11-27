/*
 * Taken from NewOS Project. BSD-like license.
 */
#ifndef _CTYPE_H_
#define _CTYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

int isalnum(int c);
int isalpha(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int isascii(int c);
int toascii(int c);

unsigned char tolower_v(unsigned char c);
unsigned char toupper_v(unsigned char c);

#ifdef __cplusplus
} /* "C" */
#endif

#endif
