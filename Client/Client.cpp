//
// Created by 张小豪 on 2018/1/14.
//

#include "Client.h"

Document::Document() {
    this->Init();
    this->operationCount = 0;
    Node head = GetNode("", make_pair(INT_MIN, INT_MIN));
    Node tail = GetNode("", make_pair(INT_MAX, INT_MAX));
    //创建头尾节点
    nodeList.push_back(head);
    list<Node>::iterator it = nodeList.end(); --it;
    HT[make_pair(INT_MIN, INT_MIN)] = it;
    nodeList.push_back(tail);
    it = nodeList.end(); --it;
    HT[make_pair(INT_MAX, INT_MAX)] = it;
}

//在文档结构上执行一个操作
bool Document::Execute(shared_ptr<Operation> op) {
    if(op->type == 0) { //插入操作
        this->InsertNode(op);
    } else if(op->type == 1) { //删除操作
        this->AddDeleteOperation(op);
    } else return false;
    return true;
}

//清空所有数据结构为部分副本同步做准备
void Document::Init() {
    this->HT.clear();
    this->nodeList.clear();
    this->documentLength = 0;
}

void Document::rangescan(list<Node>::iterator node, shared_ptr<Operation> op) {
    for( ; node != this->nodeList.end(); node ++) {
        if(node -> nodeId > op -> insertNode) {
            nodeList.insert(node, GetNode(op->data, op->insertNode, op));
            node --;
            if(node->nodeId != op->insertNode) cout << "ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1" << endl;
            cout << "size: " << node->opList.size() << endl;
            HT[op->insertNode] = node;
            break;
        }
    }
}

//执行一个删除操作
void Document::AddDeleteOperation(shared_ptr<Operation> op) {
    list<Node>::iterator node = this -> HT[op -> targetNode];
    (node -> opList).push_back(op);
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
    for(int i=0; i<pos; ++it) {
        if(it->opList.size() <= 1) i ++;
        if(i == pos) break;
    }
    return it->nodeId;
}

//获取本地操作数，每次递增
int Document::GetOperationCount() {
    return this->operationCount ++;
}

void Document::Output() {
    list<Node>::iterator it = this->nodeList.begin();
    cout << "------------------DOC---------------" << endl;
    for(int i=0; it != this->nodeList.end(); ++it) {
        cout << "Id: " << it->nodeId.first << ',' << it->nodeId.second;
        cout << " size: " << it->opList.size() << endl;
    }
    cout << "------------------------------------" << endl;
}

////////////////////////客户端控制类////////////////////////////
//构造函数
ClientControl::ClientControl(int clientId) : Doc(new Document()){
    this->clientId = clientId;
    while(!this->operationQueue.empty()) {
        operationQueue.pop();
    }
}

//操作同步
int ClientControl::OperationSync() {
    std::lock_guard<std::mutex> lck(this->DocMtx);
    shared_ptr<TransferObj> objPtr(new TransferObj());
    objPtr->clientId = this->clientId;
    while(!this->operationQueue.empty()) {
        objPtr->opList.push_back(this->operationQueue.front());
        this->operationQueue.pop();
    }
    return TransferQueue::GetInstance()->SetOperationSyncWithClientId(this->clientId, objPtr);
}

//副本同步
int ClientControl::ReplicaSync() {
    std::lock_guard<std::mutex> lck(this->DocMtx);
    //清空所有结构
    this->Doc->Init();
    return 0;
}

//执行一个本地随机操作
bool ClientControl::ExecuteAnOpertation() {
    std::lock_guard<std::mutex> lck(this->DocMtx);
    shared_ptr<Operation> operationPtr;
    int pos;
    if(rand()%10 > INSERTNUM || this->Doc->GetDocumentLength() == 0) { //插入操作的比例 或者文档中没有可以删除的节点 产生插入操作
        pos = rand() % (this->Doc->GetDocumentLength() + 1);
        string c = "a";
        c[0] += rand()%26;
        operationPtr = make_shared<Operation>(GetOperation(0, c, this->Doc->GetIdByPos(pos), make_pair(this->Doc->GetOperationCount(), clientId)));
    } else {
        pos = (rand() % this->Doc->GetDocumentLength()) + 2;
        operationPtr = std::make_shared<Operation>(GetOperation(1, "", this->Doc->GetIdByPos(pos), make_pair(-1, -1)));
    }
    this->Doc->Output();
    cout << "Doc len: " << this->Doc->GetDocumentLength() << " | pos: " << pos << endl;
    operationPtr->Output();
    (this->operationQueue).push(operationPtr);
    return this->Doc->Execute(operationPtr);
}

