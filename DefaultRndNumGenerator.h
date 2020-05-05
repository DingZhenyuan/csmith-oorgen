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
	static DefaultRndNumGenerator *make_rndnum_generator(const unsigned long seed);

	virtual std::string get_prefixed_name(const std::string &name);

	virtual std::string& trace_depth();

	virtual void get_sequence(std::string &sequence);

	virtual unsigned int rnd_upto(const unsigned int n, const Filter *f = NULL, const std::string *where = NULL);

	virtual bool rnd_flipcoin(const unsigned int p, const Filter *f = NULL, const std::string *where = NULL);

	virtual std::string RandomHexDigits( int num );

	virtual std::string RandomDigits( int num );

	virtual enum RNDNUM_GENERATOR kind() { return rDefaultRndNumGenerator; }

	void set_rand_depth(unsigned INT64 depth) { rand_depth_ = depth; }

	virtual ~DefaultRndNumGenerator();

private:
	DefaultRndNumGenerator(const unsigned long seed, Sequence *concrete_seq);

	void add_number(int v, int bound, int k);

	static DefaultRndNumGenerator *impl_;

	unsigned INT64 rand_depth_;

	std::string trace_string_;

	Sequence *seq_;

	virtual unsigned long genrand(void);

	//void seedrand(unsigned long seed);

	//Don't implement them
	DISALLOW_COPY_AND_ASSIGN(DefaultRndNumGenerator);
};

#endif //DEFAULT_RNDNUM_GENERATOR_H
