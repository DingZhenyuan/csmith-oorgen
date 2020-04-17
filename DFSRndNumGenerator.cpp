#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <cassert>
#include <iostream>
#include <sstream>

#include "DFSRndNumGenerator.h"
#include "CGOptions.h"
#include "Filter.h"
#include "SequenceFactory.h"
#include "Sequence.h"
#include "Error.h"
#include "SequenceLineParser.h"

// 表示每个随机选择的数据
class DFSRndNumGenerator::SearchState {
public:
	explicit SearchState(int index);
	SearchState(const SearchState &s);
	~SearchState();

	void initSearchState(bool init, int value, int bound);
	void dump(const string &where);

	// getters and setters
	bool init() { return init_; }
	void set_init(bool init) { init_ = init; }

	int value() { return value_; }
	void set_value(int value) { value_ = value; }
	void inc_value(void) { ++value_; }

	int bound() { return bound_; }
	void set_bound(int bound) { bound_ = bound; }

	int index() { return index_; }

private:
	// 这个state是否被初始化
	bool init_;
	// Current value from 0...(bound - 1)
	int value_;
	int bound_;
	int index_;
};

// 创建state时对index赋值
DFSRndNumGenerator::SearchState::SearchState(int index)
	: init_(false),
	  value_(0),
	  bound_(0),
	  index_(index)
{}

// 直接复制state作为新的state
DFSRndNumGenerator::SearchState::SearchState(const DFSRndNumGenerator::SearchState &state)
	: init_(state.init_),
	  value_(state.value_),
	  bound_(state.bound_),
	  index_(state.index_)
{}

DFSRndNumGenerator::SearchState::~SearchState() {}

// 初始化state，对其value和bound赋值
void DFSRndNumGenerator::SearchState::initSearchState(bool init, int value, int bound) {
	init_ = init;
	value_ = value;
	bound_ = bound;
}

// 输出state的数据信息，用于debug
#define DEBUG
#ifdef DEBUG
void DFSRndNumGenerator::SearchState::dump(const string &where)
{
	cout << "[state]" << where << ", index = " << index_ << ", init = " << init_ << ", value = " << value_ << ", bound = " \
		<< bound_ << std::endl;
}
#else
void DFSRndNumGenerator::SearchState::dump(const string &)
{
}
#endif


DFSRndNumGenerator *DFSRndNumGenerator::impl_ = 0;

DFSRndNumGenerator::DFSRndNumGenerator(Sequence *concrete_seq)
	: trace_string_(""),
	  decision_depth_(-1),
	  current_pos_(-1),
	  all_done_(false),
	  seq_(concrete_seq),
	  use_debug_sequence_(false)
{
	init_states(CGOptions::max_exhaustive_depth());
}

DFSRndNumGenerator::~DFSRndNumGenerator() {
	std::vector<DFSRndNumGenerator::SearchState *>::iterator i;
	for (i = states_.begin(); i != states_.end(); ++i) {
		delete (*i);
	}
	states_.clear();
	SequenceFactory::destroy_sequences();
}

DFSRndNumGenerator* DFSRndNumGenerator::make_rndnum_generator() {
	if (impl_) return impl_;

	Sequence *seq = SequenceFactory::make_sequence();

	impl_ = new DFSRndNumGenerator(seq);

	assert(impl_);

	std::string debug_sequence = CGOptions::dfs_debug_sequence();
	if (!debug_sequence.empty()) {
		std::vector<int> nums;
		if (!SequenceLineParser<vector<int> >::parse_sequence(nums, debug_sequence, SequenceFactory::current_sep_char())) {
			assert("dfs debugging sequence error!" && 0);
		}
		impl_->initialize_sequence(nums);
		impl_->use_debug_sequence_ = true;
	}
	return impl_;
}

void DFSRndNumGenerator::initialize_sequence(const vector<int> &v) {
	size_t i = 0;
	for (i = 0; i < v.size(); i++) {
		seq_->add_number(v[i], 0, i);
	}
}

