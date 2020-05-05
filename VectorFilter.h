#ifndef VECTOR_FILTER_H
#define VECTOR_FILTER_H

#include <vector>
#include "Filter.h"

class DistributionTable;

// Filter out elements from the vector, i.e., elements in the vector
// is invalid.
#define FILTER_OUT 0
// Elements in the vector are valid
#define NOT_FILTER_OUT 1

class VectorFilter : public Filter
{
public:
	VectorFilter(void);
	VectorFilter(DistributionTable *table);
	explicit VectorFilter(std::vector<unsigned int> &vs, int flag = FILTER_OUT);

	VectorFilter& add(unsigned int item);

	int get_max_prob(void) const;

	int lookup(int v) const;

	virtual ~VectorFilter(void);

	virtual bool filter(int v) const;
private:
	std::vector<unsigned int> vs_;

	DistributionTable *ptable;

	int flag_;
};

#endif // VECTOR_FILTER_H
