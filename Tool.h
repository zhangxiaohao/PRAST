//
// Created by 张小豪 on 2018/1/14.
//

#ifndef AST_TOOL_H
#define AST_TOOL_H

//cpp lib
#include <ctime>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <climits>
//cpp 11 lib
#include <thread>
#include <mutex>
#include <memory>
//c lib
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>

using namespace std;
typedef pair<int, int> Id;
//Id.first (s)本地分配计数器 | Id.second (ucn)全局唯一站点标识符

//const value
const int CLIENTNUM = 1;
const int INSERTNUM = 0;

class Operation {
public:
    int type; //0 插入 1 删除
    Id targetNode;	//操作目标节点
    string data;	//操作附加数据
    Id insertNode;  //插入节点

    void Output();
};

class Node {
public:
    string str;	//当前节点包含的字符串
    Id nodeId;	//当前节点的唯一标识符
    vector<shared_ptr<Operation>> opList;  //当前节点包含的操作列表
};

//一次操作同步的传输单元
class TransferObj {
public:
    int clientId;   //发起操作同步的客户端id
    vector<shared_ptr<Operation>> opList;   //操作列表
};

//id相关成员函数
Id GetId(int _s, int _ucn);

//Operation相关成员函数
Operation GetOperation(int _type, string _data, Id _targetNode, Id _insertNode);

//Node相关成员函数
Node GetNode(string _str, Id _nodeId);
Node GetNode(string _str, Id _nodeId, shared_ptr<Operation> op);

#endif //AST_TOOL_H
