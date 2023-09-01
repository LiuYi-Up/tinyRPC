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