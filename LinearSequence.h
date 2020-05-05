#ifndef LINEAR_SEQUENCE_H
#define LINEAR_SEQUENCE_H

#include <map>
#include <string>

#include "Sequence.h"

class LinearSequence : public Sequence {
public:
	explicit LinearSequence(const char sep_char);

	virtual ~LinearSequence();

	virtual void init_sequence();

	virtual unsigned INT64 sequence_length();

	virtual void add_number(int v, int bound, int k);

	virtual int get_number(int bound);

	virtual int get_number_by_pos(int pos);

	virtual void clear();

	virtual void get_sequence(std::ostream &);

	virtual char get_sep_char() const { return sep_char_; }

	static const char default_sep_char;

private:
	std::map<int, int> seq_map_;

	const char sep_char_;
};

#endif // LINEAR_SEQUENCE_H
