#ifndef RANDOM_NUMBER_H
#define RANDOM_NUMBER_H

///////////////////////////////////////////////////////////////////////////////

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
	// Make it singleton for now.
	static void CreateInstance(RNDNUM_GENERATOR rImpl, const unsigned long seed);

	static RandomNumber *GetInstance();

	static AbsRndNumGenerator *GetRndNumGenerator(void);

	// Return the previous impl
	static RNDNUM_GENERATOR SwitchRndNumGenerator(RNDNUM_GENERATOR rImpl);

	static void doFinalization();

	std::string get_prefixed_name(const std::string &name);

	std::string &trace_depth();

	void get_sequence(std::string &sequence);

	// Probably it's not a good idea to define those functions with default arguments.
	// It would have potential problem to be misused.
	// I defined them in this way only for compatible to the previous code.
	// Use it carefully.
	virtual unsigned int rnd_upto(const unsigned int n, const Filter *f = NULL, const std::string *where = NULL);

	virtual bool rnd_flipcoin(const unsigned int p, const Filter *f = NULL, const std::string *where = NULL);

	virtual ~RandomNumber(void);

	virtual std::string RandomHexDigits(int num);

	virtual std::string RandomDigits(int num);

protected:
	void make_all_rndnum_generators(const unsigned long seed);

	AbsRndNumGenerator *curr_generator_;

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
