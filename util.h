#ifndef UTIL_H
#define UTIL_H

///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include <ostream>

typedef std::vector<int> intvec;

std::string gensym(const char *basename);
std::string gensym(const std::string &basename);
void reset_gensym();
std::vector<intvec> permute(intvec in);
int expand_within_ranges(std::vector<unsigned int>, std::vector<intvec>& out);
void outputln(std::ostream &out);
void really_outputln(std::ostream &out);
void output_comment_line(std::ostream &out, const std::string &comment);
void output_tab(std::ostream &out, int indent);
void output_print_str(std::ostream& out, std::string str, std::string str_value, int indent);
void output_open_encloser(const char* symbol, std::ostream &out, int& indent);
void output_close_encloser(const char* symbol, std::ostream &out, int& indent, bool no_newline = false);

///////////////////////////////////////////////////////////////////////////////

#endif // UTIL_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
