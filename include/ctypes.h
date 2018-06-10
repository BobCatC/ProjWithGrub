#ifndef _CTYPES_H
#define _CTYPES_H

#ifndef NULL
	#define NULL 0
#endif
typedef unsigned int size_t;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef u8 BYTE;
typedef u16 WORD;
typedef u32 DWORD;
typedef u64 LDWORD;

typedef u8 TCOLOUR;

#define __OUT__
#define __IN__

#define min(a, b) ((a) > (b) ? (b) : (a))
#define max(a, b) ((a) > (b) ? (a) : (b))

#ifndef __INT32_MAX__
	#define __INT32_MAX__		(0x7FFFFFFF)
#endif

#endif
