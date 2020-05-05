#ifndef COMPATIBLE_CHECKER_H
#define COMPATIBLE_CHECKER_H

class Variable;
class Expression;

class CompatibleChecker {
public:
	// return true if two args are identical
	static bool compatible_check(const Variable *v, const Expression *exp);

	static bool compatible_check(const Expression *exp1, const Expression *exp2);

	static void enable_compatible_check();

private:
	static bool compatible_check_;
};

#endif // COMPATIBLE_CHECKER_H
