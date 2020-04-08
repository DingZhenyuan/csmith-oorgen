#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef WIN32
#pragma warning(disable : 4786) // 禁止烦人的警告消息
#endif

#include <ostream>
#include <fstream>
#include <cstring>
#include <cstdio>

#include "platform.h"

using namespace std;

// 程序种子 - 允许用户在不同平台生成同样的程序
static unsigned long g_Seed = 0;

bool parse_string_arg(const char *arg, string &s) {

}

// 入口
int main(int argc, char **argv) {
    g_Seed = platform_gen_seed();
}