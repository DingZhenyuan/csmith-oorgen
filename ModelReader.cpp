#include "ModelReader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "ClassType.h"

#define WAIT_STATE 0
#define CLASS_STATE 1
#define ATTRIBUTE_STATE 2

ModelReader::ModelReader() {}
ModelReader::~ModelReader() {}

// 去除两侧空位
string ModelReader::chop(string str) {
	string s;
	size_t last;
	while (str[0] == '\t' || str[0] == ' ') {
		str = str.substr(1);
	}
	last = str.length() - 1;
	while (str[last] == '\t' || str[last] == ' ') {
		str = str.substr(0, last);
		last--;
	}
    return str;
}

// 按照分割字符串分割字符串
vector<string> ModelReader::splitString(const string str, const string sep) {
    string::size_type pos1, pos2;
    pos2 = str.find(sep);
    pos1 = 0;
    vector<string> v;
    while (string::npos != pos2) {
        v.push_back(str.substr(pos1, pos2-pos1));
        pos1 = pos2 + sep.size();
        pos2 =  str.find(sep, pos1);
    }
    if (pos1 != str.length()) {
        v.push_back(str.substr(pos1));
    }
    return v;
}

// 分割字符串，忽略其中的简单行
vector<string> ModelReader::splitStringNonSimple(const string str, const string sep) {
    string::size_type pos1, pos2;
    pos2 = str.find(sep);
    pos1 = 0;
    vector<string> v;
    while (string::npos != pos2) {
        string substring = str.substr(pos1, pos2-pos1);
        if (substring.size() > 1) {
            v.push_back(substring);
        }
        
        pos1 = pos2 + sep.size();
        pos2 =  str.find(sep, pos1);
    }
    // 单独一个空格的不予保留
    if (pos1 != str.length()) {
        if (str.substr(pos1).size() > 1) {
            v.push_back(str.substr(pos1));
        }
    }
    return v;
}

// 判断字符串中是否包含某字符串
bool ModelReader::containString(const string str, const string s) {
    string::size_type index = str.find(s);
    if (index == string::npos) {
        return false;
    } else {
        return true;
    }
}

// 解析字符串对应的type
eSimpleType ModelReader::strToSimpleType(string type) {
    if (type == "EVoid") return eVoid;
    else if (type == "EChar") return eChar;
    else if (type == "EInt") return eInt;
    else if (type == "EShort") return eShort;
    else if (type == "ELong") return eLong;
    else if (type == "ELongLong") return eLongLong;
    else if (type == "EUChar") return eUChar;
    else if (type == "EUInt") return eUInt;
    else if (type == "EUShort") return eUShort;
    else if (type == "EULong") return eULong;
    else if (type == "EFloat") return eFloat;
    else if (type == "EULongLong") return eULongLong;
    else {
        cout << type << endl;
        cout << "simple types error" << endl;
    }
}

// 将SimpleType翻译成对应的代码
string ModelReader::SimpleTypeToStr(eSimpleType type) {
    switch(type) {
        case 0:
            return "void";
            break;
        case 1:
            return "char";
            break;
        case 2:
            return "short";
            break;
        case 3:
            return "long";
            break;
        case 4:
            return "long long";
            break;
        case 5:
            return "unsigned char";
            break;
        case 6:
            return "unsigned int";
            break;
        case 7:
            return "unsigned short";
            break;
        case 8:
            return "unsigned long";
            break;
        case 9:
            return "float";
            break;
        case 10:
            return "unsigned long long";
            break;
        default:
            cout << "simple types error" << endl;
    }
}

