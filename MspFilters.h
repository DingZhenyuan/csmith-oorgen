#ifndef MSP_FILTERS_H
#define MSP_FILTERS_H

#include "Filter.h"
#include "FunctionInvocation.h"

class MspBinaryFilter : public Filter
{
public:
	MspBinaryFilter();

	virtual ~MspBinaryFilter();

	virtual bool filter(int v) const;
};

class MspSafeOpSizeFilter : public Filter
{
public:
	MspSafeOpSizeFilter(eBinaryOps op);

	virtual ~MspSafeOpSizeFilter();

	virtual bool filter(int v) const;
private:
	eBinaryOps bin_op_;
};

#endif //MSP_FILTERS_H
