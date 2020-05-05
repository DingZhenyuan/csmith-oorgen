#ifndef STATEMENT_IF_H
#define STATEMENT_IF_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>

#include "Statement.h"

class Block;
class CGContext;
class Expression;
class FactMgr;

/*
 *
 */
class StatementIf : public Statement
{
public:
	// Factory method.
	static StatementIf *make_random(CGContext &cg_context);

	StatementIf(Block* b, const Expression &test,
				const Block &if_true, const Block &if_false);
	StatementIf(const StatementIf &si);
	virtual ~StatementIf(void);

	virtual void get_exprs(std::vector<const Expression*>& exps) const {exps.push_back(&test);}
	virtual void get_blocks(std::vector<const Block*>& blks) const { blks.push_back(&if_true); blks.push_back(&if_false);}

	const Block* get_true_branch(void) const { return &if_true; };
	const Block* get_false_branch(void) const { return &if_false; };
	const Expression* get_test(void) const { return &test; };

	void combine_branch_facts(vector<const Fact*>& pre_facts) const;

	virtual bool must_return(void) const;
	virtual bool must_jump(void) const;
	//
	virtual bool visit_facts(vector<const Fact*>& inputs, CGContext& cg_context) const;

	virtual void Output(std::ostream &out, FactMgr* fm, int indent = 0) const;
	void output_condition(std::ostream &out, FactMgr* fm, int indent = 0) const;
	void output_branches(std::ostream &out, FactMgr* fm, int indent = 0) const;

private:
	const Expression &test;
	const Block &if_true;
	const Block &if_false;

	//

	StatementIf &operator=(const StatementIf &si); // unimplementable
};

///////////////////////////////////////////////////////////////////////////////

#endif // STATEMENT_IF_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