#ifdef DEBUG
void
DFSRndNumGenerator::dumpCurrentState(int bound, const string &where)
{
	cout << "[current]" << where << ", current_pos = " << current_pos_ \
		<< ", decision_depth = " << decision_depth_ << " , bound = " \
		<< bound << ", all_done = " << all_done_ << std::endl;
}
#else
void
DFSRndNumGenerator::dumpCurrentState(int, const string &)
{
}
#endif

// 从DepthSpec.cpp调用。返回true表示我们渴望回溯。
bool
DFSRndNumGenerator::eager_backtracking(int depth_needed) {
    if (current_pos_ <= 0) {
		// all_done_ = true;
		// Error::set_error(BACKTRACKING_ERROR);
		// return true;
		return false;
	}
    // 深度检测
	int max_depth = CGOptions::max_exhaustive_depth();
	int remain_depth = max_depth - current_pos_;
	if (remain_depth >= depth_needed)
		return false;

	if (current_pos_ > decision_depth_) {
		Error::set_error(BACKTRACKING_ERROR);
		return true;
	}

	// 重置decision depth
	decision_depth_ = current_pos_;
    // 将后续state均使无效
	for (int i = current_pos_ + 1; i < max_depth; ++i) {
        states_[i]->set_init(false);
    }

	Error::set_error(BACKTRACKING_ERROR);
	return true;
}


int DFSRndNumGenerator::revisit_node(DFSRndNumGenerator::SearchState *state, int local_current_pos,
					int bound, const Filter *filter, const string *)
{
	int rv = state->value();
	if (filter) {
		if (rv >= bound) {
			state->dump("");
			dumpCurrentState(bound, "");
			cout << "rv = " << rv << ", bound = " << bound << std::endl;
			assert(0);
		}

		filter->filter(rv);

		ERROR_GUARD(-1);

		assert(current_pos_ < CGOptions::max_exhaustive_depth());

	}
	seq_->add_number(rv, bound, local_current_pos);
	return rv;
}

bool DFSRndNumGenerator::filter_invalid_nums(vector<int> *invalid_nums, int v) {
	if (!invalid_nums)
		return false;

	vector<int>::iterator i = find(invalid_nums->begin(), invalid_nums->end(), v);
    // 若v在invalid_nums中，则说明v属于无效数字，则返回true
	return (i != invalid_nums->end());
}

int DFSRndNumGenerator::random_choice (int bound, const Filter *filter, const string *where, vector<int> *invalid_nums) {
	int err = Error::get_error();

	if (err == BACKTRACKING_ERROR) {
		return -1;
	} else if (err != SUCCESS) {
		assert("request random number in an error state. " && 0);
	}

	++current_pos_;
	// 用debug序列
    if (use_debug_sequence_) {
		int rv = seq_->get_number_by_pos(current_pos_);
		if (filter) filter->filter(rv);
		//cout << "current_pos _ = " << current_pos_ << ", length = " << seq_->sequence_length() << std::endl;
		if (static_cast<unsigned int>(current_pos_) >= seq_->sequence_length() - 1) {
			all_done_ = true;
		}
		return rv;
	}

	int local_current_pos = current_pos_;

	// 检测位置是否超出
    if (current_pos_ >= CGOptions::max_exhaustive_depth() ||
		decision_depth_ >= CGOptions::max_exhaustive_depth()) {
		Error::set_error(EXCEED_MAX_DEPTH_ERROR);
		return -1;
	}

    DFSRndNumGenerator::SearchState *state = states_[current_pos_];

	state->set_bound(bound);
	//dumpCurrentState(bound, "");
	//state->dump("");

	// Revisit a node
	if (current_pos_ < decision_depth_ && state->init()) {
		return revisit_node(state, local_current_pos, bound, filter, where);
	}

	if (state->init()) {
		int v = state->value();
		int local_decision_depth = decision_depth_;
		do { // 滤除无效值
			++v;
			state->set_value(v);
			current_pos_ = local_current_pos;
			decision_depth_ = local_decision_depth;
			ERROR_GUARD(-1);
		} while (v < bound && ((filter && filter->filter(v)) || filter_invalid_nums(invalid_nums, v)));

		state->set_value(v);

		if (state->value() >= bound) { // backtracking
			current_pos_ = local_current_pos;
			for (int i = current_pos_; i < CGOptions::max_exhaustive_depth(); ++i) {
				states_[i]->set_init(false);
			}
			--decision_depth_;
			if (decision_depth_ < 0) all_done_ = true;
			Error::set_error(BACKTRACKING_ERROR);
			return -1;
		}
		else { // switch branch
			ERROR_GUARD(-1);

			int rv = state->value();
			seq_->add_number(rv, bound, local_current_pos);
			return rv;
		}
	}
	else { // First time to visit this node
		int v = 0;

		++decision_depth_;
		state->initSearchState(true, v, bound);

		while (v < bound && ((filter && (filter->filter(v))) || filter_invalid_nums(invalid_nums, v))) { // Filter out invalid value
			for (int i = decision_depth_; i < CGOptions::max_exhaustive_depth(); ++i) {
				states_[i]->set_value(0);
			}
			ERROR_GUARD(-1);
			decision_depth_ = current_pos_;
			current_pos_ = local_current_pos;
			++v;
		}
		decision_depth_ = current_pos_;

		if (v >= bound) {
			current_pos_ = local_current_pos;
			for (int i = current_pos_; i < CGOptions::max_exhaustive_depth(); ++i) {
				states_[i]->set_init(false);
			}
			--decision_depth_;
			if (decision_depth_ < 0)
				all_done_ = true;
			Error::set_error(BACKTRACKING_ERROR);
			return -1;
		}

		state->set_value(v);

		ERROR_GUARD(-1);

		seq_->add_number(v, bound, local_current_pos);
		return v;
	}
}

