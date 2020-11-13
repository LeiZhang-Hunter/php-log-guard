//
// Created by zhanglei on 2020/11/14.
//

#ifndef PHPLOGGUARD_FILEEVENT_H
#define PHPLOGGUARD_FILEEVENT_H
namespace App {
    class FileEvent {
    public:
        void onModify();

        void onDelete();

        void onCreate();

        void onAttrChange();

        void onMove();
    };
}
#endif //PHPLOGGUARD_FILEEVENT_H
