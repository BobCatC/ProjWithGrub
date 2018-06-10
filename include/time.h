#ifndef TIME_H
#define TIME_H

#include "types.h"

struct TSC2{
    u32 t1h;
    u32 t1l;
    u32 t2h;
    u32 t2l;
};

struct TSC{
    u64 t1;
    u64 t2;
};

struct TIME_T{
    u32 High;
    u32 Low;
};
#endif
