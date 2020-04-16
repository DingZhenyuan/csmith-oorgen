#ifndef DEFAULT_RNDNUM_GENERATOR_H
#define DEFAULT_RNDNUM_GENERATOR_H

#include <string>
#include <vector>
#include "Common.h"
#include "CommonMacros.h"
#include "AbsRndNumGenerator.h"

class Sequence;
class Filter;

// Singleton class for the implementation of default based random generator
class DefaultRndNumGenerator : public AbsRndNumGenerator
{
public:
	// 返回impl_，若没有，则通过SequenceFactory获得Sequence并新建一个DefaultRndNumGenerator作为impl_
	static DefaultRndNumGenerator *make_rndnum_generator(const unsigned long seed);
	// 获取前缀名，直接返回name
	virtual std::string get_prefixed_name(const std::string &name);

	virtual std::string& trace_depth();
	// 返回随机选择的Sequence
	virtual void get_sequence(std::string &sequence);
	// 返回范围为0 ..（n-1）的随机数。要求通过Filter。
	virtual unsigned int rnd_upto(const unsigned int n, const Filter *f = NULL, const std::string *where = NULL);
	// 掷硬币，p%的可能性返回true
	virtual bool rnd_flipcoin(const unsigned int p, const Filter *f = NULL, const std::string *where = NULL);
	// 随机生成十六进制数
	virtual std::string RandomHexDigits( int num );
	// 
	virtual std::string RandomDigits( int num );

	virtual enum RNDNUM_GENERATOR kind() { return rDefaultRndNumGenerator; }

	void set_rand_depth(unsigned INT64 depth) { rand_depth_ = depth; }

	virtual ~DefaultRndNumGenerator();

private:
	DefaultRndNumGenerator(const unsigned long seed, Sequence *concrete_seq);
	// 若增量监测处于运行装态，则将<v, bound>加入到seq_中
	void add_number(int v, int bound, int k);

	static DefaultRndNumGenerator *impl_;

	unsigned INT64 rand_depth_;
	// 记录位置串，where->
	std::string trace_string_;

	Sequence *seq_;
	// 调用AbsRndNumGenerator中的genrand()，生成一个unsigned long类型的随机数
	virtual unsigned long genrand(void);

	//void seedrand(unsigned long seed);

	//Don't implement them
	DISALLOW_COPY_AND_ASSIGN(DefaultRndNumGenerator);
};

#endif //DEFAULT_RNDNUM_GENERATOR_H
