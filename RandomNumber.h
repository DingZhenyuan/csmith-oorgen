#ifndef RANDOM_NUMBER_H
#define RANDOM_NUMBER_H

#include <string>
#include <map>

#include "CommonMacros.h"
#include "AbsRndNumGenerator.h"

class Filter;

/*
 * Common interface of all random number generators.
 * Works like a bridge to those generators.
 */
class RandomNumber
{
public:
	// //现在将其设为单例
	// 设定rImpl对应的生成器为当前生成器
	static void CreateInstance(RNDNUM_GENERATOR rImpl, const unsigned long seed);
	// 返回instance_
	static RandomNumber *GetInstance();
	// 获取当前的随机生成器
	static AbsRndNumGenerator *GetRndNumGenerator(void);

	// Return the previous impl
	// 切换为rSimpleDeltaRndNumGenerator
	static RNDNUM_GENERATOR SwitchRndNumGenerator(RNDNUM_GENERATOR rImpl);
	// 终止化，删除所有的generator，删除instance
	static void doFinalization();
	// 获取前缀名，即返回name
	std::string get_prefixed_name(const std::string &name);
	// 追踪深度
	std::string &trace_depth();
	// 获取序列
	void get_sequence(std::string &sequence);

	// Probably it's not a good idea to define those functions with default arguments.
	// It would have potential problem to be misused.
	// I defined them in this way only for compatible to the previous code.
	// Use it carefully.
	// 随机生成一个不大于n可通过f的数
	virtual unsigned int rnd_upto(const unsigned int n, const Filter *f = NULL, const std::string *where = NULL);
	// 随机生成p概率为true
	virtual bool rnd_flipcoin(const unsigned int p, const Filter *f = NULL, const std::string *where = NULL);

	virtual ~RandomNumber(void);
	// 随机生成16进制数
	virtual std::string RandomHexDigits(int num);
	// 随机生成10进制数
	virtual std::string RandomDigits(int num);

protected:
	// 获取三种类型的随机数生成器，加入到generator_中
	void make_all_rndnum_generators(const unsigned long seed);
	// 设定rImpl对应的生成器为当前生成器
	AbsRndNumGenerator *curr_generator_;
	// RandomNumber实例，默认为NULL
	static RandomNumber *instance_;

	std::map<RNDNUM_GENERATOR, AbsRndNumGenerator*> generators_;

private:
	const unsigned long seed_;

	explicit RandomNumber(const unsigned long seed);
	explicit RandomNumber(AbsRndNumGenerator *rndnum_generator);

	// Don't implement them
	DISALLOW_COPY_AND_ASSIGN(RandomNumber);
};

#endif //RANDOM_NUMBER_H