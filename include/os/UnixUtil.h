//
// Created by zhanglei on 2020/11/20.
//

#ifndef PHPLOGGUARD_UNIXUTIL_H
#define PHPLOGGUARD_UNIXUTIL_H

#include <string>

namespace OS {
    class UnixUtil {
    public:
        UnixUtil() {};

        /**
         * 将php的时间转换为标准的时间值
         * @param timeString
         * @return
         */
        int strToTime(const std::string& timeString);

        /**
         * 获取月份
         * @param month
         * @return
         */
        int getMonth(const std::string& month);

        /**
         * 将php格式的日期时间转换成java格式的日期时间
         * @param time
         * @return
         */
        std::string formatPhpDateToJavaDate(const std::string& timeString);
    };
}

#endif //PHPLOGGUARD_UNIXUTIL_H
