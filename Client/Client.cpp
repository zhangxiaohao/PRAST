//
// Created by 张小豪 on 2018/1/14.
//

#include "Client.h"

Document::Document() {
    this->Init();
}

//在文档结构上执行一个操作
bool Document::Execute(shared_ptr<Operation> op) {
    if(op->type == 0) { //插入操作
        this->InsertNode(op);
    } else if(op->type == 1) { //删除操作
        this->AddDeleteOperation(op);
    }
}

//清空所有数据结构为部分副本同步做准备
void Document::Init() {
    this->HT.clear();
    this->nodeList.clear();
    this->documentLength = 0;
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
    if(node->opList.size() == 2) {
        this->documentLength --;
    }
}

//执行一个插入操作
void Document::InsertNode(shared_ptr<Operation> op) {
    list<Node>::iterator node = this -> HT[op -> targetNode];
    rangescan(node, op);
    this->documentLength ++;
}

//获取当前文档的长度
int Document::GetDocumentLength() {
    return this->documentLength;
}

//根据位置获取id
Id Document::GetIdByPos(int pos) {
    list<Node>::iterator it = this->nodeList.begin();
    for(int i=0; i<pos; i++, ++it) {}
    return it->nodeId;
}

////////////////////////客户端控制类////////////////////////////
//构造函数
ClientControl::ClientControl(int clientId) {
    this->clientId = clientId;
    this->Doc = make_shared(new Document());
    while(!this->operationQueue.empty()) {
        operationQueue.pop();
    }
}

//操作同步
int ClientControl::OperationSync() {
    std::lock_guard<std::mutex> lck(this->DocMtx);

}

//副本同步
int ClientControl::ReplicaSync() {
    std::lock_guard<std::mutex> lck(this->DocMtx);

    //清空所有结构
    this->Doc->Init();
}

//执行一个本地随机操作
bool ClientControl::ExecuteAnOpertation() {
    std::lock_guard<std::mutex> lck(this->DocMtx);
    shared_ptr<Operation> operationPtr;
    int pos;
    if(rand()%10 > 7 || this->Doc->GetDocumentLength() == 0) { //插入操作的比例 或者文档中没有可以删除的节点 产生插入操作
        pos = rand() % (this->Doc->GetDocumentLength() + 1);
        operationPtr = make_shared(GetOperation(0, "" + ('a'+rand()%26), this->Doc->GetIdByPos(pos)));
    } else {
        pos = rand() % this->Doc->GetDocumentLength();
        operationPtr = make_shared(GetOperation(1, "", this->Doc->GetIdByPos(pos)));
    }
    (this->operationQueue).push(operationPtr);
    return this->Doc->Execute(operationPtr);
}

