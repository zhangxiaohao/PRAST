//
// Created by 张小豪 on 2018/1/21.
//

#include "Server.h"

///////////ServerDocument////////////////

ServerDocument::ServerDocument() {
    this->clientNumber = 0;
    this->HT.clear();
    this->nodeList.clear();
    Node mock = GetNode("", make_pair(-1, -1));
    this->nodeList.push_back(mock);
    this->nodeList.push_back(mock);
}

bool ServerDocument::AddTag(Id pos) {
    if(tagList.count(pos)) return false;
    tagList.insert(pos);
    return true;
}

bool ServerDocument::DeleteTag(Id pos) {
    if(!tagList.count(pos)) return false;
    tagList.erase(pos);
    return true;
}

///////////ServerControl////////////////
