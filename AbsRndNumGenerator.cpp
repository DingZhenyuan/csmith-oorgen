#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "AbsRndNumGenerator.h"

#include <cassert>
#include <cstdlib>
#include <iostream>

#include "DefaultRndNumGenerator.h"
#include "DFSRndNumGenerator.h"
#include "SimpleDeltaRndNumGenerator.h"

using namespace std;

#ifndef HAVE_LRAND48
extern "C" {
	extern void srand48(long seed);
	extern long lrand48(void);
}
#endif

const char *AbsRndNumGenerator::hex1 = "0123456789ABCDEF";

const char *AbsRndNumGenerator::dec1 = "0123456789";

AbsRndNumGenerator::AbsRndNumGenerator()
{
	//Nothing to do
}

AbsRndNumGenerator::~AbsRndNumGenerator()
{
	//Nothing to do
}

/*
 * Factory method to create random number generators.
 */
AbsRndNumGenerator*
AbsRndNumGenerator::make_rndnum_generator(RNDNUM_GENERATOR impl, const unsigned long seed)
{
	AbsRndNumGenerator *rImpl = 0;

	AbsRndNumGenerator::seedrand(seed);
	switch (impl) {
		case rDefaultRndNumGenerator:
			rImpl = DefaultRndNumGenerator::make_rndnum_generator(seed);
			break;
		case rDFSRndNumGenerator:
			rImpl = DFSRndNumGenerator::make_rndnum_generator();
			break;
		case rSimpleDeltaRndNumGenerator:
			rImpl = SimpleDeltaRndNumGenerator::make_rndnum_generator(seed);
			break;
		default:
			assert(!"unknown random generator");
			break;
	}

	return rImpl;
}

void
AbsRndNumGenerator::seedrand(const unsigned long seed )
{
#ifdef HAVE_SRAND48_DETERMINISTIC
	// OpenBSD requires a special call to activate the standard,
	// deterministic behavior of `lrand48'.
	srand48_deterministic(seed);
#else
	srand48(seed);
#endif
}

/*
 * Return random shuffled integers in set [0...n]
 * Note: deprecated.
 */
#if 0
unsigned int*
AbsRndNumGenerator::rnd_shuffle(unsigned int n)
{
	unsigned int* ary = new unsigned int[n];
	unsigned int i = 0;
	unsigned int tmp;

	for (i = 0; i < n; i++) {
		ary[i] = i;
	}

	for (i = 0; i < n; i++) {
		unsigned int j = rnd_upto(i);
		tmp = ary[j];
		ary[j] = ary[i];
		ary[i] = tmp;
	}
	return ary;
}
#endif

unsigned long
AbsRndNumGenerator::genrand(void)
{
	return lrand48();
}

std::string
AbsRndNumGenerator::RandomHexDigits( int num )
{
	std::string str;
	while ( num-- )
	{
		str += hex1[genrand()%16];
	}

	return str;
}

std::string
AbsRndNumGenerator::RandomDigits( int num )
{
	std::string str;
	while ( num-- )
	{
		str += dec1[genrand()%10];
	}

	return str;
}

const char *
AbsRndNumGenerator::get_hex1()
{
	return AbsRndNumGenerator::hex1;
}

const char *
AbsRndNumGenerator::get_dec1()
{
	return AbsRndNumGenerator::dec1;
}

