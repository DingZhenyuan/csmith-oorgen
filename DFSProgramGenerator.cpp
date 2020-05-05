#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "DFSProgramGenerator.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include "RandomNumber.h"
#include "AbsRndNumGenerator.h"
#include "DFSRndNumGenerator.h"
#include "DFSOutputMgr.h"
#include "Finalization.h"
#include "Error.h"
#include "Function.h"
#include "VariableSelector.h"
#include "util.h"
#include "PartialExpander.h"

using namespace std;

DFSProgramGenerator::DFSProgramGenerator(int argc, char *argv[], unsigned long seed)
	: argc_(argc),
	  argv_(argv),
	  seed_(seed),
	  good_count_(0),
	  output_mgr_(NULL)
{

}

DFSProgramGenerator::~DFSProgramGenerator()
{
	Finalization::doFinalization();
	delete output_mgr_;
}

void
DFSProgramGenerator::initialize()
{
	RandomNumber::CreateInstance(rDFSRndNumGenerator, seed_);
	output_mgr_ = DFSOutputMgr::CreateInstance();
	assert(output_mgr_);
}

std::string
DFSProgramGenerator::get_count_prefix(const std::string &name)
{
	std::ostringstream ss;
	ss << "p_" << good_count_ << "_" << name;
	return ss.str();
}

void
DFSProgramGenerator::goGenerator()
{
	DFSRndNumGenerator *impl =
		dynamic_cast<DFSRndNumGenerator*>(RandomNumber::GetRndNumGenerator());
	//unsigned long long count = 0;
	GenerateAllTypes();
	output_mgr_->OutputStructUnions(cout);
	while(!impl->get_all_done()) {
		Error::set_error(SUCCESS);
		GenerateFunctions();
		if (Error::get_error() == SUCCESS) {
			//count++;
			//if (count >= 47376)
				//cout << "here" << std::endl;
			output_mgr_->OutputHeader(argc_, argv_, seed_);
			output_mgr_->Output();
			OutputMgr::really_outputln(cout);
			good_count_++;
		}
		impl->reset_state();
		Function::doFinalization();
		VariableSelector::doFinalization();
		reset_gensym();
		PartialExpander::restore_init_values();
		//cout << "count = " << count << std::endl;
	}
}

