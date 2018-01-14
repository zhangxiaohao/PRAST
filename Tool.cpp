//
// Created by 张小豪 on 2018/1/14.
//
#include "Tool.h"

//id 相关成员函数
Id GetId(int _s, int _ucn) {
    Id ret = make_pair(_s, _ucn);
    return ret;
}

//Operation相关成员函数
Operation GetOperation(int _type, string _data, Id _targetNode) {
    Operation ret;
    ret.type = _type;
    ret.data = _data;
    ret.targetNode = _targetNode;
    return ret;
}

//Node相关成员函数
Node GetNode(string _str, Id _nodeId) {
    Node ret;
    ret.str = _str;
    ret.nodeId = _nodeId;
    return ret;
}
