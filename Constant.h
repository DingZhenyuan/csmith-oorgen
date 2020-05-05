#ifndef CONSTANT_H
#define CONSTANT_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>
#include <string>
#include <vector>

#include "Expression.h"
#include "Type.h"

class CGContext;
class Variable;

/*
 *
 */
class Constant : public Expression
{
public:
	// Factory method.
	static Constant *make_random(const Type* type);
	static Constant *make_random_upto(unsigned int limit);
	static Constant *make_random_nonzero(const Type* type);

	virtual bool compatible(const Variable *v) const;

	virtual bool compatible(const Expression *exp) const;

	// Factory method.
	static Constant *make_int(int v);

	Expression *clone() const;

	CVQualifiers get_qualifiers(void) const { return CVQualifiers(true, false);}

	virtual void get_eval_to_subexps(vector<const Expression*>& subs) const {subs.push_back(this);}

	Constant(const Type *t, const std::string &v);
	explicit Constant(const Constant &c);
	virtual ~Constant(void);

	//

	virtual const Type &get_type(void) const;
	// Unused:
	const std::string &get_value(void) const { return value; }

	string get_field(size_t fid) const;

	virtual bool less_than(int num) const;
	virtual bool not_equals(int num) const;
	virtual bool equals(int num) const;

	virtual void get_referenced_ptrs(std::vector<const Variable*>& /*ptrs*/) const {};
	virtual unsigned int get_complexity(void) const { return 1;}
	// unsigned long SizeInBytes(void) const;
	virtual void Output(std::ostream &) const;

private:
	const Type* type;
	const std::string value;
};

///////////////////////////////////////////////////////////////////////////////

#endif // CONSTANT_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
