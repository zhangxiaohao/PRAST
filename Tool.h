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
//cpp 11 lib
#include <thread>
//c lib
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;
typedef pair<int, int> Id;
//Id.first (s)本地分配计数器 | Id.second (ucn)全局唯一站点标识符

struct Operation {
    int type; //0 插入 1 删除
    string data;	//操作附加数据
    Id targetNode;	//操作目标节点
};

struct Node {
    string str;	//当前节点包含的字符串
    Id nodeId;	//当前节点的唯一标识符
    vector<Operation> opList;  //当前节点包含的操作列表
};

//id相关成员函数
Id GetId(int _s, int _ucn);

//Operation相关成员函数
Operation GetOperation(int _type, string _data, Id _targetNode);

//Node相关成员函数
Node GetNode(string _str, Id _nodeId);

#endif //AST_TOOL_H
