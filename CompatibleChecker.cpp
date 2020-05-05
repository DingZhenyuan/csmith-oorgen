#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "CompatibleChecker.h"

#include <cassert>
#include "Expression.h"
#include "Variable.h"

bool CompatibleChecker::compatible_check_ = false;

/*
 *
 */
bool
CompatibleChecker::compatible_check(const Variable *v, const Expression *exp)
{

	assert(v);
	assert(exp);

	assert(0);
	if (!CompatibleChecker::compatible_check_)
		return false;
	return exp->compatible(v);
}

/*
 *
 */
bool
CompatibleChecker::compatible_check(const Expression *exp1, const Expression *exp2)
{
	assert(exp1);
	assert(exp2);

	if (!CompatibleChecker::compatible_check_)
		return false;
	return (exp1->compatible(exp2) || exp2->compatible(exp1));
}

void
CompatibleChecker::enable_compatible_check()
{
	CompatibleChecker::compatible_check_ = true;
}
