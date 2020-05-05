#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "LinearSequence.h"

#include <cassert>
#include <sstream>
#include <iostream>

#include "CGOptions.h"

using namespace std;

const char LinearSequence::default_sep_char = '_';

LinearSequence::LinearSequence(const char sep_char)
	: sep_char_(sep_char)
{
	// Nothing to do
}

LinearSequence::~LinearSequence()
{
	// Nothing to do
}

/*
 *
 */
void
LinearSequence::init_sequence()
{

}

void
LinearSequence::add_number(int v, int /*bound*/, int k)
{
	seq_map_[k] = v;
}

int
LinearSequence::get_number(int /*bound*/)
{
	return -1;
}

int
LinearSequence::get_number_by_pos(int pos)
{
	int rv = seq_map_[pos];
	assert(rv >= 0);
	return rv;
}

void
LinearSequence::clear()
{
	seq_map_.clear();
}

void
LinearSequence::get_sequence(ostream &ss)
{
	assert(!seq_map_.empty());

	size_t i = 0;
	for (i = 0; i < seq_map_.size() - 1; ++i) {
		ss << seq_map_[i] << sep_char_;
	}
	ss << seq_map_[i];
}

unsigned INT64
LinearSequence::sequence_length()
{
	return seq_map_.size();
}
