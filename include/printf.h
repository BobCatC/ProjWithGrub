#ifndef _PRINTF_H
#define _PRINTF_H

#include "stdarg.h"

extern "C++"{
	int vprintf(void* pCrtView, const char*, va_list);
}

#endif
