#include <arpa/inet.h>
#include <unistd.h>
#include "Serve.h"
namespace NetHeartBeat {

static void *server_thread(void *arg)
{
    Serve *self = (Serve*)arg;
    while(1)
    {
        self->exec();
    }
    return NULL;
}

Serve::Serve()
{

    m_ip = CONFIG_DEFAULT_LOCAL_IP;
    m_port = CONFIG_DEFAULT_PORT;
    init_server();
}

Serve::Serve(string ip, int port)
{
    m_ip = ip;
    m_port = port;
    init_server();
}

Serve::~Serve()
{
    close(m_udp_fd);
}

int Serve::exec()
{
    handle_heart_beat();
    return 0;
}

int Serve::run()
{
    pthread_create(&m_pid, NULL, server_thread, this);
    pthread_detach(m_pid);
    return 0;
}

int Serve::handle_heart_beat(void)
{
    char buf[CONFIG_RCV_BUFF_LEN];  //接收缓冲区，1024字节
    socklen_t len;
    int count;
    struct sockaddr_in clent_addr;  //clent_addr用于记录发送方的地址信息

    memset(buf, 0, CONFIG_RCV_BUFF_LEN);
    len = sizeof(clent_addr);
    count = recvfrom(m_udp_fd, buf, CONFIG_RCV_BUFF_LEN, 0, (struct sockaddr*)&clent_addr, &len);  //recvfrom是拥塞函数，没有数据就一直拥塞
    if(count == -1)
    {
        printf("client udp recieve data fail!\n");
        return -1;
    }

    //printf("Server Rcv:%s\n",buf);  //打印client发过来的信息
    string rcv_str = string(buf);
    string ack = CONFIG_ACK_STR;
    if(rcv_str == CONFIG_HEART_BEAT_STR)
    {
       return sendto(m_udp_fd, (void*)ack.c_str(), ack.length(), 0, (struct sockaddr*)&clent_addr, len);
    }

    return 0;

}

int Serve::init_server()
{
    int ret = 0;
    struct sockaddr_in server_addr;
    m_udp_fd = socket(AF_INET, SOCK_DGRAM, 0); //AF_INET:IPV4;SOCK_DGRAM:UDP
    if(m_udp_fd < 0)
    {
        printf("udp server create error:%d\n", m_udp_fd);
        return m_udp_fd;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    //server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址，需要进行网络序转换，INADDR_ANY：本地地址
    inet_pton(AF_INET, m_ip.c_str(), &server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(m_port);  //端口号，需要网络序转换
    ret = bind(m_udp_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(ret < 0)
    {
        printf("udp server bind error:%d\n", ret);
        return ret;
    }
    return ret;
}

}

