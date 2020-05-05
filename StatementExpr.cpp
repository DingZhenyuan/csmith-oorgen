#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "StatementExpr.h"
#include <iostream>
#include "CGContext.h"
#include "CGOptions.h"
#include "FunctionInvocation.h"
#include "FunctionInvocationUser.h"
#include "Expression.h"
#include "Function.h"
#include "ExpressionVariable.h"
#include "FactMgr.h"
#include "Error.h"
#include "DepthSpec.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

/*
 *
 */
StatementExpr *
StatementExpr::make_random(CGContext &cg_context)
{
	DEPTH_GUARD_BY_TYPE_RETURN(dtStatementExpr, NULL);
	FunctionInvocation *invoke;
	// make copies
	Effect pre_effect = cg_context.get_accum_effect();
	FactMgr* fm = get_fact_mgr(&cg_context);
	vector<const Fact*> facts_copy = fm->global_facts;
	invoke = FunctionInvocation::make_random(false, cg_context, 0, 0);
	ERROR_GUARD(NULL);
	if (invoke->failed) {
		cg_context.reset_effect_accum(pre_effect);
		fm->restore_facts(facts_copy);
		delete invoke;
		return 0;
	}
	return new StatementExpr(cg_context.get_current_block(), *invoke);
}

/*
 *
 */
StatementExpr::StatementExpr(Block* b, const FunctionInvocation &e)
	: Statement(eInvoke, b),
	  expr(e)
{
	// Nothing else to do.
}

/*
 *
 */
StatementExpr::StatementExpr(const StatementExpr &se)
: Statement(se.get_type(), se.parent),
	  expr(*se.get_invoke())
{
	// Nothing else to do.
}

/*
 *
 */
StatementExpr::~StatementExpr(void)
{
	//delete &expr;
}

/*
 *
 */
void
StatementExpr::Output(std::ostream &out, FactMgr* /*fm*/, int indent) const
{
	output_tab(out, indent);
	expr.Output(out);
	out << ";";
	outputln(out);
}

bool
StatementExpr::visit_facts(vector<const Fact*>& inputs, CGContext& cg_context) const
{
	bool ok = expr.visit_facts(inputs, cg_context);

	// save effect
	FactMgr* fm = get_fact_mgr(&cg_context);
	fm->map_stm_effect[this] = cg_context.get_effect_stm();
	return ok;
}

std::vector<const ExpressionVariable*>
StatementExpr::get_dereferenced_ptrs(void) const
{
	std::vector<const ExpressionVariable*> vars;
	if (get_invoke()->invoke_type == eFuncCall) {
		const FunctionInvocationUser* func_call = dynamic_cast<const FunctionInvocationUser*>(get_invoke());
		for (size_t i=0; i<func_call->param_value.size(); i++) {
			// the parameters might be dereferenced pointers
			const Expression* value = func_call->param_value[i];
			vector<const ExpressionVariable*> param_ptrs = value->get_dereferenced_ptrs();
			vars.insert(vars.end(),  param_ptrs.begin(), param_ptrs.end());
		}
	}
	return vars;
}

bool
StatementExpr::has_uncertain_call_recursive(void) const
{
	return expr.has_uncertain_call_recursive();
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
