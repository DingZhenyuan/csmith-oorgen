#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "AbsExtension.h"
#include <sstream>
#include <cassert>
#include "Type.h"
#include "Function.h"
#include "FunctionInvocationUser.h"
#include "ExpressionVariable.h"
#include "ExtensionValue.h"
#include "Effect.h"
#include "Variable.h"
#include "VariableSelector.h"
#include "CGContext.h"

using namespace std;

const char *AbsExtension::tab_ = "    ";
const char *AbsExtension::base_name_ = "x";

void
AbsExtension::Initialize(unsigned int num, vector<ExtensionValue *> &values)
{
	for (unsigned int i = 0; i < num; i++) {
		const Type* type = Type::choose_random_simple();
		std::ostringstream s;
		s << AbsExtension::base_name_ << i;
		ExtensionValue *sv = new ExtensionValue(type, s.str());
		assert(sv);
		values.push_back(sv);
	}
}

FunctionInvocation*
AbsExtension::MakeFuncInvocation(Function *func, std::vector<ExtensionValue *> &values)
{
	FunctionInvocationUser *fi = new FunctionInvocationUser(func, true, NULL);

	vector<ExtensionValue *>::iterator i;
	for (i = values.begin(); i != values.end(); ++i) {
		assert(*i);
		CVQualifiers qfer = (*i)->get_qfer();
		Variable *v = VariableSelector::new_variable((*i)->get_name(), (*i)->get_type(),
								NULL, &qfer);
		Expression *p = new ExpressionVariable(*v, (*i)->get_type());
		fi->param_value.push_back(p);
	}
	return fi;
}

void
AbsExtension::GenerateFirstParameterList(Function &curFunc, std::vector<ExtensionValue *> &values)
{
	vector<ExtensionValue *>::iterator i;
	for (i = values.begin(); i != values.end(); ++i) {
		assert(*i);
		CVQualifiers qfer = (*i)->get_qfer();
		Variable * v = VariableSelector::GenerateParameterVariable((*i)->get_type(), &qfer);
		assert(v);
		curFunc.param.push_back(v);
	}
}

void
AbsExtension::default_output_definitions(ostream &out, std::vector<ExtensionValue *> &values, bool init_flag)
{
	std::vector<ExtensionValue*>::iterator i;
	for (i = values.begin(); i != values.end(); ++i) {
		assert(*i);
		out << AbsExtension::tab_;
		(*i)->get_type()->Output(out);
		out << " " << (*i)->get_name();
		if (init_flag)
			out << " = 0";
		out << ";" << endl;
	}
	out << endl;
}

void
AbsExtension::OutputFirstFunInvocation(std::ostream &out, FunctionInvocation *invoke)
{
	assert(invoke);
	out << "    ";
        invoke->Output(out);
        out << ";" << endl;
}

AbsExtension::AbsExtension()
{

}

AbsExtension::~AbsExtension()
{

}

