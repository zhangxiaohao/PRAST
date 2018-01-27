//
// Created by 张小豪 on 2018/1/14.
//
#include "Tool.h"

//Operation相关成员函数
Operation GetOperation(int _type, string _data, Id _targetNode, Id _insertNode) {
    Operation ret;
    ret.type = _type;
    ret.data = _data;
    ret.targetNode = _targetNode;
    ret.insertNode = _insertNode;
    return ret;
}

void Operation::Output() {
    cout << "type: ";
    if(this->type == 0) cout << "ins" << endl;
    else if(this->type == 1) cout << "del" << endl;
    cout << "data: " << this->data << endl;
    cout << "target node: " << this->targetNode.first << ' ' << this->targetNode.second << endl;
    cout << "insert node: " << this->insertNode.first << ' ' << this->insertNode.second << endl;
}

//Node相关成员函数
Node GetNode(string _str, Id _nodeId) {
    Node ret;
    ret.str = _str;
    ret.nodeId = _nodeId;
    return ret;
}

Node GetNode(string _str, Id _nodeId, shared_ptr<Operation> op) {
    Node ret;
    ret.str = _str;
    ret.nodeId = _nodeId;
    ret.opList.push_back(op);
    return ret;
}
