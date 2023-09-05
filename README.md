## 依赖环境
### tinyxml
```
wget https://udomain.dl.sourceforge.net/project/tinyxml/tinyxml/2.6.2/tinyxml_2_6_2.zip
unzip tinyxml_2_6_2.zip

cd tinyxml
vim Makefile
修改以下内容：
- 将OUTPUT := xmltest 改为OUTPUT := libtinyxml.a
- 将${LD} -o $@ ${LDFLAGS} ${OBJS} ${LIBS} ${EXTRA_LIBS}修改为：${AR} $@ ${LDFLAGS} ${OBJS} ${LIBS} ${EXTRA_LIBS}

make
sudo make install
```
接着将生成的`tinystr.h`、`tinyxml.h`拷贝到`/usr/include/tinyxml`下，将`libtinyxml.a`拷贝到`/usr/lib/`下。

### protobuf
```
sudo apt-get install autoconf automake libtool curl make g++ unzip -y
wget  https://github.com/protocolbuffers/protobuf/releases/download/v3.19.4/protobuf-cpp-3.19.4.tar.gz
tar -xzvf protobuf-cpp-3.19.4.tar.gz

cd protobuf-cpp-3.19.4
./configure -prefix=/usr/local

make -j4 
make check
sudo make install
sudo ldconfig  // 更新动态链接库

protoc --version // 验证是否安装成功
```

## 日志模块开发文档
日志模块：
```
1.日志级别
2.打印到文件，支持日期命名，以及日志的滚动
3.c 格式化风格
4.线程安全
```

LogLevel：
```
Debug
Info
Error
```

LogEvent：
```
文件名-行号
MsgNo
进程号
线程号
日期-时间，精确到ms
自定义

格式：
[LogLevel]\t[%y-%m-%d %H:%M:%s.%ms]\t[pid:thread_id]\t[file_name:line][%msg]
```

Logger:
```
1.提供打印接口
2.设置日志输出路径
```

## EventLoop开发
EventLoop模块:
```
while(1){

    // 从已经有事件触发的
    while(!m_tasks.empty()){
        task = m_tasks.front();
        m_tasks.pop();
        if(task != nullptr){
            task();
        }
    }

    int rt = epoll_wait(epfd, fds, ..., time_out);
    if(rt < 0){
        // epoll wait调用失败
    }
    else{
        // 将有事件触发的句柄添加到任务队列中
        foreach(fd : fds){
            task.push(fd);
        }
    }
}
```

其中所有文件句柄通过`FdEvent`管理:
```
1. m_fd  // 文件句柄
2. m_listen_event  // 监听的epoll 事件
3. handle return callback function  // 返回还事件的回调函数
4. trigger epoll event: EPOLLIN, EPOLLOUT  // 事件的类型，刻度或可写
5. return fd or event  // 返回文件句柄或epoll 事件
```

Reactor？？？？


## TimerEvent定时任务
```
1. 指定任务发生的时间点 arrive_time
2. interval，单位ms
3. is_repeated  // 是否重复性发生
4. is_cancled  // 是否已经关闭该任务
5. task  // 要执行的任务


cancle()
cancleRepeated()
```

通过 Timer 管理，Timer 继承 FdEvent, 将TimerEvent也注册到 epoll 里，使得时间事件能够及时执行，避免因 epoll wait 阻塞而没有及时发现时间任务:
```
multimap<arrivetime, TimerEvent>  // Timer Event 集合

addTimerEvent()  // 添加定时任务
deleteTimerEvent()  // 删除定时任务
onTimer()  // 发生了 I/O事件后，需要执行的方法

reserArriveTime()
```