//
// Created by zhanglei on 2020/11/16.
//

#ifndef PHPLOGGUARD_ATELFORMAT_H
#define PHPLOGGUARD_ATELFORMAT_H

#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "Noncopyable.h"

namespace App {
    class AtelFormat : Noncopyable{
    public:
        AtelFormat();
        void setMaxBuffer(ssize_t size) {
            bufferSize = size;
        }

        /**
         * 追加
         * @param buffer
         */
        void append(std::string& buffer);

        void setOutPut(const std::string& path) {
            outPath = path;
        }

        /**
         * 刷新缓冲区
         */
        void flush();

        ~AtelFormat();

    private:
        ssize_t bufferSize;

        //缓冲区
        std::vector<std::string> atelBuffer;

        //路径
        std::string outPath;
    };
}

#endif //PHPLOGGUARD_ATELFORMAT_H