// 分析一个标签的内容，并
void ModelReader::analyze(vector<string> v, vector<ClassType>& classTypes, int& state) {
    if (state == WAIT_STATE) {
        if (v[0] == "<eClassifiers" && v[1] == "xsi:type=\"ecore:EClass\"") {
            // 0->1
            state = CLASS_STATE;
            ClassType* ct = new ClassType();
            // 对类定义进行分析
            // 分解name定义
            vector<string> map = splitString(v[2], "=");
            map[1] = splitString(map[1], "\"")[1];
            if (map[0] == "name") {
                ct->setName(map[1]);
            } else {
                // 这里规定每个类必须有name
                cout << "error1" << endl;
            }
            // 非抽象类可以省略abstract标签
            if (v.size() > 3) {
                // 分解对abstract定义
                map = splitString(v[3], "=");
                map[1] = splitString(map[1], "\"")[1];
                if (map[0] == "abstract") {
                    ct->setAbstract(map[0] == "true" ? true : false);
                } else if (map[0] == "eSuperTypes") {
                    // TODO:完成继承关系
                } else {
                    cout << "error2" << endl;
                }
            }
            // 装入
            classTypes.push_back(*ct);

        }
    } else if (state == CLASS_STATE) {
        if (v[0] == "<eStructuralFeatures") {
            if (v[1] == "xsi:type=\"ecore:EAttribute\"") {
                // 1->2
                state = ATTRIBUTE_STATE;
                // 分解name定义
                string name = "";
                vector<string> map = splitString(v[2], "=");
                map[1] = splitString(map[1], "\"")[1];
                if (map[0] == "name") {
                    // cout << map[1] << "*****" << endl;
                    name = map[1];
                } else {
                    cout << "error3" << endl;
                }

                // 分解type定义
                eSimpleType type;
                map = splitString(v[3], "=");
                map[1] = splitString(map[1], "\"")[1];
                if (map[0] == "eType") {
                    type = strToSimpleType(map[1]);
                } else {
                    cout << "error4" << endl;
                }

                // 分解默认值
                if (v.size() > 4) {
                    string value;
                    map = splitString(v[4], "=");
                    map[1] = splitString(map[1], "\"")[1];
                    if (map[0] == "defaultValueLiteral") {
                        value = map[1];
                    } else {
                        cout << "error5" << endl;
                    }
                    classTypes.back().addAttribute(name, type, value);
                } else {
                    classTypes.back().addAttribute(name, type);
                }
                // 2->1
                state = CLASS_STATE;

            }
        } else if (v[0] == "</eClassifiers>") {
            // 1->0
            state = WAIT_STATE;
        }
    } else {
        cout << "error6" << endl;
    }
}

// 将获得的classes打印输出查看
void ModelReader::printClasses(vector<ClassType> classTypes) {
    for (int i = 0; i < classTypes.size(); i++) {
        if (classTypes[i].getAbstract()) {
            cout << "Abstract Class " << classTypes[i].getName() << " {" << endl;
        } else {
            cout << "Class " << classTypes[i].getName() << " {" << endl;
        }
        vector<SimpleAttribute> simpleAttributes = classTypes[i].getAttributes();
        for (int j = 0; j != simpleAttributes.size(); j++) {
            if (simpleAttributes[j].value == "") {
                cout << "    " << SimpleTypeToStr(simpleAttributes[j].type) << " " << simpleAttributes[j].name << ";" << endl;
            } else {
                cout << "    " << SimpleTypeToStr(simpleAttributes[j].type) << " " << simpleAttributes[j].name << " = " << simpleAttributes[j].value << ";" << endl;
            }
        }
        cout << "};" << endl;
        cout << endl;
    }
}

// 读取并分析ecore文件
void ModelReader::Read(vector<ClassType>& classTypes) {
    ifstream fin("model/test.ecore");
    ofstream fout("model/test.txt");
    if (fin.fail() || fout.fail()) {
        cout << "file error!" << endl;
    }
    string s = "";
    string temp = "";
    int state = 0;
    while (getline(fin, temp)) {
        if (temp[temp.length() - 1] == '>') {
            // 一个标签结束
            s += temp;
            // 去除两侧空位
            s = chop(s);
            vector<string> v = splitStringNonSimple(s, " ");
            // for (auto iter = v.cbegin(); iter != v.cend(); iter++) {
            //     fout << (*iter) << endl;
            // }
            // fout << "***********************" << endl;
            analyze(v, classTypes, state);

            s = "";
        } else {
            // 标签未结束
            s += temp.substr(0, temp.length() - 1);
        }
    }
    // printClasses(classTypes);
}
