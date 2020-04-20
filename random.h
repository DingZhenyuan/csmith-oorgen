#ifndef RANDOM_H
#define RANDOM_H

#include <string>

class Filter;

// Old stuff.
// 16进制随机数
std::string	RandomHexDigits(int num);
// 10进制随机数
std::string	RandomDigits(int num);

// New stuff.
// 生成不大于n的随机数
unsigned int	rnd_upto(const unsigned int n, const Filter *f = NULL, const std::string* where = NULL);
// p概率生成true
bool		rnd_flipcoin(const unsigned int p, const Filter *f = NULL, const std::string* where = NULL);
// 根据CGOptions::is_random()判断来切换随机数生成器
// 即使csmith处于其他模式（例如穷举模式），也将返回纯随机数
std::string	PureRandomHexDigits(int num);
std::string	PureRandomDigits(int num);
unsigned int	pure_rnd_upto(const unsigned int n, const Filter *f = NULL, const std::string* where = NULL);
bool		pure_rnd_flipcoin(const unsigned int p, const Filter *f = NULL, const std::string* where = NULL);

std::string	get_prefixed_name(const std::string &name);
// 追溯深度
std::string&	trace_depth();
// 获取sequence
void		get_sequence(std::string &sequence);
#if 0
// Deprecated
unsigned int*   rnd_shuffle(unsigned int n);
#endif

#endif // RANDOM_H