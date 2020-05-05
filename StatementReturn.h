#ifndef STATEMENT_RETURN_H
#define STATEMENT_RETURN_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>

#include "Statement.h"
#include "ExpressionVariable.h"

class CGContext;
class Variable;
class Function;
class FactMgr;

/*
 *
 */
class StatementReturn : public Statement
{
public:
	// Factory method.
	static StatementReturn *make_random(CGContext &cg_context);

	StatementReturn(Block* b, const ExpressionVariable &v);
	StatementReturn(const StatementReturn &sr);
	virtual ~StatementReturn(void);

	virtual void Output(std::ostream &out, FactMgr* fm, int indent = 0) const;

	virtual bool must_return(void) const {return true;}

	virtual bool must_jump(void) const {return true;}

	virtual std::vector<const ExpressionVariable*> get_dereferenced_ptrs(void) const;

	virtual void get_blocks(std::vector<const Block*>& /* blks */) const {};
	virtual void get_exprs(std::vector<const Expression*>& exps) const {exps.push_back(&var);}

	virtual bool visit_facts(vector<const Fact*>& inputs, CGContext& cg_context) const;

    const ExpressionVariable* get_var(void) const { return &var;};

private:
	const ExpressionVariable &var;
	//

	StatementReturn &operator=(const StatementReturn &sr); // unimplementable
};

///////////////////////////////////////////////////////////////////////////////

#endif // STATEMENT_RETURN_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
