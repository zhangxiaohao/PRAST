//
// Created by 张小豪 on 2018/1/14.
//

#ifndef AST_CLIENT_H
#define AST_CLIENT_H

#include "../Tool.h"

//客户端负责文档的结构和算法
class Document {
public:
    Document();
    void AddDeleteOperation(shared_ptr<Operation> op);
    void InsertNode(shared_ptr<Operation> op);

private:
    map<Id, list<Node>::iterator> HT;
    list<Node> nodeList;

    void rangescan(list<Node>::iterator node, shared_ptr<Operation> op);
};

//负责客户端的流程控制
class ClientControl {
private:
    Document Doc;
    static ClientControl Instance;
    ClientControl();
    ClientControl(const ClientControl &ins);

public:
    //获取单例类
    static ClientControl GetInstance() {
        return Instance;
    }
    int OperationSync();
    int ReplicaSync();
};

#endif //AST_CLIENT_H
