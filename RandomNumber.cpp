#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <cassert>
#include <iostream>

#include "RandomNumber.h"
#include "AbsRndNumGenerator.h"
#include "Filter.h"


RandomNumber *RandomNumber::instance_ = NULL;

RandomNumber::RandomNumber(const unsigned long seed) : seed_(seed) {
	// 这里返回3，随机数生成器有三种
    unsigned int count = AbsRndNumGenerator::count();
    // 创建3种生成器的数组generators_，均初始化为NULL
	for (unsigned int i = 0; i < count; ++i) {
		RNDNUM_GENERATOR rImpl = static_cast<RNDNUM_GENERATOR>(i);
		generators_[rImpl] = NULL;
	}
}

RandomNumber::~RandomNumber()
{
	generators_.clear();
}


void RandomNumber::make_all_rndnum_generators(const unsigned long seed) {
	// 3种
    unsigned int count = AbsRndNumGenerator::count();
	AbsRndNumGenerator *generator;

	for (unsigned int i = 0; i < count; ++i) {
		RNDNUM_GENERATOR rImpl = static_cast<RNDNUM_GENERATOR>(i);
        // 根据随机数生成器的种类和种子选择合适的生成函数，并返回一个对应的随机数生成器
		generator = AbsRndNumGenerator::make_rndnum_generator(rImpl, seed);
		generators_[rImpl] = generator;
	}
}

void RandomNumber::CreateInstance(RNDNUM_GENERATOR rImpl, const unsigned long seed) {
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
        // 设定rImpl对应的生成器为当前生成器
		instance_->curr_generator_ = instance_->generators_[rImpl];
		assert(instance_->curr_generator_);
	}
}

RandomNumber* RandomNumber::GetInstance(void) {
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
// 当前我们无法从其他模式切换到SeqRandom模式，因为无论如何我们需要一种指定序列号的方法。
RNDNUM_GENERATOR
RandomNumber::SwitchRndNumGenerator(RNDNUM_GENERATOR rImpl)
{
	// 3
    unsigned int count = AbsRndNumGenerator::count();
	AbsRndNumGenerator *generator = instance_->generators_[rImpl];
	if (generator == NULL) {
		generator = AbsRndNumGenerator::make_rndnum_generator(rImpl, instance_->seed_);
		assert(generator);
		instance_->generators_[rImpl] = generator;
	}
    // old=2, rSimpleDeltaRndNumGenerator
	RNDNUM_GENERATOR old = instance_->curr_generator_->kind();
	assert(static_cast<unsigned int>(old) < count);

	instance_->curr_generator_ = generator;
	return old;
}

std::string RandomNumber::get_prefixed_name(const std::string &name) {
	return curr_generator_->get_prefixed_name(name);
}

std::string& RandomNumber::trace_depth() {
	return curr_generator_->trace_depth();
}

void RandomNumber::get_sequence(std::string &sequence) {
	curr_generator_->get_sequence(sequence);
}

unsigned int RandomNumber::rnd_upto(const unsigned int n, const Filter *f, const std::string *where) {
	return curr_generator_->rnd_upto(n, f, where);
}

bool RandomNumber::rnd_flipcoin(const unsigned int p, const Filter *f, const std::string *where) {
	return curr_generator_->rnd_flipcoin(p, f, where);
}

std::string RandomNumber::RandomHexDigits(int num) {
	return curr_generator_->RandomHexDigits(num);
}

std::string RandomNumber::RandomDigits(int num) {
	return curr_generator_->RandomDigits(num);
}

void RandomNumber::doFinalization() {
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