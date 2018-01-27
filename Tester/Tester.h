//
// Created by 张小豪 on 2018/1/20.
//

#ifndef AST_TESTER_H
#define AST_TESTER_H

#include "../Client/Client.h"

/*
 * 负责生成随机操作
 * 负责设置部分副本大小
 * 和客户端是一对一的关系
 */
class Tester {
    shared_ptr<ClientControl> myClient;

public:
    Tester(ClientControl myClient);

    void GeneratorOperation(int number);

};

#endif //AST_TESTER_H
