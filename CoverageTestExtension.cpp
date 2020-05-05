#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "CoverageTestExtension.h"
#include <cassert>
#include "Type.h"
#include "ExtensionValue.h"
#include "Constant.h"

using namespace std;

std::string CoverageTestExtension::array_base_name_ = "a";
std::string CoverageTestExtension::array_index_ = "test_index";

CoverageTestExtension::CoverageTestExtension(int inputs_size)
	: inputs_size_(inputs_size)
{
	assert(inputs_size_ > 0);
}

CoverageTestExtension::~CoverageTestExtension()
{
	values_.clear();
}

void
CoverageTestExtension::GenerateValues()
{
	vector<ExtensionValue *>::iterator i;
	for (i = values_.begin(); i != values_.end(); ++i) {
		const Type *type = (*i)->get_type();
		for (int j = 0; j < inputs_size_; ++j) {
			Constant *c = Constant::make_random(type);
			test_values_.push_back(c);
		}
	}
}

void
CoverageTestExtension::output_array_init(std::ostream &out, int count)
{
	if (inputs_size_ == 1) {
		test_values_[count]->Output(out);
		return;
	}
	int len = 0;
	int last_index = inputs_size_ + count - 1;
	for (int i = count; i < last_index; ++i) {
		if ((len % 10) == 0) {
			out << std::endl;
			out << AbsExtension::tab_ << AbsExtension::tab_;
		}
		test_values_[i]->Output(out);
		out << ", ";
		len++;
	}
	if ((len % 10) == 0) {
		out << std::endl;
		out << AbsExtension::tab_ << AbsExtension::tab_;
	}
	test_values_[last_index]->Output(out);
}

void
CoverageTestExtension::output_decls(std::ostream &out)
{
	AbsExtension::default_output_definitions(out, values_, false);
	vector<ExtensionValue *>::iterator i;
	int count = 0;
	for (i = values_.begin(); i != values_.end(); ++i) {
		out << AbsExtension::tab_;
		(*i)->get_type()->Output(out);
		out << " " << CoverageTestExtension::array_base_name_ << count;
		out << "[" << inputs_size_ << "] = {";
		output_array_init(out, count);
		out << "};" << std::endl;
		count++;
	}
	out << AbsExtension::tab_ << "int " << array_index_ << ";" << std::endl;
}

void
CoverageTestExtension::OutputFirstFunInvocation(std::ostream &out, FunctionInvocation *invoke)
{
	out << AbsExtension::tab_ << "for(" << array_index_ << " = 0; ";
	out << array_index_ << " < " << inputs_size_ << "; " << array_index_ << "++) {" << std::endl;
	vector<ExtensionValue *>::iterator i;
	int count = 0;
	for (i = values_.begin(); i != values_.end(); ++i) {
		out << AbsExtension::tab_ << AbsExtension::tab_;
		out << (*i)->get_name() << " = ";
		out << CoverageTestExtension::array_base_name_ << count;
		out << "[" << array_index_ << "];" << std::endl;
		count++;
	}
	assert(invoke);
	out << AbsExtension::tab_ << AbsExtension::tab_;
	invoke->Output(out);
	out << ";" << std::endl;
	out << AbsExtension::tab_ << "}" << std::endl;
}

void
CoverageTestExtension::OutputInit(std::ostream &out)
{
	out << "int main(void)" << endl;
	out << "{" << endl;
	output_decls(out);
	//output_array_init(out);
}

void
CoverageTestExtension::OutputHeader(std::ostream &)
{
	// Nothing to do
}

void
CoverageTestExtension::OutputTail(std::ostream &out)
{
	out << AbsExtension::tab_ << "return 0;" << endl;
}

