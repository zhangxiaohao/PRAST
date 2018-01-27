//
// Created by 张小豪 on 2018/1/21.
//

#include "Server.h"

extern ServerControl* ServerControl::instance;

///////////ServerDocument////////////////

//构造函数
ServerDocument::ServerDocument() {
    this->HT.clear();
    this->nodeList.clear();
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
        if(node->nodeId > op->insertNode) {
            nodeList.insert(node, GetNode(op->data, op->insertNode));
            node --;
            HT[op->insertNode] = node;
            break;
        }
    }
}

//执行一个删除操作
void ServerDocument::AddDeleteOperation(shared_ptr<Operation> op) {
    list<Node>::iterator node = this->HT[op->targetNode];
    (node->opList).push_back(op);
}

//执行一个插入操作
void ServerDocument::InsertNode(shared_ptr<Operation> op) {
    list<Node>::iterator node = this->HT[op->targetNode];
    rangescan(node, op);
}

///////////ServerControl////////////////

//构造函数
ServerControl::ServerControl() : Doc(new ServerDocument()){
}

//get Instance
ServerControl* ServerControl::GetInstance() {
    if(instance == NULL) {
        instance = new ServerControl();
    }
    return instance;
}

//和客户端进行一次操作同步 objPtr也是返回值
void ServerControl::SyncWithClient(shared_ptr<TransferObj> objPtr) {
    //找到需要返回的操作
    vector<shared_ptr<Operation>> rtnList;
    for(int i=(int)this->operationQueue.size() - 1; i > 0; i--) {
        if(this->operationQueue[i].second == objPtr->clientId) break;
        rtnList.push_back(this->operationQueue[i].first);
    }
    for(int i=0; i<objPtr->opList.size(); i++) {
        //服务器上执行操作
        this->Doc->Execute(objPtr->opList[i]);
        //push到本地临时队列
        (this->operationQueue).push_back(make_pair(objPtr->opList[i], objPtr->clientId));
    }
    objPtr->opList = rtnList;
    TransferQueue::GetInstance()->SetOperationSyncWithClientIdRtn(objPtr->clientId, objPtr);
}

//服务器主函数
void ServerControl::Work(int &done) {
    shared_ptr<TransferObj> objPtr;
    while(!done) {
        for(int i=0; i<CLIENTNUM; i++) {
            objPtr = TransferQueue::GetInstance()->GetOperationSyncWithClientId(i);
            SyncWithClient(objPtr);
            TransferQueue::GetInstance()->SetOperationSyncWithClientIdRtn(i, objPtr);
        }
    }
}
