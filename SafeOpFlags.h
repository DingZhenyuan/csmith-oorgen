#ifndef SAFEOPFLAGS_H
#define SAFEOPFLAGS_H

#include <ostream>
#include "FunctionInvocation.h"
#include "Type.h"

enum SafeOpKind {
	sOpUnary,
	sOpBinary,
	sOpAssign,
};

#define MAX_SAFE_OP_KIND ((SafeOpKind) (sOpAssign+1))

enum SafeOpSize {
	sInt8,
	sInt16,
	sInt32,
	sInt64,
	sFloat,
};
#define MAX_SAFE_OP_SIZE ((SafeOpSize) (sFloat+1))

class SafeOpFlags {
public:
	static SafeOpFlags *make_random_binary(const Type *rv_type, const Type *op1_type, const Type *op2_type,
					SafeOpKind op_kind, eBinaryOps op);

	static SafeOpFlags *make_random_unary(const Type *rv_type, const Type *op1_type, eUnaryOps op);

	static SafeOpFlags *make_dummy_flags();

	static eSimpleType flags_to_type(bool sign, enum SafeOpSize size);

	const Type* get_lhs_type(void);
	const Type* get_rhs_type(void);

	SafeOpFlags *clone() const;

	void OutputSize(std::ostream &out) const;

	void OutputFuncOrMacro(std::ostream &out) const;

	void OutputOp1(std::ostream &out) const;

	void OutputOp2(std::ostream &out) const;

	bool get_op1_sign() { return op1_; }

	bool get_op2_sign() { return op2_; }

	enum SafeOpSize get_op_size() const { return op_size_; }

	std::string to_string(enum eBinaryOps op) const;
	std::string to_string(enum eUnaryOps  op) const;
	static int to_id(std::string fname);

	~SafeOpFlags();

	static std::vector<std::string> wrapper_names;
private:
	bool op1_;
	bool op2_;
	bool is_func_;
	SafeOpSize op_size_;

	void OutputSign(std::ostream &out, bool sgnd) const;

	bool static return_float_type(const Type *rv_type, const Type *op1_type, const Type *op2_type,
					eBinaryOps op);

	bool static return_float_type(const Type *rv_type, const Type *op1_type,
					eUnaryOps uop);

	std::string safe_float_func_string(enum eBinaryOps op) const;

	SafeOpFlags();

	SafeOpFlags(const SafeOpFlags &flags);

	SafeOpFlags(bool op1, bool op2, bool is_func, SafeOpSize size);

	SafeOpFlags &operator=(const SafeOpFlags &flags); //unimplemented;
};

#endif //SAFEOPFLAGS_H

