#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "SequenceFactory.h"

#include <cassert>
#include <iostream>
#include <string>
#include "LinearSequence.h"
#include "SimpleDeltaSequence.h"
#include "DeltaMonitor.h"

std::set<Sequence*> SequenceFactory::seqs_;

char SequenceFactory::current_sep_char_ = '_';

Sequence*
SequenceFactory::make_sequence()
{
	Sequence *seq = NULL;

	if (DeltaMonitor::is_running()) {
		seq = DeltaMonitor::GetSequence();
		current_sep_char_ = DeltaMonitor::GetSepChar();
	}
	else {
		seq = new LinearSequence(LinearSequence::default_sep_char);
		current_sep_char_ = LinearSequence::default_sep_char;
	}

	assert(seq);
	seqs_.insert(seq);
	return seq;
}

void
SequenceFactory::destroy_sequences()
{
	std::set<Sequence*>::iterator i;
	for(i = seqs_.begin(); i != seqs_.end(); ++i) {
		delete *i;
	}
	seqs_.clear();
}

