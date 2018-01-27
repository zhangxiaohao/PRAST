//
// Created by 张小豪 on 2018/1/21.
//

#include "Server.h"

///////////ServerDocument////////////////

//构造函数
ServerDocument::ServerDocument() {
    this->clientNumber = 0;
    this->HT.clear();
    this->nodeList.clear();
    Node mock = GetNode("", make_pair(-1, -1));
    this->nodeList.push_back(mock);
    this->nodeList.push_back(mock);
}

//插入一个标记
bool ServerDocument::AddTag(Id pos) {
    if(tagList.count(pos)) return false;
    tagList.insert(pos);
    return true;
}

//删除一个标记
bool ServerDocument::DeleteTag(Id pos) {
    if(!tagList.count(pos)) return false;
    tagList.erase(pos);
    return true;
}

//在文档结构上执行一个操作
bool ServerDocument::Execute(shared_ptr<Operation> op) {
    if(op->type == 0) { //插入操作
        InsertNode(op);
    } else if(op->type == 1) { //删除操作
        AddDeleteOperation(op);
    } else return false;
    return true;
}

void ServerDocument::rangescan(list<Node>::iterator node, shared_ptr<Operation> op) {
    for( ; node != this->nodeList.end(); node ++) {
        if(node -> nodeId > op -> insertNode) {
            nodeList.insert(node, GetNode(op -> data, op -> insertNode));
        }
    }
}

//执行一个删除操作
void ServerDocument::AddDeleteOperation(shared_ptr<Operation> op) {
    list<Node>::iterator node = this -> HT[op -> targetNode];
    (node -> opList).push_back(*op);
}

//执行一个插入操作
void ServerDocument::InsertNode(shared_ptr<Operation> op) {
    list<Node>::iterator node = this -> HT[op -> targetNode];
    rangescan(node, op);
}

///////////ServerControl////////////////
