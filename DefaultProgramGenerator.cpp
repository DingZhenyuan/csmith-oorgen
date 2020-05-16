#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "DefaultProgramGenerator.h"
#include <cassert>
#include <sstream>
#include <fstream>
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
#include "ModelReader.h"
#include "InheritTree.h"

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

void DefaultProgramGenerator::goGenerator() {
	// 读取model
	vector<ClassType> classTypes;
	ModelReader::Read(classTypes);
	// ModelReader::printClasses(classTypes);

	// 构造继承关系树
	InheritTree* inheritTree = new InheritTree();
	inheritTree->init(classTypes);

	// 生成所有Types
	GenerateAllTypes();

	// 生成所有Functions
	GenerateFunctions();

	// 每个文件中生成function的个数
	int funcNumPerClass = FuncListSize()/classTypes.size();
	// cout << FuncListSize() << endl;
	// cout << funcNumPerClass << endl;
	// 分文件打印输出
	for (int i = 0; i < classTypes.size(); i++) {
		int funcNum;
		if (i < classTypes.size() - 1) {
			funcNum = funcNumPerClass;
		} else {
			funcNum = FuncListSize() - funcNumPerClass * i;
		}

		ofstream out_c("codes/" + classTypes[i].getName() + ".cpp");
		
		// 打印头
		output_mgr_->OutputHeaderClass(argc_, argv_, seed_, out_c);
		output_mgr_->OutputClass(classTypes[i], out_c);

		// Function::GenerateMemberFunction(funcNumPerClass * i, funcNumPerClass);
		// output_mgr_->OutputFunc(out_c);

		bool outVariable = false;
		if (classTypes[i].getParent() == "") {
			outVariable = true;
		}
		output_mgr_->OutputFunc(funcNumPerClass * i, funcNum, out_c, outVariable, false);

		out_c.close();
	}

	ofstream out_c("codes/main.cpp");
	// 输出main函数的头
	output_mgr_->OutputHeaderClass(argc_, argv_, seed_, out_c);

	// 输出main函数相关
	output_mgr_->OutputFunc(0, 0, out_c, false, true);
	out_c.close();
	
	// output_mgr_->OutputHeader(argc_, argv_, seed_);
	// // 生成所有Types
	// GenerateAllTypes();
	// // 生成functions
	// GenerateFunctions();
	// output_mgr_->Output();
	// if (CGOptions::identify_wrappers()) {
	// 	ofstream ofile;
	// 	ofile.open("wrapper.h");
	// 	ofile << "#define N_WRAP " << SafeOpFlags::wrapper_names.size() << std::endl;
	// 	ofile.close();
	// }
}
