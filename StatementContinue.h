#ifndef STATEMENT_CONTINUE_H
#define STATEMENT_CONTINUE_H

///////////////////////////////////////////////////////////////////////////////
#include <ostream>
#include "Statement.h"

class Expression;
class Block;
class CGContext;
/*
 *
 */
class StatementContinue : public Statement
{
public:
	// Factory method.
	static StatementContinue *make_random(CGContext &cg_context);

	StatementContinue(Block* parent, const Expression &test, const Block& b);
	StatementContinue(const StatementContinue &sc);
	virtual ~StatementContinue(void);
	//
	virtual bool must_jump(void) const;
	virtual void get_blocks(std::vector<const Block*>& /* blks */) const {};
	virtual void get_exprs(std::vector<const Expression*>& exps) const {exps.push_back(&test);}
	virtual bool visit_facts(vector<const Fact*>& inputs, CGContext& cg_context) const;
	virtual void Output(std::ostream &out, FactMgr* fm, int indent = 0) const;

	const Expression &test;
	const Block& loop_blk;
};

///////////////////////////////////////////////////////////////////////////////

#endif // STATEMENT_CONTINUE_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
