//
// Created by zhanglei on 2020/11/11.
//

#ifndef PHPLOGGUARD_CHANNEL_H
#define PHPLOGGUARD_CHANNEL_H

#include <sys/epoll.h>
#include <cstdint>
#include <memory>
#include <functional>


namespace Event {
    typedef std::function<void ()> EventCallable;

    class EventLoop;

    class Channel {
    public:
        Channel(EventLoop* loop_,int fd) :loop(loop_),channelFd(fd), events(0) {

        }

        bool setOnReadCallable(const EventCallable& callable) {
            eventOnRead = callable;
            return true;
        }

        bool setOnCloseCallable(const EventCallable& callable) {
            eventOnClose = callable;
            return true;
        }

        bool setOnErrorCallable(const EventCallable& callable) {
            eventOnError = callable;
            return true;
        }

        bool setOnWriteCallable(const EventCallable& callable) {
            eventOnWrite = callable;
            return true;
        }

        bool setEvents(uint32_t _events) {
            return events = _events;
        }

        uint32_t getEvents() {
            return events;
        }

        int getChannelFd() {
            return channelFd;
        }

        bool enableReading() {
            events |= EPOLLIN;
            update();
            return true;
        }

        bool disableReading() {
            events &= ~EPOLLIN;
            update();
            return true;
        }

        bool enableWriting() {
            events |= EPOLLOUT;
            update();
            return true;
        }

        bool disableWriting() {
            events &= ~EPOLLOUT;
            update();
            return true;
        }

        void update();

    private:
        int channelFd;
        EventLoop* loop;
        uint32_t events;
        EventCallable eventOnRead;
        EventCallable eventOnClose;
        EventCallable eventOnWrite;
        EventCallable eventOnError;
        std::weak_ptr<Event::Channel> tie;
    };
}


#endif //PHPLOGGUARD_CHANNEL_H
