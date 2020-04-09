#ifndef COMMON_H
#define COMMON_H

#include <algorithm>
#include <functional>
#include <vector>
#include <string>
#include <iostream>

using std::string;
using std::vector;
using namespace std;

#ifdef WIN32
#define INT64 __int64
#else
#define INT64 long long
#endif

#define BREAK_NOP {int i=0; i++;}

class Block;
class Expression;
class Function;
class FunctionInvocation;
class Statement;
class Type;
class Variable;


#endif // COMMON_H
