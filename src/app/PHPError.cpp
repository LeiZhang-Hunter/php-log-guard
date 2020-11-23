//
// Created by zhanglei on 2020/11/14.
//

#include <thread>
#include "app/PHPError.h"

void App::PHPError::onReceive(const std::string &buffer) {
    std::smatch result;
    std::regex pattern(rule);	//匹配错误日志内容
    size_t bufferLen = buffer.length();
    //拼接加入缓冲区
    errorBuffer.append(buffer);
    size_t errorBufferLen = errorBuffer.length();

    //迭代器声明
    std::string::const_iterator iterStart = errorBuffer.begin();
    std::string::const_iterator iterEnd = errorBuffer.end();
    std::string temp;

    //atel格式化处理程序
    std::unique_ptr<App::AtelFormat> atel(new App::AtelFormat);

    atel->setOutPut(outPath);

    while (regex_search(iterStart, iterEnd, result, pattern))
    {
        std::string atelLog;
        atelLog.append(util->formatPhpDateToJavaDate(result[1]));
        atelLog.append(" ");
        atelLog.append("php_errors");
        atelLog.append(" ");
        atelLog.append(covertAtelLevel(result[2]));
        atelLog.append(" ");
        atelLog.append(result[2]);
        atelLog.append(":");
        atelLog.append(result[3]);
        atelLog.append("\n");
        atel->append(atelLog);
        iterStart = result[0].second;	//更新搜索起始位置,搜索剩下的字符串
        continue;
    }

    //刷新到缓冲区
    atel->flush();

    //查看结尾有多少个字符串没有匹配到
    size_t lastByte = result.position();

    if (lastByte > 0) {

        if (bufferLen + lastByte <= BUFSIZ * 10) {
            errorBuffer.assign(errorBuffer.substr(errorBufferLen - lastByte, lastByte));
        } else {
            //缓冲区占用内存太多，放弃缓冲区内容
            errorBuffer.clear();
        }
    } else {
        //清空缓冲区
        errorBuffer.clear();
    }


}

std::string App::PHPError::covertAtelLevel(const std::string& errorLevel) {
    if (errorLevel == "PHP Fatal error" || errorLevel == "PHP Recoverable fatal error" || errorLevel == "PHP Parse error") {
        return "ERROR";
    } else {
        return "WARN";
    }
}

void App::PHPError::onClose(const std::string &message) {
    if (!errorBuffer.empty()) {
        errorBuffer.clear();
    }
}