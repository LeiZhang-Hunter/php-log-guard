//
// Created by zhanglei on 2020/11/14.
//

#ifndef PHPLOGGUARD_FILEEVENT_H
#define PHPLOGGUARD_FILEEVENT_H

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
#include <memory>
#include <vector>

namespace OS {
    class UnixInodeWatcher;
}
namespace App {


    class FileEvent {
    public:
        FileEvent(const std::string& path, std::shared_ptr<OS::UnixInodeWatcher>& _watcher);

        /**
         * 修改文件时候触发的函数
         */
        void onModify();

        /**
         * 删除文件的时候创建文件的函数
         */
        void onDelete();

        /**
         * 创建文件的时候触发的函数
         */
        void onCreate();

        /**
         * 属性变更的时候触发的函数
         */
        void onAttrChange();

        /**
         * 移动的时候触发的函数
         */
        void onMove();

        /**
         * access的时候触发的函数
         */
        void onAccess();

        void setOnReceive() {

        }

        ~FileEvent() {
            closeFile();
        }

    private:
        /**
         * 获取文件大小，用来计算offset
         * @return
         */
        bool flushOffset();
        /**
         * 打开文件描述符
         * @return
         */
        bool openFile();
        /**
         * 关闭文件描述符
         * @return
         */
        bool closeFile();
        //存储路径
        std::string filePath;
        //监控的文件id
        int monitorFileFd;
        //偏移量
        size_t offset;

        /**
         * 观察者
         */
        std::shared_ptr<OS::UnixInodeWatcher> watcher;
    };
}
#endif //PHPLOGGUARD_FILEEVENT_H
