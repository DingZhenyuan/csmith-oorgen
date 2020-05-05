#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>
#include "Common.h"
using namespace std;

class StringUtils {
public:
	static bool empty_line(const std::string &line);

	static bool is_space(const char c);

	static bool end_with(string s, string tail);

	static void ignore_spaces(const std::string &str, size_t &pos);

	static std::string get_substring(const std::string &s, const char open_delim, const char close_delim);

	static std::string get_substring_before(const std::string &s, size_t &pos, const char close_delim);

	static int str2int(const std::string &s);

	static std::string int2str(int i);

	static INT64 str2longlong(const std::string &s);

	static std::string longlong2str(INT64 i);

	static char first_nonspace_char(const std::string &s);

	static size_t find_any_char(const string &s, size_t pos, const string& to_match);

	static void split_string(const string str, vector<string> &v, const char sep_char);

	static void split_string(const string str, vector<string> &v, string sep_chars);

	static void split_int_string(const string str, vector<int> &values, string sep_chars);

	static void chop(string& str);

	static void breakup_assigns(const string& assigns, vector<string>& vars, vector<string>& values);

private:
	StringUtils();

	~StringUtils();
};

#endif //STRING_UTILS_H
