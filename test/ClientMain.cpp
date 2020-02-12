#include "Client.h"
int offline()
{
    printf("offline\n");
    return 0;
}

int online()
{
    printf("online\n");
    return 0;
}

int main(int argc, char **argv)
{
    NetHeartBeat::Client cl(offline, online);
    while(1)
    {
        cl.exec();
    }

    /*
    or using like this 
    cl.run();
    while(1);
    */
}
