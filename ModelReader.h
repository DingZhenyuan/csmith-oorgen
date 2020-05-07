#ifndef MODEL_READER
#define MODEL_READER

#include <string>
#include <vector>
#include "ClassType.h"

using namespace std;

class ModelReader {
public:
    static string chop(string str);

    static vector<string> splitString(const string str, const string sep);

    static vector<string> splitStringNonSimple(const string str, const string sep);

    static bool containString(const string str, const string s);

    static eSimpleType strToSimpleType(string type);

    static string SimpleTypeToStr(eSimpleType type);

    static void analyze(vector<string> v, vector<ClassType>& classTypes, int& state);

    static void printClasses(vector<ClassType> classTypes);

    static void Read(vector<ClassType>& classTypes);

private:
    
    ModelReader();
    ~ModelReader();
};

#endif