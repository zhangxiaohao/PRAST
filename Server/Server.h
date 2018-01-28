//
// Created by 张小豪 on 2018/1/21.
//

#ifndef AST_SERVER_H
#define AST_SERVER_H

#include <atomic>
#include "../Tool.h"
#include "../TransferQueue/TransferQueue.h"

//服务器的文本结构
class ServerDocument {
public:
    ServerDocument();

    ///记录tag
    //插入失败则代表已存在
    bool AddTag(Id pos);
    //删除失败则代表不存在
    bool DeleteTag(Id pos);

    ///在文档上执行一个操作
    ///返回操作执行是否成功
    bool Execute(shared_ptr<Operation> op);

private:

    ///AST基本函数
    void AddDeleteOperation(shared_ptr<Operation> op);
    void InsertNode(shared_ptr<Operation> op);
    void rangescan(list<Node>::iterator node, shared_ptr<Operation> op);

    //AST基础结构
    map<Id, list<Node>::iterator> HT;
    list<Node> nodeList;

    //记录客户端部分副本范围
    map<int, pair<int, int>> ART;
    //记录tag
    set<Id> tagList;

};


//负责服务器的逻辑控制
class ServerControl {
private:
    shared_ptr<ServerDocument> Doc;
    //临时队列
    vector<pair<shared_ptr<Operation>, int>> operationQueue;

    ServerControl();

    //处理一次操作同步请求
    void SyncWithClient(shared_ptr<TransferObj> objPtr);

public:
    static ServerControl* instance;
    static ServerControl* GetInstance();

    //服务器工作函数
    void Work();

};

#endif //AST_SERVER_H
