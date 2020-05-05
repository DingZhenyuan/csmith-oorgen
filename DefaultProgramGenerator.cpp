#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "DefaultProgramGenerator.h"
#include <cassert>
#include <sstream>
#include "RandomNumber.h"
#include "AbsRndNumGenerator.h"
#include "DefaultOutputMgr.h"
#include "ReducerOutputMgr.h"
#include "Finalization.h"
#include "Function.h"
#include "Type.h"
#include "DeltaMonitor.h"
#include "CGOptions.h"
#include "SafeOpFlags.h"
#include "ExtensionMgr.h"

DefaultProgramGenerator::DefaultProgramGenerator(int argc, char *argv[], unsigned long seed)
	: argc_(argc),
	  argv_(argv),
	  seed_(seed),
	  output_mgr_(NULL)
{

}

DefaultProgramGenerator::~DefaultProgramGenerator()
{
	Finalization::doFinalization();
	delete output_mgr_;
}

void
DefaultProgramGenerator::initialize()
{
	if (DeltaMonitor::is_delta()) {
		DeltaMonitor::CreateRndNumInstance(seed_);
	}
	else {
		RandomNumber::CreateInstance(rDefaultRndNumGenerator, seed_);
	}
	if (CGOptions::get_reducer()) {
		output_mgr_ = new ReducerOutputMgr();
	} else {
		output_mgr_ = DefaultOutputMgr::CreateInstance();
	}
	assert(output_mgr_);

	ExtensionMgr::CreateExtension();
}

std::string
DefaultProgramGenerator::get_count_prefix(const std::string &)
{
	assert(0);
	return "";
}

void
DefaultProgramGenerator::goGenerator()
{
	output_mgr_->OutputHeader(argc_, argv_, seed_);

	GenerateAllTypes();
	GenerateFunctions();
	output_mgr_->Output();
	if (CGOptions::identify_wrappers()) {
		ofstream ofile;
		ofile.open("wrapper.h");
		ofile << "#define N_WRAP " << SafeOpFlags::wrapper_names.size() << std::endl;
		ofile.close();
	}
}
