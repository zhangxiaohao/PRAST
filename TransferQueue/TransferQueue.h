//
// Created by 张小豪 on 2018/1/27.
//

#ifndef AST_TRANSFERQUEUE_H
#define AST_TRANSFERQUEUE_H

#include "../Tool.h"

//传输类 操作同步时 服务器不直接和客户端交互 而是通过传输队列间接交互
//线程安全 类
class TransferQueue {
public:

    //客户端发起一次操作同步
    bool SetOperationSyncWithClientId(int clientId, shared_ptr<TransferObj> transferObj);
    //服务器返回一次操作同步的结果
    bool SetOperationSyncWithClientIdRtn(int clientId, shared_ptr<TransferObj> transferObj);
    //客户端取走服务器返回值
    shared_ptr<TransferObj> GetOperationSyncWithClientIdRtn(int clientId);
    //服务器取走客户端请求
    shared_ptr<TransferObj> GetOperationSyncWithClientId(int clientId);

    //获取单例句柄
    static shared_ptr<TransferQueue> GetInstance() {
        if(instance == nullptr) {
            instance = new TransferQueue();
        }
        return instance;
    }

private:
    TransferQueue();
    TransferQueue(const TransferQueue &ob);
    //mutex
    std::mutex requestMtx;
    std::mutex responseMtx;

    //单例句柄
    static shared_ptr<TransferQueue> instance;
    //clientId, request
    map<int, shared_ptr<TransferObj>> requestQueue;
    //clientId, response
    map<int, shared_ptr<TransferObj>> responseQueue;
};

#endif //AST_TRANSFERQUEUE_H

