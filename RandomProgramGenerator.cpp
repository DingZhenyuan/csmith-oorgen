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
#include "CGOptions.h"

using namespace std;

// 程序种子 - 允许用户在不同平台生成同样的程序
static unsigned long g_Seed = 0;

bool parse_string_arg(const char *arg, string &s) {

}

// 显示help信息
static void print_help() {

}

// 显示高级help信息
static void print_advanced_help() {

}

// 入口
int main(int argc, char **argv) {
    g_Seed = platform_gen_seed();
    CGOptions::set_default_settings();
    // 分析command
    for (int i = 0; i < argc; i++) {
        // --help -h
        if (strcmp (argv[i], "--help") == 0 || strcmp (argv[i], "-h") == 0) {
            print_help();
            return 0;
        }
        // -hh
        if (strcmp (argv[i], "-hh") == 0) {
            print_advanced_help();
            return 0;
        }
        
    }
}