//
// Created by zhanglei on 2020/11/20.
//
#include <iostream>
#include <sstream>
#include "os/UnixUtil.h"

int OS::UnixUtil::strToTime(const std::string& timeString) {
    std::stringstream stream(timeString);
    std::string headTimeString;
    std::string tailTimeString;
    stream >> headTimeString;
    stream >> tailTimeString;
    std::stringstream Date1(headTimeString);
    std::stringstream Date2(tailTimeString);
    std::string stringDate;
    struct tm unixTime;

    int headerCount = 0;

    while (std::getline(Date1, stringDate, '-')) {
        //日
        if (headerCount == 0) {
            unixTime.tm_mday = atoi(stringDate.c_str());
        }

        //月
        if (headerCount == 1) {
            unixTime.tm_mon = getMonth(stringDate);
        }

        //年
        if (headerCount == 2) {
            unixTime.tm_year = atoi(stringDate.c_str());
        }

        headerCount++;
    }

    int tailCount = 0;
    while (std::getline(Date2, stringDate, ':')) {
        //时
        if (tailCount == 0) {
            unixTime.tm_hour = atoi(stringDate.c_str());
        }

        //分
        if (tailCount == 1) {
            unixTime.tm_min = atoi(stringDate.c_str());
        }

        //秒
        if (tailCount == 2) {
            unixTime.tm_sec = atoi(stringDate.c_str());
        }
        tailCount++;
    }

    unixTime.tm_year -= 1900;
    unixTime.tm_mon--;

    time_t res = mktime(&unixTime);
    return res;
}


int OS::UnixUtil::getMonth(const std::string& month) {
    if (month == "Jan") {
        return 1;
    }

    if (month == "Feb") {
        return 2;
    }

    if (month == "Mar") {
        return 3;
    }

    if (month == "Apr") {
        return 4;
    }

    if (month == "Jun") {
        return 5;
    }

    if (month == "Jul") {
        return 6;
    }

    if (month == "May") {
        return 7;
    }

    if (month == "Aug") {
        return 8;
    }

    if (month == "Sept") {
        return 9;
    }

    if (month == "Oct") {
        return 10;
    }

    if (month == "Nov") {
        return 11;
    }

    if (month == "Dec") {
        return 12;
    }

    return 0;
}

std::string OS::UnixUtil::formatPhpDateToJavaDate(const std::string& timeString) {
    std::stringstream stream(timeString);
    std::string headTimeString;
    std::string tailTimeString;
    std::string stringDate;
    struct tm unixTime;
    char buf[64] = {0};

    //格式化出要分割的字符串
    stream >> headTimeString;
    stream >> tailTimeString;

    int headerCount = 0;

    //要格式化的时间缓冲区
    std::stringstream Date1(headTimeString);
    std::stringstream Date2(tailTimeString);

    while (std::getline(Date1, stringDate, '-')) {
        //日
        if (headerCount == 0) {
            unixTime.tm_mday = atoi(stringDate.c_str());
        }

        //月
        if (headerCount == 1) {
            unixTime.tm_mon = getMonth(stringDate);
        }

        //年
        if (headerCount == 2) {
            unixTime.tm_year = atoi(stringDate.c_str());
        }

        headerCount++;
    }

    int tailCount = 0;
    while (std::getline(Date2, stringDate, ':')) {
        //时
        if (tailCount == 0) {
            unixTime.tm_hour = atoi(stringDate.c_str());
        }

        //分
        if (tailCount == 1) {
            unixTime.tm_min = atoi(stringDate.c_str());
        }

        //秒
        if (tailCount == 2) {
            unixTime.tm_sec = atoi(stringDate.c_str());
        }
        tailCount++;
    }

    snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d",
             unixTime.tm_year, unixTime.tm_mon, unixTime.tm_mday,
             unixTime.tm_hour, unixTime.tm_min, unixTime.tm_sec);
    return buf;
}