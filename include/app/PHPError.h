//
// Created by zhanglei on 2020/11/14.
//

#ifndef PHPLOGGUARD_PHPERROR_H
#define PHPLOGGUARD_PHPERROR_H

#include <string>
#include <regex>
#include <memory>
#include <iostream>
#include <regex>

#include "Noncopyable.h"
#include "os/UnixUtil.h"
#include "app/AtelFormat.h"

namespace App {



    //日志的错误级别
    enum {
        PHP_ERROR = 1,
        PHP_FPM_ERROR = 2,
        PHP_FPM_SLOW = 3
    };

    class PHPError : Noncopyable, std::enable_shared_from_this<PHPError> {
    public:
        PHPError() : util(new OS::UnixUtil()) {

        }

        /**
         * 收到函数的时候触发的函数地址
         */
        void onReceive(const std::string &);

        /**
         * 解析php_error_log
         */
        void parsePHPLog(std::string& atelLog, const std::smatch& result);

        /**
         * 解析php-fpm的log
         * @param atelLog
         * @param result
         */
        void parsePHPFPMLog(std::string& atelLog, const std::smatch& result);

        /**
         * 设置正则表达式规则
         * @param regexRule
         */
        void setRegEx(const std::string &regexRule) {
            rule = regexRule;
        }

        /**
         * 转换为Atel的错误等级
         * @param errorLevel
         * @return
         */
        std::string covertAtelLevel(const std::string &errorLevel);

        /**
         * 设置输出文件的路径
         * @param path
         */
        void setOutPath(const std::string &path) {
            outPath = path;
        }

        /**
         * 文件被关闭的时候触发的函数
         */
        void onClose(const std::string &message);

        /**
         * 设置监听日志的错误级别
         * @param logType
         */
        void setPHPLogType(int logType) {
            type = logType;
        }

        /**
         * 缓冲区
         */
        std::string errorBuffer;

        /**
         * 正则表达式规则
         */
        std::string rule;

        /**
         * 通用工具
         */
        std::unique_ptr<OS::UnixUtil> util;

        /**
         * 缓冲区大小
         */
        size_t bufferSize;

        /**
         * 输出路径
         */
        std::string outPath;

    private:
        int type;
    };
}
#endif //PHPLOGGUARD_PHPERROR_H
