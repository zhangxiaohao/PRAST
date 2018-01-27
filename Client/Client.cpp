//
// Created by 张小豪 on 2018/1/14.
//

#include "Client.h"

Document::Document() {
    HT.clear();
    Node mock = GetNode("", make_pair(-1, -1));
    //创建头尾节点
    nodeList.push_back(mock);
    nodeList.push_back(mock);
}

void Document::rangescan(list<Node>::iterator node, shared_ptr<Operation> op) {
    for( ; node != this->nodeList.end(); node ++) {
        if(node -> nodeId > op -> insertNode) {
            nodeList.insert(node, GetNode(op -> data, op -> insertNode));
        }
    }
}

//执行一个删除操作
void Document::AddDeleteOperation(shared_ptr<Operation> op) {
    list<Node>::iterator node = this -> HT[op -> targetNode];
    (node -> opList).push_back(*op);
}

//执行一个插入操作
void Document::InsertNode(shared_ptr<Operation> op) {
    list<Node>::iterator node = this -> HT[op -> targetNode];
    rangescan(node, op);
}

////////////////////////客户端控制类////////////////////////////
//构造函数
ClientControl::ClientControl() {

}

//操作同步
int ClientControl::OperationSync() {

}

//副本同步
int ClientControl::ReplicaSync() {

}


