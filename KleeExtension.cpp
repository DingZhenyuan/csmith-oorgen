#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "KleeExtension.h"
#include <cassert>
#include "Type.h"
#include "ExtensionValue.h"

using namespace std;

std::string KleeExtension::input_base_name_ = "input";

KleeExtension::KleeExtension()
{

}

KleeExtension::~KleeExtension()
{
	values_.clear();
}

void
KleeExtension::GenerateValues()
{

}

void
KleeExtension::output_symbolics(ostream &out)
{
	std::vector<ExtensionValue*>::iterator i;
	int count = 0;
	for (i = values_.begin(); i != values_.end(); ++i) {
		assert(*i);
		out << AbsExtension::tab_;
		out << "klee_make_symbolic(&" << (*i)->get_name() << ", sizeof(" << (*i)->get_name() << "), ";
		out << "\"" << KleeExtension::input_base_name_ << count << "\");" << endl;
		count++;
	}
}

void
KleeExtension::OutputInit(std::ostream &out)
{
	out << "int main(void)" << endl;
	out << "{" << endl;
	AbsExtension::default_output_definitions(out, values_, false);
	output_symbolics(out);
}

void
KleeExtension::OutputHeader(std::ostream &out)
{
	out << "#include \"klee/klee.h\"" << endl;
}

void
KleeExtension::OutputTail(std::ostream &out)
{
	out << AbsExtension::tab_ << "return 0;" << endl;
}

void
KleeExtension::OutputFirstFunInvocation(std::ostream &out, FunctionInvocation *invoke)
{
	AbsExtension::OutputFirstFunInvocation(out, invoke);
}

