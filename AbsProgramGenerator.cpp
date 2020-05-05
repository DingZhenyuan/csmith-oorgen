#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "AbsProgramGenerator.h"
#include <iostream>
#include <cassert>
#include <string>
#include "CGOptions.h"
#include "DefaultProgramGenerator.h"
#include "DFSProgramGenerator.h"
#include "Probabilities.h"

using namespace std;

AbsProgramGenerator *AbsProgramGenerator::current_generator_ = NULL;

OutputMgr *
AbsProgramGenerator::GetOutputMgr()
{
	assert(current_generator_);
	return current_generator_->getOutputMgr();
}

AbsProgramGenerator *
AbsProgramGenerator::CreateInstance(int argc, char *argv[], unsigned long seed)
{
	if (CGOptions::dfs_exhaustive()) {
		AbsProgramGenerator::current_generator_ = new DFSProgramGenerator(argc, argv, seed);
	}
	else
	{
		AbsProgramGenerator::current_generator_ = new DefaultProgramGenerator(argc, argv, seed);
	}

	AbsProgramGenerator::current_generator_->initialize();

	if (!(CGOptions::dump_default_probabilities().empty())) {
		CGOptions::random_random(false);
		Probabilities *prob = Probabilities::GetInstance();
		prob->dump_default_probabilities(CGOptions::dump_default_probabilities());
		exit(0);
	}
	else if (!(CGOptions::dump_random_probabilities().empty())) {
		CGOptions::random_random(true);
		Probabilities *prob = Probabilities::GetInstance();
		prob->dump_actual_probabilities(CGOptions::dump_random_probabilities(), seed);
		exit(0);
	}

	Probabilities *prob = Probabilities::GetInstance();
	string msg;
	if (!(CGOptions::probability_configuration().empty())) {
		if (!prob->parse_configuration(msg, CGOptions::probability_configuration())) {
			cout << "parsing configuration file error:" << msg << endl;
			return NULL;
		}
	}

	return AbsProgramGenerator::current_generator_;
}

AbsProgramGenerator *
AbsProgramGenerator::GetInstance()
{
	assert(AbsProgramGenerator::current_generator_);
	return AbsProgramGenerator::current_generator_;
}

AbsProgramGenerator::AbsProgramGenerator()
{

}

AbsProgramGenerator::~AbsProgramGenerator()
{

}
