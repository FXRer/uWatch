#ifndef _XPRINT_H_
#define _XPRINT_H_

#ifndef __ASSEMBLER__
#define out_dev(func) out_func = (void(*)(unsigned char))(func)
extern void (*out_func)(unsigned char);

void xprint(char*, ...);
void xsprint(char*,char*,...);
#endif

//#define SUPPORT_LONG

#define SUPPORT_FIXED

#define SUPPORT_HEX
#define SUPPORT_UNSIGNED
#define SUPPORT_SIGNED
#define SUPPORT_STR
#define SUPPORT_CHAR
//#define SUPPORT_OCTAL
#define SUPPORT_BINARY


#endif // _XPRINT_H_
