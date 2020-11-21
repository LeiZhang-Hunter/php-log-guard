//
// Created by zhanglei on 2020/11/14.
//

#include <thread>
#include "app/PHPError.h"

void App::PHPError::onReceive(const std::string& buffer) {
    std::smatch result;
    std::regex pattern(rule);	//匹配错误日志内容

    //拼接加入缓冲区
    errorBuffer.append(buffer);
    size_t errorBufferLen = errorBuffer.length();

    //迭代器声明
    std::string::const_iterator iterStart = errorBuffer.begin();
    std::string::const_iterator iterEnd = errorBuffer.end();
    std::string temp;
    std::string atelLog;

    while (regex_search(iterStart, iterEnd, result, pattern))
    {
        atelLog.clear();
        atelLog.append(util->formatPhpDateToJavaDate(result[1]));
        atelLog.append(" ");
        atelLog.append("php_errors");
        atelLog.append(" ");
        atelLog.append(result[2]);
        atelLog.append(" ");
        atelLog.append(result[3]);
        iterStart = result[0].second;	//更新搜索起始位置,搜索剩下的字符串
        continue;
    }
    std::cout << atelLog << std::endl;

    //查看结尾有多少个字符串没有匹配到
    size_t lastByte = result.position();

    if (lastByte > 0) {
        errorBuffer.assign(errorBuffer.substr(errorBufferLen - lastByte, lastByte));
    } else {
        //清空缓冲区
        errorBuffer.clear();
    }


}

void App::PHPError::onClose(const std::string &message) {
    if (!errorBuffer.empty()) {
        errorBuffer.clear();
    }
}