void DFSRndNumGenerator::log_depth(int d, const string *where, const char *log) {
	std::ostringstream ss1;

	if (log)
		ss1 << "[" << log << "]";

	if (where)
		ss1 << d << "(" << *where << ", pos = " << current_pos_ << ", current_decision_depth=" << decision_depth_ << ")->";
	else
		ss1 << d << "(..., pos = " << current_pos_ << ", current_decision_depth=" << decision_depth_ << ")->";
	trace_string_ += ss1.str();
}

void DFSRndNumGenerator::init_states(int size) {
	for (int i = 0; i < size; ++i) {
		DFSRndNumGenerator::SearchState *state = new SearchState(i);
		assert(state && "new SearchState: error!");
		states_.push_back(state);
	}
}

void DFSRndNumGenerator::reset_state(void) {
	current_pos_ = -1;
	trace_string_ = "";
	seq_->clear();
}

void DFSRndNumGenerator::get_sequence(std::string &sequence) {
	std::ostringstream ss;
	seq_->get_sequence(ss);
	sequence = ss.str();
}

std::string DFSRndNumGenerator::get_prefixed_name(const std::string &name) {
	std::ostringstream ss;
	ss << "p_";
	seq_->get_sequence(ss);
	ss << seq_->get_sep_char() << name;
	return ss.str();
}

std::string& DFSRndNumGenerator::trace_depth() {
	return trace_string_;
}

unsigned int DFSRndNumGenerator::rnd_upto(const unsigned int n, const Filter *f, const std::string *where) {
	int x = random_choice(n, f, where);
	assert(x == -1 || (x >= 0 && x < static_cast<int>(n)));
	return x;
}

bool DFSRndNumGenerator::rnd_flipcoin(const unsigned int n, const Filter *f, const std::string *where) {
	vector<int> invalid;
	int y;
	if (n == 100) {
		invalid.push_back(0);
		y = random_choice(2, f, where, &invalid);
	} else if (n == 0) {
		invalid.push_back(1);
		y = random_choice(2, f, where, &invalid);
	} else {
		y = random_choice(2, f, where);
	}
	assert(y == -1 || (y >= 0 && y < 2));
	return y;
}

unsigned long DFSRndNumGenerator::genrand(void) {
	return AbsRndNumGenerator::genrand();
}

std::string DFSRndNumGenerator::RandomHexDigits( int num ) {
	return AbsRndNumGenerator::RandomHexDigits(num);
}

std::string DFSRndNumGenerator::RandomDigits( int num ) {
	return AbsRndNumGenerator::RandomDigits(num);
}

