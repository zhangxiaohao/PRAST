//
// Created by 张小豪 on 2018/1/27.
//

#include "TransferQueue.h"

extern TransferQueue* TransferQueue::instance;

//构造函数
TransferQueue::TransferQueue() {
    this->requestQueue.clear();
    this->responseQueue.clear();
}

//获取单例句柄
TransferQueue* TransferQueue::GetInstance() {
    if(instance == nullptr) {
        instance = new TransferQueue();
    }
    return instance;
}

//客户端发起操作同步
bool TransferQueue::SetOperationSyncWithClientId(int clientId, shared_ptr<TransferObj> transferObj) {
    std::lock_guard<std::mutex> lck(this->requestMtx);
    //服务器还没有取走上一次结果
    if(this->requestQueue.count(clientId)) return false;
    this->requestQueue[clientId] = transferObj;
    return true;
}

//服务器返回客户端操作同步的结果
bool TransferQueue::SetOperationSyncWithClientIdRtn(int clientId, shared_ptr<TransferObj> transferObj) {
    std::lock_guard<std::mutex> lck(this->responseMtx);
    //客户端还没有取走上一次的结果
    if(this -> responseQueue.count(clientId)) return false;
    this->responseQueue[clientId] = transferObj;
    return true;
}

//客户端取走服务器返回值
shared_ptr<TransferObj> TransferQueue::GetOperationSyncWithClientIdRtn(int clientId) {
    std::lock_guard<std::mutex> lck(this->responseMtx);
    return this->responseQueue[clientId];
}

//服务器取走客户端请求
shared_ptr<TransferObj> TransferQueue::GetOperationSyncWithClientId(int clientId) {
    std::lock_guard<std::mutex> lck(this->requestMtx);
    return this->requestQueue[clientId];
}