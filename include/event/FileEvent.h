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
#include <functional>

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

        /**
         * 设置文件发生变动时候通知的API
         */
        void setOnReceiveApi(const std::function<void(const std::string&)>& _modifyFunc) {
            modifyFunc = _modifyFunc;
        }

        /**
         * 设置文件被删除，移动，改变属性时候触发的API，注意删除和移动只有在监控级别是文件的时候才会是OnAttr
         * 其余情况都是OnDelete和OnMove
         */
        void setOnCloseApi(const std::function<void(const std::string&)>& _closeFunc) {
            closeFunc = _closeFunc;
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

        /**
         * 修改的时候触发的函数
         */
        std::function<void(const std::string&)> modifyFunc;

        /**
         * 关闭时候触发的函数
         */
        std::function<void(const std::string&)> closeFunc;
    };
}
#endif //PHPLOGGUARD_FILEEVENT_H
