#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "Finalization.h"

#include "Function.h"
#include "RandomNumber.h"
#include "VariableSelector.h"
#include "Type.h"
#include "FunctionInvocationUser.h"
#include "Bookkeeper.h"
#include "Probabilities.h"
#include "StatementGoto.h"
#include "ExtensionMgr.h"

void
Finalization::doFinalization()
{
	Function::doFinalization();
	VariableSelector::doFinalization();
	Variable::doFinalization();
	Type::doFinalization();
	RandomNumber::doFinalization();
	FunctionInvocationUser::doFinalization();
	Probabilities::DestroyInstance();
	StatementGoto::doFinalization();
	ExtensionMgr::DestroyExtension();
}

