#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "VectorFilter.h"

#include <functional>
#include <algorithm>
#include "ProbabilityTable.h"

using namespace std;

VectorFilter::VectorFilter(void)
	: ptable(NULL),
	  flag_(FILTER_OUT)
{
	//Nothing to do
}

VectorFilter::VectorFilter(DistributionTable *table)
	: ptable(table),
	  flag_(FILTER_OUT)
{
	//Nothing to do
}

VectorFilter::VectorFilter(std::vector<unsigned int> &vs, int flag)
	: vs_(vs),
	  ptable(NULL),
	  flag_(flag)
{
	//Nothing to do
}

VectorFilter::~VectorFilter(void)
{
	//Nothing to do
}

bool
VectorFilter::filter(int v) const
{
	if (!this->valid_filter())
		return false;

	v = lookup(v);
	bool re = std::find(vs_.begin(), vs_.end(), static_cast<unsigned int>(v)) != vs_.end();
	return (flag_ == FILTER_OUT) ? re : !re;
}

VectorFilter&
VectorFilter::add(unsigned int item)
{
	if (std::find(vs_.begin(), vs_.end(), item) == vs_.end()) {
		vs_.push_back(item);
	}
	return *this;
}

int
VectorFilter::get_max_prob(void) const
{
	return ptable ? ptable->get_max() : 100;
}

int
VectorFilter::lookup(int v) const
{
	if (!this->valid_filter() || ptable == NULL)
		return v;
	return v = ptable->rnd_num_to_key(v);
}
