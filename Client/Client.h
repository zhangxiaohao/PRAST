//
// Created by 张小豪 on 2018/1/14.
//

#ifndef AST_CLIENT_H
#define AST_CLIENT_H

#include "../Tool.h"
#include "../TransferQueue/TransferQueue.h"

//客户端负责文档的结构和算法
class Document {
public:
    Document();

    //在文档结构是上执行一个操作
    bool Execute(shared_ptr<Operation> op);
    //清空文档为部分副本同步做准备
    void Init();
    //获取当前文档的长度
    int GetDocumentLength();
    //获取pos位置的Id
    Id GetIdByPos(int pos);
    //获取当前本地操作数
    int GetOperationCount();
    //output
    void Output();

private:

    ///AST基本函数
    void rangescan(list<Node>::iterator node, shared_ptr<Operation> op);
    void AddDeleteOperation(shared_ptr<Operation> op);
    void InsertNode(shared_ptr<Operation> op);

    map<Id, list<Node>::iterator> HT;
    list<Node> nodeList;
    int documentLength;
    //操作计数器 用来生成id
    int operationCount;
};

//负责客户端的流程控制
//设计上一个客户端有两个线程，一个负责和服务器交互，另一个负责产生本地操作，所以这个类必须是线程安全的
class ClientControl {
private:
    //在生成本地操作的时候需要上锁
    std::mutex DocMtx;
    //客户端的编号
    int clientId;

    shared_ptr<Document> Doc;
    //临时存储本地还没和服务器同步的操作
    queue<shared_ptr<Operation>> operationQueue;

public:
    //服务器交互函数 线程1
    int OperationSync();
    int ReplicaSync();

    ClientControl(int clientId);
    ClientControl(const ClientControl &ins);
    //执行一个随机本地操作 线程2
    int ExecuteAnOpertation();
};

#endif //AST_CLIENT_H
