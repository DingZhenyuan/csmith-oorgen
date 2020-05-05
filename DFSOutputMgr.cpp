#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "DFSOutputMgr.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include "CGOptions.h"
#include "Function.h"
#include "Type.h"
#include "VariableSelector.h"

using namespace std;

DFSOutputMgr *DFSOutputMgr::instance_ = NULL;

DFSOutputMgr::DFSOutputMgr()
{

}

DFSOutputMgr::~DFSOutputMgr()
{

}

DFSOutputMgr *
DFSOutputMgr::CreateInstance()
{
	if (DFSOutputMgr::instance_)
		return DFSOutputMgr::instance_;

	DFSOutputMgr::instance_ = new DFSOutputMgr();
	std::string s_output = CGOptions::struct_output();
	if (s_output.empty())
		instance_->struct_output_ = DEFAULT_STRUCT_OUTPUT;
	else
		instance_->struct_output_ = s_output;

	assert(DFSOutputMgr::instance_);
	return DFSOutputMgr::instance_;
}

void
DFSOutputMgr::OutputHeader(int argc, char *argv[], unsigned long seed)
{
	if (!CGOptions::compact_output())
		OutputMgr::OutputHeader(argc, argv, seed);
}

void
DFSOutputMgr::OutputStructUnions(ostream& /*out*/)
{
	ofstream o_struct(struct_output_.c_str());
	OutputStructUnionDeclarations(o_struct);
	o_struct.close();
}

void
DFSOutputMgr::Output()
{
	std::ostream &out = get_main_out();

	OutputGlobalVariables(out);
	if (!CGOptions::compact_output())
		OutputForwardDeclarations(out);
	OutputFunctions(out);

	if (CGOptions::step_hash_by_stmt()) {
		OutputMgr::OutputHashFuncDef(out);
		OutputMgr::OutputStepHashFuncDef(out);
	}

	if (!CGOptions::compact_output())
		OutputMain(out);
}

std::ostream &
DFSOutputMgr::get_main_out()
{
	return std::cout;
}

void
DFSOutputMgr::outputln(ostream &out)
{
	if (!CGOptions::compact_output())
		out << std::endl;
}

void
DFSOutputMgr::output_comment_line(ostream &out, const std::string &comment)
{
	if (!CGOptions::compact_output())
		OutputMgr::output_comment_line(out, comment);
}

void
DFSOutputMgr::output_tab(ostream &out, int indent)
{
	if (!CGOptions::compact_output())
		OutputMgr::output_tab(out, indent);
}

