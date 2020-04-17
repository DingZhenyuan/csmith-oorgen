#if HAVE_CONFIG_H
#  include <config.h>
#endif

#ifdef WIN32
#pragma warning(disable : 4786)   /* Disable annoying warning messages */
#endif

#include <cassert>
#include <iostream>
#include <sstream>

#include "SimpleDeltaRndNumGenerator.h"
#include "CGOptions.h"
#include "Filter.h"
#include "SequenceFactory.h"
#include "Sequence.h"
#include "Error.h"
#include "RandomNumber.h"
#include "DefaultRndNumGenerator.h"
#include "DeltaMonitor.h"

SimpleDeltaRndNumGenerator *SimpleDeltaRndNumGenerator::impl_ = 0;

SimpleDeltaRndNumGenerator::SimpleDeltaRndNumGenerator(Sequence *concrete_seq)
	: rand_depth_(0),
	  random_point_(0),
	  filter_depth_(0),
	  trace_string_(""),
	  seq_(concrete_seq)
{}

SimpleDeltaRndNumGenerator::~SimpleDeltaRndNumGenerator() {
    SequenceFactory::destroy_sequences();
}

SimpleDeltaRndNumGenerator* SimpleDeltaRndNumGenerator::make_rndnum_generator(const unsigned long /*seed*/) {
    if (impl_) return impl_;

    // 新建sequence并初始化，用其创建一个新的SimpleDeltaRndNumGenerator
    Sequence *seq = SequenceFactory::make_sequence();
    seq->init_sequence();
    impl_ = new SimpleDeltaRndNumGenerator(seq);
    // 在序列内确定随机点
    impl_->random_point_ = SimpleDeltaRndNumGenerator::pure_rnd_upto(seq->sequence_length());
    assert(impl_);
    return impl_;
}

int SimpleDeltaRndNumGenerator::random_choice (int bound, const Filter *f, const string *) {
    assert(seq_);
    int rv = seq_->get_number(bound);
    rand_depth_++;
    if (rv == -1) {
        // 无效的增量序列
        Error::set_error(INVALID_SIMPLE_DELTA_SEQUENCE);
        return -1;
    }
    if (f) {
        filter_depth_++;
        if (f->filter(rv)) {
            // 过滤器错误
            Error::set_error(FILTER_ERROR);
            return -1;
        }
        filter_depth_--;
    }
    switch_to_default_generator();
    return rv;
}

void SimpleDeltaRndNumGenerator::get_sequence(std::string &sequence) {
	std::ostringstream ss;
	seq_->get_sequence(ss);
	sequence = ss.str();
}

std::string SimpleDeltaRndNumGenerator::get_prefixed_name(const std::string &name) {
	return name;
}

std::string& SimpleDeltaRndNumGenerator::trace_depth() {
	return trace_string_;
}

unsigned int SimpleDeltaRndNumGenerator::rnd_upto(const unsigned int n, const Filter *f, const std::string *where) {
	int x = random_choice(n, f, where);
	assert(x == -1 || (x >= 0 && x < static_cast<int>(n)));
	return x;
}

bool SimpleDeltaRndNumGenerator::rnd_flipcoin(const unsigned int, const Filter *f, const std::string *where) {
	int y = random_choice(2, f, where);
	assert(y == -1 || (y >= 0 && y < 2));
	return y;
}

unsigned long SimpleDeltaRndNumGenerator::genrand(void) {
    return AbsRndNumGenerator::genrand();
}

std::string SimpleDeltaRndNumGenerator::RandomHexDigits(int num) {
    std::string str;
    const char* hex1 = AbsRndNumGenerator::get_hex1();
    while (num--) {
        int x = random_choice(16, NULL, NULL);
        str += hex1[x];
    }
    return str;
}

std::string SimpleDeltaRndNumGenerator::RandomDigits(int num) {
    std::string str;
    const char* dec1 = AbsRndNumGenerator::get_dec1();
    while (num--) {
        int x = random_choice(10, NULL, NULL);
        str += dec1[x];
    }
    return str;
}

unsigned int SimpleDeltaRndNumGenerator::pure_rnd_upto(const unsigned int bound) {
    assert(impl_);
    return impl_->genrand() % bound;
}

bool SimpleDeltaRndNumGenerator::pure_rnd_flipcoin(const unsigned int p) {
	assert(impl_);
	bool rv = (impl_->genrand() % 100) < p;
	return rv;
}

void SimpleDeltaRndNumGenerator::switch_to_default_generator() {
	if (DeltaMonitor::no_delta_reduction() || rand_depth_ < impl_->random_point_ || impl_->filter_depth_ != 0)
		return;

	//RNDNUM_GENERATOR old;
    //old = RandomNumber::SwitchRndNumGenerator(rDefaultRndNumGenerator);
    RandomNumber::SwitchRndNumGenerator(rDefaultRndNumGenerator);
	DefaultRndNumGenerator *generator = dynamic_cast<DefaultRndNumGenerator*>(RandomNumber::GetRndNumGenerator());
	generator->set_rand_depth(impl_->rand_depth_);
}

void SimpleDeltaRndNumGenerator::OutputStatistics(ostream &out) {
	assert(impl_);
	assert(impl_->seq_);
	out << "/*" << std::endl;
	out << "** This program was reduced by the simple delta reduction algorithm" << std::endl;
#ifdef WIN32
	out << "** at a random point " << (unsigned int)impl_->random_point_ << " with the sequence of length " << (unsigned int)impl_->seq_->sequence_length() << "." << std::endl;
#else
	out << "** at a random point " << impl_->random_point_ << " with the sequence of length " << impl_->seq_->sequence_length() << "." << std::endl;
#endif
	out << "*/" << std::endl;
}
