#ifndef ABS_RNDNUM_GENERATOR
#define ABS_RNDNUM_GENERATOR

#include <string>
#include "CommonMacros.h"

class Filter;

enum RNDNUM_GENERATOR {
    rDefaultRndNumGenerator = 0,
	rDFSRndNumGenerator,
	rSimpleDeltaRndNumGenerator,
}

#define MAX_RNDNUM_GENERATOR (rSimpleDeltaRndNumGenerator+1)

class AbsRndNumGenerator {
public:
    // 创建随机数生成器的工厂方法
	// 根据随机数生成器的种类和种子选择合适的生成函数，并返回一个对应的随机数生成器
	static AbsRndNumGenerator *make_rndnum_generator(RNDNUM_GENERATOR impl, const unsigned long seed);
	// 根据确定性选择srand48的调用函数
	static void seedrand(const unsigned long seed);

	static const char* get_hex1();

	static const char* get_dec1();

	static unsigned int count (void) {
		return MAX_RNDNUM_GENERATOR;
	}

	virtual std::string get_prefixed_name(const std::string &name) = 0;

	virtual std::string& trace_depth(void) = 0;

	virtual void get_sequence(std::string &sequence) = 0;

	virtual unsigned int rnd_upto(const unsigned int n, const Filter *f = NULL, const std::string *where = NULL) = 0;

	virtual bool rnd_flipcoin(const unsigned int p, const Filter *f = NULL, const std::string *where = NULL) = 0;

	// 生成一个字符串类型的十六进制数，位数为num
	virtual std::string RandomHexDigits( int num ) = 0;

	// 生成一个字符串类型的十进制数，位数为num
	virtual std::string RandomDigits( int num ) = 0;

	// Although it's not a good idea to return the kind of different implementation,
	// it's useful for error_handler. Basically we don't want to make the code
	// depend on the kind, use polymorphism instead.
	virtual enum RNDNUM_GENERATOR kind() = 0;

	virtual ~AbsRndNumGenerator(void);

protected:
    // 生成一个long类型的随机数
	virtual unsigned long genrand(void) = 0;
	
	AbsRndNumGenerator();

private:
	// “ hex”和“ dec”是MSVC中的保留关键字，我们必须将其重命名
	// 十六进制字符
	static const char *hex1;
	// 十进制字符
	static const char *dec1;

	// Don't implement them
	DISALLOW_COPY_AND_ASSIGN(AbsRndNumGenerator);
}

#endif //ABS_RNDNUM_GENERATOR