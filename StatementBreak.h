#ifndef STATEMENT_BREAK_H
#define STATEMENT_BREAK_H

///////////////////////////////////////////////////////////////////////////////
#include <ostream>
#include "Statement.h"

class Expression;
class Block;
class CGContext;
/*
 *
 */
class StatementBreak : public Statement
{
public:
	// Factory method.
	static StatementBreak *make_random(CGContext &cg_context);

	StatementBreak(Block* parent, const Expression &test, const Block& b);
	StatementBreak(const StatementBreak &sc);
	virtual ~StatementBreak(void);
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

#endif // STATEMENT_BREAK_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
