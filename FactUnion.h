#ifndef FACTUNION_H
#define FACTUNION_H

///////////////////////////////////////////////////////////////////////////////
#include <ostream>
#include <vector>
#include "Fact.h"
#include "Variable.h"

class Function;
class Statement;
class Block;
class StatementAssign;
class StatementReturn;

using namespace std;

///////////////////////////////////////////////////////////////////////////////

class FactUnion : public Fact
{
public:
	static FactUnion *make_fact(const Variable* v, int fid = 0);
	static vector<const Fact*> make_facts(const vector<const Variable*>& vars, int fid);
	static void doFinalization();
	static bool is_nonreadable_field(const Variable *v, const std::vector<const Fact*>& facts);

	virtual ~FactUnion(void) {};

	virtual const Variable* get_var(void) const { return var;};
	void set_var(const Variable* v) { var = v;}
	const Type* get_last_written_type(void) const;
	int   get_last_written_fid(void) const { return last_written_fid; };
	static bool is_field_readable(const Variable* v, int fid, const vector<const Fact*>& facts);

	// lattice functions
	virtual bool is_top(void) const { return last_written_fid == TOP;}
	virtual bool is_bottom(void) const { return last_written_fid == BOTTOM;}
	virtual void set_top(void) { last_written_fid = TOP;}
	virtual void set_bottom(void) { last_written_fid = BOTTOM;}
	virtual bool imply(const Fact& fact) const;
	virtual bool equal(const Fact& fact) const;
	virtual int join(const Fact& fact);

	// transfer functions
	vector<const Fact*> rhs_to_lhs_transfer(const vector<const Fact*>& facts, const vector<const Variable*>& lvars, const Expression* rhs);
	virtual vector<const Fact*> abstract_fact_for_assign(const std::vector<const Fact*>& facts, const Lhs* lhs, const Expression* /*rhs*/);
	//virtual vector<const Fact*> abstract_fact_for_return(const std::vector<const Fact*>& facts, const ExpressionVariable* rv, const Function* func);
	virtual Fact* join_var_facts(const vector<const Fact*>& facts, const vector<const Variable*>& vars) const;
	virtual Fact* clone(void) const;

	// output functions
	virtual bool is_assertable(const Statement* /*s*/) const { return false;}
	virtual void Output(std::ostream &out) const;
	virtual void OutputAssertion(std::ostream &/*out*/, const Statement* /*s*/) const {};

	// constants to indicate lattice top/bottom
	static const int TOP;
	static const int BOTTOM;
private:
	FactUnion(const Variable* v);
	FactUnion(const Variable* v, int fid);

	const Variable* var;

	// last written field id
	int  last_written_fid;
};

///////////////////////////////////////////////////////////////////////////////

#endif // FactUnion_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
