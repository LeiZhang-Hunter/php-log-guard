//
// Created by zhanglei on 2020/11/14.
//

#include "app/PHPError.h"

void App::PHPError::onReceive(const std::string &buffer) {
    std::smatch result;
    std::regex pattern("\\[(.*?)\\]\\s(.*?):\\s\\s(.*?)\n");	//匹配错误日志内容
    size_t errorBufferLen = errorBuffer.length();
    size_t bufferLen = buffer.length();

    //拼接加入缓冲区
    errorBuffer.append(buffer);

    //迭代器声明
    std::string::const_iterator iterStart = errorBuffer.begin();
    std::string::const_iterator iterEnd = errorBuffer.end();
    std::string temp;
    while (regex_search(iterStart, iterEnd, result, pattern))
    {
        temp = result[1];
        std::cout << temp << std::endl;
        std::cout << result[2] << std::endl;
        std::cout << result[3] << std::endl;

        iterStart = result[0].second;	//更新搜索起始位置,搜索剩下的字符串

        continue;
    }

    //查看结尾有多少个字符串没有匹配到
    size_t lastByte = result.position();
    if (lastByte > 0) {
        if (bufferLen + lastByte <= BUFSIZ * 10) {
            errorBuffer.append(errorBuffer.substr(bufferLen - lastByte, lastByte));
        } else {
            //缓冲区占用内存太多，放弃缓冲区内容
            errorBuffer.clear();
        }
    }
}

void App::PHPError::onClose(const std::string &message) {
    if (!errorBuffer.empty()) {
        errorBuffer.clear();
    }
}