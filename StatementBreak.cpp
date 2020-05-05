#if HAVE_CONFIG_H
#  include <config.h>
#endif

#ifdef WIN32
#pragma warning(disable : 4786)   /* Disable annoying warning messages */
#endif

#include "StatementBreak.h"
#include <iostream>
#include <cassert>
#include "CGOptions.h"
#include "CGContext.h"
#include "Block.h"
#include "Type.h"
#include "Function.h"
#include "Expression.h"
#include "FactMgr.h"
#include "Bookkeeper.h"
#include "ArrayVariable.h"
#include "Error.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

/*
 *
 */
StatementBreak *
StatementBreak::make_random(CGContext &cg_context)
{
	// quick fix: don't generate break statement for nested loops (this including multi-dimension array operations)
	// JYTODO: treat "break" for nested loops as "continue", because it's effect is going back to the
	// head of loop body, same as continue
	//if (cg_context.focus_var && cg_context.focus_var->get_dimension() > 1) {
	//	return 0;
	//}
	//FactMgr* fm = get_fact_mgr(&cg_context);
	// find the closest looping parent block: the one "continue"
	// would apply to
	Block* b = cg_context.get_current_block();
	while (b && !b->looping) {
		b = b->parent;
	}
	assert(b);
	cg_context.get_effect_stm().clear();
	Expression *expr = Expression::make_random(cg_context, get_int_type(), 0, true, true, eVariable);
	ERROR_GUARD(NULL);
	StatementBreak* sc = new StatementBreak(cg_context.get_current_block(), *expr, *b);
	b->break_stms.push_back(sc);
    return sc;
}

/*
 *
 */
StatementBreak::StatementBreak(Block* parent, const Expression &test, const Block &b)
	: Statement(eBreak, parent),
	  test(test),
	  loop_blk(b)
{
	// Nothing else to do.
}

/*
 *
 */
StatementBreak::StatementBreak(const StatementBreak &sc)
: Statement(sc.get_type(), sc.parent),
	  test(sc.test),
	  loop_blk(sc.loop_blk)
{
	// Nothing else to do.
}

/*
 *
 */
StatementBreak::~StatementBreak(void)
{
	delete &test;
}

/*
 * return true if condition is always true
 */
bool
StatementBreak::must_jump(void) const
{
	return test.not_equals(0);
}

/*
 *
 */
void
StatementBreak::Output(std::ostream &out, FactMgr* /*fm*/, int indent) const
{
	output_tab(out, indent);
	out << "if (";
	test.Output(out);
	out << ")";
	outputln(out);
	output_tab(out, indent+1);
	out << "break;";
	outputln(out);
}

bool
StatementBreak::visit_facts(vector<const Fact*>& inputs, CGContext& cg_context) const
{
	// evaludate condition first
	if (!test.visit_facts(inputs, cg_context)) {
		return false;
	}
	FactMgr* fm = get_fact_mgr(&cg_context);
	fm->map_stm_effect[this] = cg_context.get_effect_stm();
	return true;
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
