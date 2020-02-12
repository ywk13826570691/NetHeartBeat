#ifndef SERVE_H
#define SERVE_H
#include <string>
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "Common.h"

namespace NetHeartBeat {
using namespace std;
class Serve
{
public:
    Serve(string ip, int port);
    Serve();
    ~Serve();
    int exec();//用户自己创建线程运行此函数
    int run();//创建线程并运行exec
private:
    int init_server();
    int handle_heart_beat(void);
private:
    pthread_t m_pid;
    string m_ip;
    int m_port;
    int m_udp_fd;
};

}

#endif // SERVE_H
