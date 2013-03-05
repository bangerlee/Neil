Neil
====

Neil是一个轻量的服务器框架。

代码解析
========

src/net/thread.h   
src/net/thread.cpp   

线程基类，提供起/停线程的接口，子类需提供run函数，run函数中完成具体线程事务。

src/net/epoll_server.h   
src/net/epoll_server.cpp   

epoll_server类，负责消息接收与发送

src/net/lock.h   
src/net/lock.c   

基于pthread_mutex_t和计数器实现的读写锁

src/net/queue.h   
src/net/queue.c   

由读写锁保护的消息队列

src/process_map.h   
src/process_map.cpp   

根据与客户端协议好的消息ID调用相应的注册函数进行处理

src/process.h   
src/process.cpp   

定义消息ID和相应的消息处理函数

消息流程
========

客户端-->|接收线程-->消息接收队列-->消息处理-->消息发送队列-->发送线程|-->客户端   


