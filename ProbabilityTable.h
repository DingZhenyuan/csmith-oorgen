#ifndef PROBABILITY_TABLE_H
#define PROBABILITY_TABLE_H

#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>
#include "Probabilities.h"
#include "VectorFilter.h"
#include "CGOptions.h"
#include "random.h"

using namespace std;

template <class Key, class Value>
class TableEntry {
public:
	TableEntry(Key k, Value v);

	Key get_key() { return key_; }

	Value get_value() { return value_; }

private:
	Key key_;
	Value value_;
};

template <class Key, class Value>
class ProbabilityTable {
	typedef TableEntry<Key, Value> Entry;

public:
	ProbabilityTable();

	~ProbabilityTable();

	void initialize(ProbName pname);

	void add_elem(Key k, Value v);

	void sorted_insert(Entry *t);

	Value get_value(Key k);

private:
	Key curr_max_key_;
	std::vector<Entry *> table_;
};

template <class Key, class Value>
TableEntry<Key, Value>::TableEntry(Key k, Value v)
	: key_(k),
	  value_(v)
{
}

template <class Key, class Value>
ProbabilityTable<Key, Value>::ProbabilityTable()
	: curr_max_key_(0)
{
	table_.clear();
}

template <class Key, class Value>
ProbabilityTable<Key, Value>::~ProbabilityTable()
{
	typename vector<Entry *>::iterator i;
	for (i = table_.begin(); i != table_.end(); ++i)
		delete (*i);
	table_.clear();
}

template <class Key, class Value>
void ProbabilityTable<Key, Value>::initialize(ProbName pname)
{
	Probabilities *impl_ = Probabilities::GetInstance();
	impl_->set_prob_table(this, pname);
}

template <class Key, class Value>
bool my_less(TableEntry<Key, Value> *t, Key k2)
{
	Key k1 = t->get_key();
	return (k1 < k2);
}

template <class Key, class Value>
bool my_greater(TableEntry<Key, Value> *t, Key k2)
{
	Key k1 = t->get_key();
	return (k1 > k2);
}

template <class Key, class Value>
void
ProbabilityTable<Key, Value>::sorted_insert(Entry *t)
{
	assert(t);

	Key k = t->get_key();

	if (table_.empty()) {
		table_.push_back(t);
		curr_max_key_ = k;
		return;
	}

	typename vector<Entry *>::iterator i;
	for (i=table_.begin(); i!=table_.end(); i++) {
		if (my_greater<Key, Value>(*i, k)) {
			break;
		}
	}
	//i = find_if(table_.begin(), table_.end(), std::bind2nd(std::ptr_fun(my_greater<Key, Value>), k));

	if (i != table_.end()) {
		table_.insert(i, t);
	}
	else {
		table_.push_back(t);
		curr_max_key_ = k;
	}
}

template <class Key, class Value>
void
ProbabilityTable<Key, Value>::add_elem(Key k, Value v)
{
	Entry *t = new Entry(k, v);
	sorted_insert(t);
}

template <class Key, class Value>
Value
ProbabilityTable<Key, Value>::get_value(Key k)
{
	assert(k < curr_max_key_);

	typename vector<Entry *>::iterator i;
	i = find_if(table_.begin(), table_.end(), std::bind2nd(std::ptr_fun(my_greater<Key, Value>), k));

	assert(i != table_.end());
	return (*i)->get_value();
}

class DistributionTable {
public:
	DistributionTable() { max_prob_ = 0;}
	~DistributionTable() {};

	void add_entry(int key, int prob);
	int get_max(void) const { return max_prob_;}
	int key_to_prob(int key) const;
	int rnd_num_to_key(int rnd) const;
private:
	int max_prob_;
	vector<int> keys_;
	vector<int> probs_;
};

#endif