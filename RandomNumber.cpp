#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "RandomNumber.h"

#include <cassert>
#include <iostream>
#include "AbsRndNumGenerator.h"
#include "Filter.h"

RandomNumber *RandomNumber::instance_ = NULL;

RandomNumber::RandomNumber(const unsigned long seed)
	: seed_(seed)
{
	unsigned int count = AbsRndNumGenerator::count();

	for (unsigned int i = 0; i < count; ++i) {
		RNDNUM_GENERATOR rImpl = static_cast<RNDNUM_GENERATOR>(i);
		generators_[rImpl] = NULL;
	}
}

RandomNumber::~RandomNumber()
{
	generators_.clear();
}

void
RandomNumber::make_all_rndnum_generators(const unsigned long seed)
{
	unsigned int count = AbsRndNumGenerator::count();
	AbsRndNumGenerator *generator;

	for (unsigned int i = 0; i < count; ++i) {
		RNDNUM_GENERATOR rImpl = static_cast<RNDNUM_GENERATOR>(i);
		generator = AbsRndNumGenerator::make_rndnum_generator(rImpl, seed);
		generators_[rImpl] = generator;
	}
}

void
RandomNumber::CreateInstance(RNDNUM_GENERATOR rImpl, const unsigned long seed)
{
	if (!instance_) {
		instance_ = new RandomNumber(seed);
		//instance_->make_all_rndnum_generators(seed);
		//instance_->curr_generator_ = instance_->generators_[rImpl];
		AbsRndNumGenerator *generator = AbsRndNumGenerator::make_rndnum_generator(rImpl, seed);
		assert(generator);
		instance_->curr_generator_ = generator;
		instance_->generators_[rImpl] = generator;
	}
	else {
		instance_->curr_generator_ = instance_->generators_[rImpl];
		assert(instance_->curr_generator_);
	}
}

RandomNumber*
RandomNumber::GetInstance(void)
{
	assert(instance_);
	return instance_;
}

AbsRndNumGenerator*
RandomNumber::GetRndNumGenerator(void)
{
	assert(instance_->curr_generator_);
	return instance_->curr_generator_;
}

/*
 * Currently we can't switch to SeqRandom mode from other mode,
 * because we need a way to specify sequence numbers anyway.
 */
RNDNUM_GENERATOR
RandomNumber::SwitchRndNumGenerator(RNDNUM_GENERATOR rImpl)
{
	unsigned int count = AbsRndNumGenerator::count();
	AbsRndNumGenerator *generator = instance_->generators_[rImpl];
	if (generator == NULL) {
		generator = AbsRndNumGenerator::make_rndnum_generator(rImpl, instance_->seed_);
		assert(generator);
		instance_->generators_[rImpl] = generator;
	}

	RNDNUM_GENERATOR old = instance_->curr_generator_->kind();
	assert(static_cast<unsigned int>(old) < count);

	instance_->curr_generator_ = generator;
	return old;
}

std::string
RandomNumber::get_prefixed_name(const std::string &name)
{
	return curr_generator_->get_prefixed_name(name);
}

std::string &
RandomNumber::trace_depth()
{
	return curr_generator_->trace_depth();
}

void
RandomNumber::get_sequence(std::string &sequence)
{
	curr_generator_->get_sequence(sequence);
}

unsigned int
RandomNumber::rnd_upto(const unsigned int n, const Filter *f, const std::string *where)
{
	return curr_generator_->rnd_upto(n, f, where);
}

bool
RandomNumber::rnd_flipcoin(const unsigned int p, const Filter *f, const std::string *where)
{
	return curr_generator_->rnd_flipcoin(p, f, where);
}

std::string
RandomNumber::RandomHexDigits(int num)
{
	return curr_generator_->RandomHexDigits(num);
}

std::string
RandomNumber::RandomDigits(int num)
{
	return curr_generator_->RandomDigits(num);
}

void
RandomNumber::doFinalization()
{
	unsigned int count = AbsRndNumGenerator::count();
	AbsRndNumGenerator *generator;

	for (unsigned int i = 0; i < count; ++i) {
		RNDNUM_GENERATOR rImpl = static_cast<RNDNUM_GENERATOR>(i);
		generator = instance_->generators_[rImpl];
		if (generator != NULL) {
			delete generator;
		}
	}
	delete instance_;
}

