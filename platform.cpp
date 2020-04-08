#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "platform.h"
#include <stdlib.h>
#if HAVE_BSD_STDLIB_H
#  include <bsd/stdlib.h>
#endif

// 如果该平台没有`arc4random_buf'，则定义一个静态的，特定于平台的函数'read_time'
// 以便我们可以从当前时间设置随机种子。

#ifndef HAVE_ARC4RANDOM_BUF
#  if defined(__powerpc__) || defined(__powerpc64__)
static inline unsigned long read_time(void) {
    unsigned long a;
    asm volatile("mftb %0" : "=r" (a));
    return a;
}
#  elif defined(WIN32)
static unsigned __int64 read_time(void) {
    unsigned l, h;
    _asm {
        rdtsc
        mov l, eax
        mov h, edx
    }
    return (h << 32) + l;
}
#elif defined(__s390)
static unsigned long long read_time(void) {
	unsigned long long clk;
	asm volatile("stckf %0" : "=Q" (clk) : : "cc");
	return clk;
}
#  elif defined(__i386__) || defined(__x86_64__)
static long long read_time(void) {
	long long l;
	asm volatile("rdtsc\n\t"
				 : "=A" (l)
				 );
	return l;
}
#  else
#    include <time.h>
static long long read_time(void) {
	time_t t = time(NULL);
	return t;
}
#  endif

#endif // HAVE_ARC4RANDOM_BUF


#if HAVE_ARC4RANDOM_BUF
unsigned long platform_gen_seed() {
	unsigned long seed;
	arc4random_buf(&seed, sizeof seed);
	return seed;
}
#else
unsigned long platform_gen_seed() {
	return (long) read_time();
}
#endif

//////////// platform specific mkdir /////////////////

#ifndef WIN32
#  include <sys/stat.h>
#  include <unistd.h>
#else
#  include <direct.h>
#endif
#include <cerrno>

bool create_dir(const char *dir)
{
#ifndef WIN32
	if (mkdir(dir, 0770) == -1) {
#else
	if (mkdir(dir) == -1) {
#endif
		return (errno == EEXIST) ? true : false;
	}
	return true;
}
