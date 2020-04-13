#include "rand48.h"

unsigned short _rand48_seed[3] = {
    RAND48_SEED_0,
    RAND48_SEED_1,
    RAND48_SEED_2
};

unsigned short _rand48_mult[3] = {
    RAND48_MULT_0,
    RAND48_MULT_1,
    RAND48_MULT_2
};

unsigned short _rand48_add = RAND48_ADD;

// 对xseed进行随机化处理
void _dorand48(unsigned short xseed[3]) {
    unsigned long accu;
    unsigned short temp[2];
    
    accu = (unsigned long) _rand48_mult[0] * (unsigned long) xseed[0] + (unsigned long) _rand48_add;
	temp[0] = (unsigned short) accu; // 第十六位
	accu >>= sizeof(unsigned short) * 8;
	accu += (unsigned long) _rand48_mult[0] * (unsigned long) xseed[1] + (unsigned long) _rand48_mult[1] * (unsigned long) xseed[0];
	temp[1] = (unsigned short) accu; // 中十六位
	accu >>= sizeof(unsigned short) * 8;
	accu += _rand48_mult[0] * xseed[2] + _rand48_mult[1] * xseed[1] + _rand48_mult[2] * xseed[0];
	xseed[0] = temp[0];
	xseed[1] = temp[1];
	xseed[2] = (unsigned short) accu;
}