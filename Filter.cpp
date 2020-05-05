#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "Filter.h"

#include <cassert>
#include "CGOptions.h"
#include "CGContext.h"

using namespace std;

Filter::Filter()
{
	kinds_.set();
}

Filter::~Filter(void)
{
	kinds_.reset();
}

/*
 *
 */
void Filter::enable(FilterKind kind)
{
	assert(kind < MAX_FILTER_KIND_SIZE);
	kinds_[kind] = true;
}

/*
 *
 */
void Filter::disable(FilterKind kind)
{
	assert(kind < MAX_FILTER_KIND_SIZE);
	kinds_[kind] = false;
}

/*
 *
 */
FilterKind
Filter::current_kind(void) const
{
	if (CGOptions::random_based())
		return fDefault;
	else if (CGOptions::dfs_exhaustive())
		return fDFS;
	return MAX_FILTER_KIND_SIZE;
}

/*
 *
 */
bool
Filter::valid_filter() const
{
	FilterKind k = current_kind();

	if (!kinds_.test(k))
		return false;
	return true;
}

