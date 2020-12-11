//
// Created by zhanglei on 2020/11/16.
//

#include "app/AtelFormat.h"

App::AtelFormat::AtelFormat() {

}

void App::AtelFormat::append(std::string& buffer) {
    if (buffer.empty()) {
        return;
    }
    //加入缓冲区
    atelBuffer.push_back(std::move(buffer));
}

void App::AtelFormat::flush() {
    if (atelBuffer.empty()) {
        return;
    }

    //刷新日志内容到缓冲区
    std::ofstream fileOut;
    fileOut.open(outPath, std::ios::app|std::ios::out);
    std::vector<std::string>::iterator atelIterator;
    if (!fileOut.is_open())
    {
        return;
    }

    for (atelIterator = atelBuffer.begin(); atelIterator != atelBuffer.end(); ++atelIterator)
    {
        fileOut<<(*atelIterator);
    }
    fileOut.close();
}

App::AtelFormat::~AtelFormat() {
    atelBuffer.clear();
}