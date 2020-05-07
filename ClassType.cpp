#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "ClassType.h"

using namespace std;

int ClassType::num = 0;

ClassType::ClassType() {
    num++;
};

ClassType::~ClassType() {};

// 添加attributes，不设置value
void ClassType::addAttribute(string name, eSimpleType type) {
    SimpleAttribute* sa = new SimpleAttribute();
    sa->name = name;
    sa->type = type;
    sa->value = "";
    simpleAttributes.push_back(*sa);
}

// 添加attributes，设置value
void ClassType::addAttribute(string name, eSimpleType type, string value) {
    SimpleAttribute* sa = new SimpleAttribute();
    sa->name = name;
    sa->type = type;
    sa->value = value;
    simpleAttributes.push_back(*sa);
}


