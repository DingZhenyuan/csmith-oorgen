#ifndef ABS_RNDNUM_GENERATOR
#define ABS_RNDNUM_GENERATOR

#include <string>
#include "CommonMacros.h"

class Filter;

enum RNDNUM_GENERATOR {
	rDefaultRndNumGenerator = 0,
	rDFSRndNumGenerator,
	rSimpleDeltaRndNumGenerator,
};

#define MAX_RNDNUM_GENERATOR (rSimpleDeltaRndNumGenerator+1)

// I could make AbsRndNumGenerator not pure, but want to force each subclass implement
// it's own member functions, in case of forgetting something.
class AbsRndNumGenerator
{
public:
	static AbsRndNumGenerator *make_rndnum_generator(RNDNUM_GENERATOR impl, const unsigned long seed);

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

	virtual std::string RandomHexDigits( int num ) = 0;

	virtual std::string RandomDigits( int num ) = 0;

	// Although it's not a good idea to return the kind of different implementation,
	// it's useful for error_handler. Basically we don't want to make the code
	// depend on the kind, use polymorphism instead.
	virtual enum RNDNUM_GENERATOR kind() = 0;

	virtual ~AbsRndNumGenerator(void);

protected:
	virtual unsigned long genrand(void) = 0;

	AbsRndNumGenerator();

private:
	// ------------------------------------------------------------------------------------------
	// "hex" and "dec" are reserved keywords in MSVC, we have to rename them
	static const char *hex1;

	static const char *dec1;

	// Don't implement them
	DISALLOW_COPY_AND_ASSIGN(AbsRndNumGenerator);
};

#endif //ABS_RNDNUM_GENERATOR

