#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <map>
#include <string>
#include <ostream>
#include "Common.h"

class Sequence {
public:
	Sequence();

	virtual ~Sequence();

	static Sequence *make_sequence();

	virtual void init_sequence() = 0;

	virtual unsigned INT64 sequence_length() = 0;

	virtual void add_number(int v, int bound, int k) = 0;

	virtual int get_number(int bound) = 0;

	virtual int get_number_by_pos(int pos) = 0;

	virtual void clear() = 0;

	virtual void get_sequence(std::ostream &out) = 0;

	virtual char get_sep_char() const = 0;
};

#endif // SEQUENCE_H
