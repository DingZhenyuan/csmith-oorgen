#ifndef RANDOM_H
#define RANDOM_H

///////////////////////////////////////////////////////////////////////////////

#include <string>

class Filter;

// Old stuff.
std::string	RandomHexDigits(int num);
std::string	RandomDigits(int num);

// New stuff.
unsigned int	rnd_upto(const unsigned int n, const Filter *f = NULL, const std::string* where = NULL);
bool		rnd_flipcoin(const unsigned int p, const Filter *f = NULL, const std::string* where = NULL);
// return pure random numbers even if csmith is in other mode, e.g., exhaustive mode
std::string	PureRandomHexDigits(int num);
std::string	PureRandomDigits(int num);
unsigned int	pure_rnd_upto(const unsigned int n, const Filter *f = NULL, const std::string* where = NULL);
bool		pure_rnd_flipcoin(const unsigned int p, const Filter *f = NULL, const std::string* where = NULL);

std::string	get_prefixed_name(const std::string &name);
std::string&	trace_depth();
void		get_sequence(std::string &sequence);
#if 0
// Deprecated
unsigned int*   rnd_shuffle(unsigned int n);
#endif

///////////////////////////////////////////////////////////////////////////////

#endif // RANDOM_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
