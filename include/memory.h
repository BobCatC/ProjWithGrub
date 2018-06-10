#ifndef _MEMORY_H
#define _MEMORY_H

#include "types.h"

bool fEqualStrings(const char* s1, const char* s2, size_t c);

void memcpy(__IN__ void* pDist, __IN__ const void* pScr, __IN__ size_t cbSize);

size_t strlen(__IN__ const char* szString);

bool atouh(const char* s, u32* pnum);


#define SWAP8(a, b)  {u8  c = a; a = b; b = c:}
#define SWAP16(a, b) {u16 c = a; a = b; b = c;}
#define SWAP32(a, b) {u32 c = a; a = b; b = c;}
#define SWAP64(a, b) {u64 c = a; a = b; b = c;}

#endif
