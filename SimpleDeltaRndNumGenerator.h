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

	static SimpleDeltaRndNumGenerator *make_rndnum_generator(const unsigned long seed);

	static void OutputStatistics(ostream &out);

	virtual std::string get_prefixed_name(const std::string &name);

	virtual std::string& trace_depth();

	virtual void get_sequence(std::string &sequence);

	virtual unsigned int rnd_upto(const unsigned int n, const Filter *f = NULL, const std::string *where = NULL);

	virtual bool rnd_flipcoin(const unsigned int p, const Filter *f = NULL, const std::string *where = NULL);

	virtual std::string RandomHexDigits( int num );

	virtual std::string RandomDigits( int num );

	virtual enum RNDNUM_GENERATOR kind() { return rSimpleDeltaRndNumGenerator; }

private:
	// ------------------------------------------------------------------------------------------
	SimpleDeltaRndNumGenerator(Sequence *concrete_seq);

	virtual unsigned long genrand(void);

	int random_choice(int bound, const Filter *f = NULL, const std::string *where = NULL);

	static unsigned int pure_rnd_upto(const unsigned int bound);

	static bool pure_rnd_flipcoin(const unsigned int p);

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
