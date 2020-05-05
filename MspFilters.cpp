#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <cassert>
#include "MspFilters.h"
#include "SafeOpFlags.h"

MspBinaryFilter::MspBinaryFilter()
{

}

MspBinaryFilter::~MspBinaryFilter()
{

}

bool
MspBinaryFilter::filter(int v) const
{
	assert(v >= 0);
	eBinaryOps op = static_cast<eBinaryOps>(v);
	if ((op == eDiv || op == eMod))
		return true;
	else
		return false;
}

/////////////////////////////////////////////////////////

MspSafeOpSizeFilter::MspSafeOpSizeFilter(eBinaryOps op)
	: bin_op_(op)
{

}

MspSafeOpSizeFilter::~MspSafeOpSizeFilter()
{

}

bool
MspSafeOpSizeFilter::filter(int v) const
{
	assert(v >= 0);
	SafeOpSize op_size = static_cast<SafeOpSize>(v);
	switch(bin_op_) {
	case eMul: //fall-through
	case eRShift: //fall-through
	case eLShift:
		if ((op_size == sInt32 || op_size == sInt64))
			return true;
		else
			return false;
		break;
	default:
		return false;
	}
}

