#include "InheritTree.h"

using namespace std;

ClassNode::ClassNode() {}
ClassNode::~ClassNode() {}

// 添加Children
void ClassNode::addChildren(ClassNode* classNode) {
    if (!classNode) {
        children.push_back(classNode);
        classNode->parents.push_back(this);
    } else {
        cout << "error: addChildren" << endl;
    }
}

InheritTree::InheritTree() {}
InheritTree::~InheritTree() {}

// 添加Root
void InheritTree::addRoots(ClassNode* classNode) {
    if (classNode) {
        roots.push_back(classNode);
    } else {
        cout << "error: addRoots" << endl;
    }
}

// 队列节点定义
typedef struct SeqNode {
    ClassNode* classNode;
    SeqNode* next;
};

// 队列定义
typedef struct Sequence {
    SeqNode* head;
    SeqNode* tail;
};

// 创建队列
Sequence* CreateSequence(ClassNode* root) {
    // 新建一个队列节点
    SeqNode* seqNode = new SeqNode();
    seqNode->classNode = root;
    // 新建一个空节点,这里为了方便后续判断，一直在尾部维持一个空节点
    SeqNode* nullNode = new SeqNode();
    nullNode->classNode = NULL;
    nullNode->next = NULL;

    seqNode->next = nullNode;
    
    // 新建一个队列
    Sequence* sequence = new Sequence();
    sequence->head = seqNode;
    sequence->tail = nullNode;

    return sequence;
}

// 判断队列是否为空
bool isEmpty(Sequence* sequence) {
    if (sequence) {
        if (sequence->head == sequence->tail) {
            return true;
        } else {
            return false;
        }
    } else {
        cout << "error: sequence is NULL" << endl;
        return true;
    }
}

// 队列尾部添加节点
void pushSeqNode(Sequence* sequence, ClassNode* _classNode) {
    // 数据加入到尾部
    sequence->tail->classNode = _classNode;
    // 在尾部新加空节点
    SeqNode* nullNode = new SeqNode();
    nullNode->classNode = NULL;
    nullNode->next = NULL;
    
    sequence->tail->next = nullNode;
    sequence->tail = nullNode;
}

// 队列头部取出节点
ClassNode* popSeqNode(Sequence* sequence) {
    // 得到头
    ClassNode* classNode = sequence->head->classNode;
    SeqNode* temp = sequence->head;
    sequence->head = sequence->head->next;
    delete temp;

    return classNode;
}


// 释放队列
void emptySequence(Sequence* sequence) {
    while(!isEmpty(sequence)) {
        popSeqNode(sequence);
    }
    delete sequence->head;
    delete sequence;
}


// 检查继承关系的规范性
bool InheritTree::check() {
    // 对每个根分别进行检查
    for (int i = 0; i < roots.size(); i++) {
        // 获取队列
        Sequence* sequence = CreateSequence(roots[i]);
        // 广度优先遍历标记
        while(!isEmpty(sequence)) {
            ClassNode* classNode = popSeqNode(sequence);
            // 标记访问过
            classNode->setMark(true);
            
            for (int j = 0; j < classNode->getChildren().size(); j++) {
                if (classNode->getChildren()[j]->getMark()) {
                    // 遇到已经标记的情况，则发现环,直接释放队列，返回
                    emptySequence(sequence);
                    return false;
                } else {
                    // 新节点，加入队列
                    pushSeqNode(sequence, classNode->getChildren()[j]);
                }
            }
        }
        // 释放队列
        emptySequence(sequence);

        Sequence* sequence = CreateSequence(roots[i]);
        // 广度优先恢复mark
        while(!isEmpty(sequence)) {
            ClassNode* classNode = popSeqNode(sequence);
            // 恢复
            classNode->setMark(false);
            
            for (int j = 0; j < classNode->getChildren().size(); j++) {
                if (!classNode->getChildren()[j]->getMark()) {
                    continue;
                } else {
                    // 新节点，加入队列
                    pushSeqNode(sequence, classNode->getChildren()[j]);
                }
            }
        }
        // 释放队列
        emptySequence(sequence);
    }
    return true;
}


void InheritTree::init(const vector<ClassType> classTypes) {

}
