#ifndef SIMPLE_DELTA_RNDNUM_GENERATOR_H
#define SIMPLE_DELTA_RNDNUM_GENERATOR_H

#include <vector>
#include <string>

#include "Common.h"
#include "CommonMacros.h"
#include "AbsRndNumGenerator.h"

class Sequence;
class Filter;

class SimpleDeltaRndNumGenerator : public AbsRndNumGenerator
{
public:
	virtual ~SimpleDeltaRndNumGenerator();
	// 初始化随机生成器impl_
	static SimpleDeltaRndNumGenerator *make_rndnum_generator(const unsigned long seed);
	// 输出统计信息，随机点+序列长度
	static void OutputStatistics(ostream &out);
	// 返回name
	virtual std::string get_prefixed_name(const std::string &name);
	// 返回跟踪信息以进行调试
	virtual std::string& trace_depth();
	// 将seq_中所有值对输出
	virtual void get_sequence(std::string &sequence);
	// 根据bound（n）选择value，即生成不大于n的随机数
	virtual unsigned int rnd_upto(const unsigned int n, const Filter *f = NULL, const std::string *where = NULL);
	// 随机生成布尔值
	virtual bool rnd_flipcoin(const unsigned int p, const Filter *f = NULL, const std::string *where = NULL);
	// 获取长度为num的16进制随机数
	virtual std::string RandomHexDigits( int num );
	// 获取长度为num的十进制随机数
	virtual std::string RandomDigits( int num );

	virtual enum RNDNUM_GENERATOR kind() { return rSimpleDeltaRndNumGenerator; }

private:
	// ------------------------------------------------------------------------------------------
	SimpleDeltaRndNumGenerator(Sequence *concrete_seq);
	// 获取一个long的随机数
	virtual unsigned long genrand(void);
	// 根据bound从seq_中得到value，并过滤器检验，切换默认生成器
	int random_choice(int bound, const Filter *f = NULL, const std::string *where = NULL);
	// 生成一个不大于bound的随机数
	static unsigned int pure_rnd_upto(const unsigned int bound);
	// 概率生成true
	static bool pure_rnd_flipcoin(const unsigned int p);
	// 切换为默认的随机数生成器rDefaultRndNumGenerator
	void switch_to_default_generator();

	// ----------------------------------------------------------------------------------------
	static SimpleDeltaRndNumGenerator *impl_;

	unsigned INT64 rand_depth_;

	unsigned INT64 random_point_;

	unsigned int filter_depth_;

	std::string trace_string_;

	Sequence *seq_;

	// disallow copy and assignment constructors
	DISALLOW_COPY_AND_ASSIGN(SimpleDeltaRndNumGenerator);
};

#endif //SIMPLE_DELTA_RNDNUM_GENERATOR_H
