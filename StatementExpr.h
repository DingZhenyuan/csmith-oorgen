#ifndef STATEMENT_EXPR_H
#define STATEMENT_EXPR_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>
#include <vector>

#include "Statement.h"
#include "ExpressionFuncall.h"

class CGContext;
class FunctionInvocation;
class FunctionInvocationUser;
class FactMgr;

/*
 *
 */
class StatementExpr : public Statement
{
public:
	// Factory method.
	static StatementExpr *make_random(CGContext &cg_context);

	StatementExpr(Block* b, const FunctionInvocation &e);
	StatementExpr(const StatementExpr &se);
	virtual ~StatementExpr(void);

	//
	const FunctionInvocation* get_invoke(void) const { return expr.get_invoke(); };
	const ExpressionFuncall* get_call(void) const { return &expr;}

	virtual bool visit_facts(vector<const Fact*>& inputs, CGContext& cg_context) const;

	virtual std::vector<const ExpressionVariable*> get_dereferenced_ptrs(void) const;

	virtual bool has_uncertain_call_recursive(void) const;

	virtual void get_blocks(std::vector<const Block*>& /* blks */) const {};
	virtual void get_exprs(std::vector<const Expression*>& exps) const {exps.push_back(&expr);}

	virtual void Output(std::ostream &out, FactMgr* fm, int indent = 0) const;

private:
	const ExpressionFuncall expr;

	//

	StatementExpr &operator=(const StatementExpr &se); // unimplementable
};

///////////////////////////////////////////////////////////////////////////////

#endif // STATEMENT_EXPR_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
