#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>

#include "Common.h"

using namespace std;

class StringUtils {
public:
    // 判断字符串是否为空
    static bool empty_line(const std::string &line);
    // 判断字符是否为空位（' ', '\t', '\n'）
	static bool is_space(const char c);
    // 判断字符串是否由某字符串结尾
	static bool end_with(string s, string tail);
    // 越过空位
	static void ignore_spaces(const std::string &str, size_t &pos);
    // 根据开始字符和结束字符获取子串
	static std::string get_substring(const std::string &s, const char open_delim, const char close_delim);
    // 根据开始索引和结尾字符获取子串
	static std::string get_substring_before(const std::string &s, size_t &pos, const char close_delim);
    // 字符串转int
	static int str2int(const std::string &s);
    // int转字符串
	static std::string int2str(int i);
    // 字符串转longlong
	static INT64 str2longlong(const std::string &s);
    // longlong转字符串
	static std::string longlong2str(INT64 i);
    // 获取字符串中第一个非空位的字符
	static char first_nonspace_char(const std::string &s);
    // 在字符串中寻找字符，返回首个出现位置
	static size_t find_any_char(const string &s, size_t pos, const string& to_match);
    // 以字符分割字符串
	static void split_string(const string str, vector<string> &v, const char sep_char);
    // 以字符串分割字符串
	static void split_string(const string str, vector<string> &v, string sep_chars);
    // 将字符串分割成int值的子串
	static void split_int_string(const string str, vector<int> &values, string sep_chars);
    // 将字符串两侧空位去掉
	static void chop(string& str);
    // 将赋值字符串拆分，将变量和值分别存储
	static void breakup_assigns(const string& assigns, vector<string>& vars, vector<string>& values);

private:
	StringUtils();
	~StringUtils();
};

#endif // STRING_UTILS_H
