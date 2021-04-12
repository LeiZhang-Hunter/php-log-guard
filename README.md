# 虎扑php错误日志收集计划

## 描述

虎扑c++ 高性能php错误日志转换器，用来把php的错误日志，转换到java的Atel 日志收集器中

## github地址:

```
https://github.com/LeiZhang-Hunter/php-log-guard
```

## 安装

```
build.sh
```

## 命令解析

```
    -c 配置文件位置
    -e 启动为start,停止位stop
```

## 启动事例

启动
```
./php-atel-agent -c ../config/config.ini -e start
```

停止

```
./php-atel-agent -c ../config/config.ini -e stop
```