#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "SplatExtension.h"
#include <cassert>
#include "Type.h"
#include "Constant.h"
#include "ExtensionValue.h"

using namespace std;

SplatExtension::SplatExtension()
{

}

SplatExtension::~SplatExtension()
{
	std::vector<ExtensionValue*>::iterator i;
	for (i = values_.begin(); i != values_.end(); ++i) {
		delete (*i);
	}
	values_.clear();
}

void
SplatExtension::GenerateValues()
{
	vector<ExtensionValue *>::iterator i;
	for (i = values_.begin(); i != values_.end(); ++i) {
		const Type *type = (*i)->get_type();
		Constant *c =  Constant::make_random(type);
		(*i)->set_value(c);
	}
}

void
SplatExtension::output_symbolics(ostream &out)
{
	std::vector<ExtensionValue*>::iterator i;
	out << "#ifdef __SPLAT__" << endl;
	for (i = values_.begin(); i != values_.end(); ++i) {
		assert(*i);
		out << AbsExtension::tab_;
		int bits = (*i)->get_type()->SizeInBytes() * 8;
		out << "setInput(&" << (*i)->get_name() << ", " << bits << ", ";
		Constant *value = (*i)->get_value();
		assert(value);
		value->Output(out);
		out << ");" << endl;
	}
	out << "#endif" << endl << endl;
}

void
SplatExtension::OutputHeader(std::ostream &out)
{
	out << "#ifdef __SPLAT__" << endl;
	out << "#include \"../includes/instrument.h\"" << endl;
	out << "#endif" << endl;
}

void
SplatExtension::OutputTail(std::ostream &out)
{
	out << "#ifdef __SPLAT__" << endl;
	out << AbsExtension::tab_ << "__splat_exit(ExitNoBug);" << endl;
	out << "#else" << endl;
	out << AbsExtension::tab_ << "return 0;" << endl;
	out << "#endif" << endl;
}

void
SplatExtension::OutputInit(std::ostream &out)
{
	out << "#ifdef __SPLAT__" << endl;
	out << "void __splat_instrumentedCode()" << endl;
	out << "#else" << endl;
	out << "int main(void)" << endl;
	out << "#endif" << endl;
	out << "{" << endl;
	AbsExtension::default_output_definitions(out, values_, true);
	output_symbolics(out);
}

void
SplatExtension::OutputFirstFunInvocation(std::ostream &out, FunctionInvocation *invoke)
{
	AbsExtension::OutputFirstFunInvocation(out, invoke);
}

