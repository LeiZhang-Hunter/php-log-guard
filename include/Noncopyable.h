//
// Created by zhanglei on 2020/11/11.
//

#ifndef PHPLOGGUARD_NONCOPYABLE_H
#define PHPLOGGUARD_NONCOPYABLE_H
class Noncopyable
{
public:
    Noncopyable(const Noncopyable&) = delete;
    void operator=(const Noncopyable&) = delete;

protected:
    Noncopyable() = default;
    ~Noncopyable() = default;
};
#endif //PHPLOGGUARD_NONCOPYABLE_H
