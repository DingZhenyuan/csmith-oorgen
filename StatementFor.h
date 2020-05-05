#ifndef STATEMENT_FOR_H
#define STATEMENT_FOR_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>

#include "Statement.h"
#include "StatementAssign.h"

class Block;
class CGContext;
class Expression;
class FactMgr;

/*
 *
 */
class StatementFor : public Statement
{
public:
	// Factory method.
	static StatementFor *make_random(CGContext &cg_context);

	static StatementFor *make_random_array_loop(const CGContext &cg_context);

	static const Variable* make_iteration(CGContext& cg_context, StatementAssign*& begin, Expression*& exit_cond, StatementAssign*& step, unsigned int& bound);

	StatementFor(Block* b,
				 const StatementAssign &init,
				 const Expression &test,
				 const StatementAssign &incr,
				 const Block &body);
	virtual ~StatementFor(void);

	void post_loop_analysis(CGContext& cg_context, vector<const Fact*>& pre_facts, Effect& pre_effect);
	const StatementAssign* get_init(void) const { return &init;};
	const Expression* get_test(void) const { return &test; };
	const StatementAssign* get_incr(void) const { return &incr; };
	const Block* get_body(void) const { return &body; };
	void output_header(std::ostream& out, int indent) const;
	//
	virtual void get_blocks(std::vector<const Block*>& blks) const { blks.push_back(&body);}

	virtual void get_exprs(std::vector<const Expression*>& exps) const {exps.push_back(&test);}

	virtual bool visit_facts(vector<const Fact*>& inputs, CGContext& cg_context) const;

	virtual void Output(std::ostream &out, FactMgr* fm, int indent = 0) const;

private:
	// XXX --- `init' and `incr' have the "wrong types."  Should be exprs.
	const StatementAssign &init;
	const Expression      &test;
	const StatementAssign &incr;

	const Block &body;

	//

	StatementFor(const StatementFor &sf);
	StatementFor &operator=(const StatementFor &sf); // unimplementable
};

///////////////////////////////////////////////////////////////////////////////

#endif // STATEMENT_FOR_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
