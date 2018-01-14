//
// Created by 张小豪 on 2018/1/14.
//

#ifndef AST_CLIENT_H
#define AST_CLIENT_H

#include "Tool.h"

//客户端负责文档的结构和算法
class Document {
public:
    Document();
    void AddDeleteOperation(Operation op);
    void InsertNode(Operation op);

private:
    map<Id, list<Node>::iterator> HT;
    list<Node> nodeList;

    void rangescan();
};

//负责客户端的流程控制
class ClientControl {
private:
    Document Doc;
    static ClientControl Instance;
    ClientControl() = default;
    ClientControl(const ClientControl &ins);

public:
    static ClientControl GetInstance();
    int OperationSync();
    int ReplicaSync();
};

#endif //AST_CLIENT_H
