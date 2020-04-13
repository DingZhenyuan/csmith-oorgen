#include "rand48.h"

extern unsigned short _rand48_seed[3];

long
lrand48(void)
{
	_dorand48(_rand48_seed);
    // 将unsigned short数组返回成一个long的随机数
	return ((long) _rand48_seed[2] << 15) + ((long) _rand48_seed[1] >> 1);
}