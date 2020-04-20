#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <cstdlib>

#include "random.h"
#include "RandomNumber.h"
#include "Filter.h"
#include "CGOptions.h"
// #include "AbsProgramGenerator.h"

// std::string get_prefixed_name(const std::string &name)
// {
// 	if (!CGOptions::prefix_name())
// 		return name;
// 	if (!CGOptions::sequence_name_prefix()) {
// 		AbsProgramGenerator* instance = AbsProgramGenerator::GetInstance();
// 		assert(instance);
// 		return instance->get_count_prefix(name);
// 	}

// 	RandomNumber *rnd = RandomNumber::GetInstance();
// 	return rnd->get_prefixed_name(name);
// }

std::string RandomHexDigits( int num ) {
	RandomNumber *rnd = RandomNumber::GetInstance();
	return rnd->RandomHexDigits(num);
}

std::string RandomDigits( int num ) {
	RandomNumber *rnd = RandomNumber::GetInstance();
	return rnd->RandomDigits(num);
}

unsigned int
rnd_upto(const unsigned int n, const Filter *f, const std::string* where) {
	RandomNumber *rnd = RandomNumber::GetInstance();
	return rnd->rnd_upto(n, f, where);
}

bool
rnd_flipcoin(const unsigned int p, const Filter *f, const std::string* where) {
	RandomNumber *rnd = RandomNumber::GetInstance();
	return rnd->rnd_flipcoin(p, f, where);
}

std::string PureRandomHexDigits( int num ) {
	if (!CGOptions::is_random()) {
		RNDNUM_GENERATOR old;
	    old = RandomNumber::SwitchRndNumGenerator(rDefaultRndNumGenerator);
		std::string rv = RandomHexDigits(num);
	    RandomNumber::SwitchRndNumGenerator(old);
		return rv;
	} else {
		return RandomHexDigits(num);
	}
}

std::string PureRandomDigits( int num )
{

	if (!CGOptions::is_random()) {
		RNDNUM_GENERATOR old;
	    old = RandomNumber::SwitchRndNumGenerator(rDefaultRndNumGenerator);
		std::string rv = RandomDigits(num);
	    RandomNumber::SwitchRndNumGenerator(old);
		return rv;
	} else {
		return RandomDigits(num);
	}
}

unsigned int pure_rnd_upto(const unsigned int n, const Filter *f, const std::string* where) {
	if (n==0) return 0;		// 并不是随机的，但是也需要进行处理
	if (!CGOptions::is_random()) {
		RNDNUM_GENERATOR old;
	    old = RandomNumber::SwitchRndNumGenerator(rDefaultRndNumGenerator);
		unsigned int rv = rnd_upto(n, f, where);
	    RandomNumber::SwitchRndNumGenerator(old);
		return rv;
	} else {
		return rnd_upto(n, f, where);
	}
}

bool pure_rnd_flipcoin(const unsigned int n, const Filter *f, const std::string* where) {
	if (!CGOptions::is_random()) {
		RNDNUM_GENERATOR old;
	    old = RandomNumber::SwitchRndNumGenerator(rDefaultRndNumGenerator);
		bool rv = rnd_flipcoin(n, f, where);
	    RandomNumber::SwitchRndNumGenerator(old);
		return rv;
	} else {
		return rnd_flipcoin(n, f, where);
	}
}

std::string& trace_depth() {
	RandomNumber *rnd = RandomNumber::GetInstance();
	return rnd->trace_depth();
}

void get_sequence(std::string &sequence) {
	RandomNumber *rnd = RandomNumber::GetInstance();
	rnd->get_sequence(sequence);
}
