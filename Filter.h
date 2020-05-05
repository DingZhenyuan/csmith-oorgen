#ifndef FILTER_H
#define FILTER_H

#include <bitset>

enum FilterKind {
	fDefault,
	fDFS,
};

#define MAX_FILTER_KIND_SIZE ((FilterKind) (fDFS + 1))

// Filter base class
class Filter
{
public:
	Filter();
	virtual ~Filter(void);

	virtual bool filter(int v) const = 0;

	void enable(FilterKind kind);

	void disable(FilterKind kind);

protected:
	bool valid_filter() const;

	// What kind of mode this filter can apply to
	// By default, it can work for all modes.
	std::bitset<MAX_FILTER_KIND_SIZE> kinds_;

private:
	FilterKind current_kind() const;
};

#endif // FILTER_H
