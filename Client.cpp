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

void Document::rangescan() {

}

//执行一个删除操作
void Document::AddDeleteOperation(Operation op) {

}

//执行一个插入操作
void Document::InsertNode(Operation op) {

}

////////////////////////客户端控制类////////////////////////////

//获取单例类
static ClientControl ClientControl::GetInstance() {
    return ClientControl::Instance;
}

//操作同步
int ClientControl::OperationSync() {

}

//副本同步
int ClientControl::ReplicaSync() {

}
