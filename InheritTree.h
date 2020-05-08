#ifndef INHERIT_TREE_H
#define INHERIT_TREE_H

#include <vector>
#include "ClassType.h"

using namespace std;

// 继承关系图的类节点
class ClassNode {
public:
    ClassNode();
    ~ClassNode();

    vector<ClassNode*> getParents() { return parents; }
    vector<ClassNode*> getChildren() { return children; }
    void setMark(bool _mark) { mark = _mark; }
    bool getMark() { return mark; }

    void addChildren(ClassNode* classNode);

private:
    ClassType* matchClass;
    vector<ClassNode*> parents;
    vector<ClassNode*> children;
    bool mark = false;
};

// 继承关系图
class InheritTree {
public:
    InheritTree();
    ~InheritTree();

    vector<ClassNode*> getRoots() { return roots; }
    void addRoots(ClassNode* classNode);
    bool check();
    void init(const vector<ClassType> classTypes);
    
private:
    vector<ClassNode*> roots;
};


#endif // INHERIT_TREE_H

