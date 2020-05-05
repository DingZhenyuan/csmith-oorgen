#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "CrestExtension.h"
#include <cassert>
#include "Type.h"
#include "ExtensionValue.h"

using namespace std;

std::string CrestExtension::input_base_name_ = "CREST_";

CrestExtension::CrestExtension()
{

}

CrestExtension::~CrestExtension()
{
	values_.clear();
}

void
CrestExtension::GenerateValues()
{
	// Nothing to do
}

std::string
CrestExtension::type_to_string(const Type *t)
{
	assert(t->eType == eSimple);
	switch (t->simple_type) {
	case eChar: return "char";
	case eUChar: return "unsigned_char";
	case eShort: return "short";
	case eUShort: return "unsigned_short";
	case eInt: return "int";
	case eUInt: return "unsigned_int";
	case eLong: return "int";
	case eULong: return "unsigned_int";
	default: assert(0); break;
	}
	assert(0);
	return "";
}

void
CrestExtension::output_symbolics(ostream &out)
{
	std::vector<ExtensionValue*>::iterator i;
	for (i = values_.begin(); i != values_.end(); ++i) {
		assert(*i);
		out << AbsExtension::tab_;
		out << CrestExtension::input_base_name_ << type_to_string((*i)->get_type());
		out << "(" << (*i)->get_name() << ");" << std::endl;
	}
}

void
CrestExtension::OutputInit(std::ostream &out)
{
	out << "int main(void)" << endl;
	out << "{" << endl;
	AbsExtension::default_output_definitions(out, values_, false);
	output_symbolics(out);
}

void
CrestExtension::OutputHeader(std::ostream &out)
{
	out << "#include \"crest.h\"" << endl;
}

void
CrestExtension::OutputTail(std::ostream &out)
{
	out << AbsExtension::tab_ << "return 0;" << endl;
}

void
CrestExtension::OutputFirstFunInvocation(std::ostream &out, FunctionInvocation *invoke)
{
	AbsExtension::OutputFirstFunInvocation(out, invoke);
}

