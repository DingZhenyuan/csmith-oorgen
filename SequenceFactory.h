#ifndef SEQUENCE_FACTORY_H
#define SEQUENCE_FACTORY_H

#include <set>

class Sequence;

class SequenceFactory {
public:
	static Sequence *make_sequence();

	static void destroy_sequences();

	static char current_sep_char() { return current_sep_char_; }

private:
	static std::set<Sequence*> seqs_;

	static char current_sep_char_;
};

#endif // SEQUENCE_FACTORY_H
