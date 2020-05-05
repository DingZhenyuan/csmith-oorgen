#ifndef SIMPLE_DELTA_SEQUENCE_H
#define SIMPLE_DELTA_SEQUENCE_H

#include <map>
#include <string>
#include <vector>
#include <ostream>
#include "Common.h"
#include "Sequence.h"

class SimpleDeltaSequence : public Sequence {
public:
	static SimpleDeltaSequence *CreateInstance(const char sep_char);

	virtual ~SimpleDeltaSequence();

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
	class ValuePair;

	explicit SimpleDeltaSequence(const char sep_char);

	bool empty_line(const std::string &line);

	void output_one(std::ostream &out, const ValuePair *p);

	std::map<int, ValuePair*> seq_map_;

	std::map<int, ValuePair*> sequence_;

	static SimpleDeltaSequence *impl_;

	const char sep_char_;

	unsigned INT64 current_pos_;
};

#endif // SIMPLE_DELTA_SEQUENCE_H
