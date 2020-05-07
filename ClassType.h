#ifndef CLASSTYPE_H
#define CLASSTYPE_H

#include <string>
#include <vector>

#include "Type.h"

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
    // 获取本class的attributes
    vector<SimpleAttribute> getAttributes() { return simpleAttributes; }
    
    // 添加attributes,不设置value
    void addAttribute(string name, eSimpleType type);
    // 添加attributes，设置value
    void addAttribute(string name, eSimpleType type, string value);

private:
    static int num;
    string name = "";
    bool abstract = false;
    vector<SimpleAttribute> simpleAttributes;
};

#endif // CLASSTYPE_H
