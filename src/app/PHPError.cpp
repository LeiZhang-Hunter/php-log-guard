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

    //设置atel的输出路劲
    atel->setOutPut(outPath);

    //执行正则匹配内容
    while (regex_search(iterStart, iterEnd, result, pattern))
    {
        std::string atelLog;
        switch (type) {
            //解析php的错误日志
            case PHP_ERROR:
                parsePHPLog(atelLog, result);
                break;

            //解析fpm的错误日志
            case PHP_FPM_ERROR:
                parsePHPFPMLog(atelLog, result);
                break;

            //解析慢日志
        }
        std::cout << atelLog << std::endl;

        atel->append(atelLog);
        iterStart = result[0].second;	//更新搜索起始位置,搜索剩下的字符串
        continue;
    }

    //刷新到目标缓冲区
    atel->flush();

    //查看结尾有多少个字符串没有匹配到
    size_t lastByte = result.position();

    //没有残包
    if (lastByte <= 0) {
        //清空缓冲区
        errorBuffer.clear();
        return;
    }

    if (bufferLen + lastByte <= BUFSIZ * 10) {
        errorBuffer.assign(errorBuffer.substr(errorBufferLen - lastByte, lastByte));
    } else {
        //缓冲区占用内存太多，放弃缓冲区内容
        errorBuffer.clear();
    }
}

void App::PHPError::parsePHPFPMLog(std::string& atelLog, const std::smatch& result) {
    //时间
    atelLog.append(util->formatPhpDateToJavaDate(result[1]));
    atelLog.append(" ");
    //线程名字
    atelLog.append("php_fpm_log");
    atelLog.append(" ");
    //日志级别
    atelLog.append(covertAtelLevel(result[2]));
    //内容
    atelLog.append(" ");
    atelLog.append(result[3]);
    atelLog.append("\n");
}

void App::PHPError::parsePHPLog(std::string& atelLog, const std::smatch& result) {
    atelLog.append(util->formatPhpDateToJavaDate(result[1]));
    atelLog.append(" ");
    //线程名字
    atelLog.append("php_errors");
    atelLog.append(" ");
    //日志级别
    atelLog.append(covertAtelLevel(result[2]));
    atelLog.append(" ");
    //内容
    atelLog.append(result[2]);
    atelLog.append(":");
    atelLog.append(result[3]);
    atelLog.append("\n");
}

std::string App::PHPError::covertAtelLevel(const std::string& errorLevel) {

    switch (type) {
        //php错误日志的格式
        case PHP_ERROR:
            if (errorLevel == "PHP Fatal error" || errorLevel == "PHP Recoverable fatal error" || errorLevel == "PHP Parse error") {
                return "ERROR";
            } else {
                return "WARN";
            }
            break;

        //php-fpm的日志解析
        //错误级别在php内核源码中只有 ZLOG_DEBUG  ZLOG_NOTICE ZLOG_WARNING ZLOG_ERROR ZLOG_ALERT
        case PHP_FPM_ERROR:
            if (errorLevel == "ALERT" || errorLevel == "ERROR") {
                return "ERROR";
            } else {
                return "WARN";
            }
            break;

        //php-slow日志的解析
        case PHP_FPM_SLOW:
            break;
    }

    return "WARN";
}

void App::PHPError::onClose(const std::string &message) {
    if (!errorBuffer.empty()) {
        errorBuffer.clear();
    }
}