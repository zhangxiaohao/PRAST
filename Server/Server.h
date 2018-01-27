//
// Created by 张小豪 on 2018/1/21.
//

#ifndef AST_SERVER_H
#define AST_SERVER_H

#include "../Tool.h"
//临时队列
class OperationQueue {
public:
    OperationQueue();
};

//服务器的文本结构
class ServerDocument {
public:
    ServerDocument();

    ///记录tag
    //插入失败则代表已存在
    bool AddTag(Id pos);
    //删除失败则代表不存在
    bool DeleteTag(Id pos);
private:
    //AST基础结构
    map<Id, list<Node>::iterator> HT;
    list<Node> nodeList;

    //记录客户端部分副本范围
    map<int, pair<int, int>> ART;
    //记录tag
    set<Id> tagList;
    int clientNumber;
};


//负责服务器的逻辑控制
class ServerControl {
private:
    static ServerControl Instance;
    ServerControl();
public:
    static ServerControl GetInstance() {
        return Instance;
    }

};

#endif //AST_SERVER_H
