#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "Client.h"
namespace NetHeartBeat {

static int server_offline()
{
    return 0;
}

static int server_online()
{
    return 0;
}

static void * client_thread(void* arg)
{
    Client *self = (Client *)arg;
    while(1)
    {
        self->exec();
    }
    return NULL;
}

Client::Client(string ip, int port, serve_offline_func offline_func, serve_online_func online_func)
{
    m_ip = ip;
    m_port = port;
    m_heartbeat_cnt = 0;
    m_server_offline =  offline_func;
    m_server_online = online_func;
    m_server_last_state = SERVER_NO_STATE;
    init_client();
}

Client::Client(serve_offline_func offline_func, serve_online_func online_func)
{
    m_ip = CONFIG_DEFAULT_LOCAL_IP;
    m_port = CONFIG_DEFAULT_PORT;
    m_heartbeat_cnt = 0;
    m_server_offline =  offline_func;
    m_server_online = online_func;
    m_server_last_state = SERVER_NO_STATE;
    init_client();
}

Client::Client()
{
    m_ip = CONFIG_DEFAULT_LOCAL_IP;
    m_port = CONFIG_DEFAULT_PORT;
    m_heartbeat_cnt = 0;
    m_server_offline =  server_offline;
    m_server_online = server_online;
    m_server_last_state = SERVER_NO_STATE;
    init_client();
}

Client::~Client()
{
    close(m_udp_fd);
}

int Client::exec()
{
    send_heart_beat();
    handle_heart_beat();
    return 0;
}

int Client::run()
{
    pthread_create(&m_pid, NULL, client_thread, this);
    pthread_detach(m_pid);
    return 0;
}

int Client::init_client()
{
    m_udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    int flags = fcntl(m_udp_fd, F_GETFL, 0);
    fcntl(m_udp_fd, F_SETFL, flags | O_NONBLOCK);
    memset(&m_server_addr, 0, sizeof(m_server_addr));

    m_server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, m_ip.c_str(), &m_server_addr.sin_addr.s_addr);
    m_server_addr.sin_port = htons(m_port);  //注意网络序转换
    return m_udp_fd;
}

int Client::handle_heart_beat(void)
{
    socklen_t len;
    struct sockaddr_in src;
    char buf[CONFIG_RCV_BUFF_LEN];
    memset(buf, 0, CONFIG_RCV_BUFF_LEN);
    recvfrom(m_udp_fd, buf, CONFIG_RCV_BUFF_LEN, 0, (struct sockaddr*)&src, &len);  //接收来自server的信息
    //printf("Client rcv:%s\n",buf);

    if(string(buf) == CONFIG_ACK_STR)
    {
        m_heartbeat_cnt = 0;
    }
    else
    {
        m_heartbeat_cnt++;
    }
    if(m_server_last_state == SERVER_NO_STATE)
    {
        if(m_heartbeat_cnt == 0)
        {
            m_server_last_state = SERVER_ONLINE_STATE;
            m_server_online();
        }
        if(m_heartbeat_cnt > 10)
        {
            m_server_last_state = SERVER_OFFLINE_STATE;
            m_server_offline();
        }
    }
    else if(m_server_last_state == SERVER_ONLINE_STATE)
    {
        if(m_heartbeat_cnt > 10)
        {
            m_server_offline();
            m_server_last_state = SERVER_OFFLINE_STATE;
        }

    }
    else if(m_server_last_state == SERVER_OFFLINE_STATE)
    {
        if(m_heartbeat_cnt == 0)
        {
            m_server_online();
            m_server_last_state = SERVER_ONLINE_STATE;
        }

    }
    sleep(1);  //一秒发送一次消息
    return 0;
}

int Client::send_heart_beat()
{
    string buf = CONFIG_HEART_BEAT_STR;
    return sendto(m_udp_fd, buf.c_str(), buf.length(), 0, (struct sockaddr*)&m_server_addr, sizeof(m_server_addr));
}

}


