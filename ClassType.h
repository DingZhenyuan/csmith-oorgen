#ifndef CLASSTYPE_H
#define CLASSTYPE_H

#include <string>
#include <vector>

#include "Type.h"
#include "Function.h"

using namespace std;

struct SimpleAttribute {
    string name;
    eSimpleType type;
    string value;
};

class ClassType {
public:
    ClassType();
    ~ClassType();
    
    // 获取class个数
    int getNum() { return num; }
    // name
    void setName(string _name) { name = _name; }
    string getName() { return name; }
    // abstract
    void setAbstract(bool _abstract) { abstract = _abstract; };
    bool getAbstract() { return abstract; }
    // parent
    void setParent(string _parent) { parent = _parent; }
    string getParent() { return parent; }

    // 获取本class的attributes
    vector<SimpleAttribute> getAttributes() { return simpleAttributes; }
    
    // 添加attributes,不设置value
    void addAttribute(string name, eSimpleType type);
    // 添加attributes，设置value
    void addAttribute(string name, eSimpleType type, string value);

    // 添加function
    void addFunction(Function* function) { functions.push_back(function); }
    // 获得functions
    vector<Function*> getFunctions() { return functions; }

private:
    static int num;
    string name = "";
    bool abstract = false;
    string parent = "";
    vector<SimpleAttribute> simpleAttributes;
    vector<Function*> functions;
};

#endif // CLASSTYPE_H
