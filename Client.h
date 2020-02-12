#ifndef CLIENT_H
#define CLIENT_H

#include<string>
#include<pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "Common.h"

namespace NetHeartBeat {
using namespace std;

typedef int(*serve_offline_func)();
typedef int(*serve_online_func)();

class Client
{
public:
    Client(string ip, int port, serve_offline_func offline_func, serve_online_func online_func);
    Client(serve_offline_func offline_func, serve_online_func online_func);
    ~Client();
    Client();
    int exec();//用户自己创建线程运行此函数
    int run();//创建线程并运行exec
private:
    int init_client();
    int handle_heart_beat(void);
    int send_heart_beat();
private:
    struct sockaddr_in m_server_addr;
    pthread_t m_pid;
    string m_ip;
    int m_port;
    int m_udp_fd;
    int m_heartbeat_cnt;
    serve_offline_func m_server_offline; // server offline function
    serve_online_func m_server_online; //server on line function

    typedef enum SERVER_STATE
    {
        SERVER_NO_STATE,
        SERVER_ONLINE_STATE,
        SERVER_OFFLINE_STATE
    }SERVER_STATE;
    int m_server_last_state;
};


}

#endif // CLIENT_H
