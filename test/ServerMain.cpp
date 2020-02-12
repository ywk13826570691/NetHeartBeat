#include "Serve.h"
int main(int argc, char **argv)
{
    NetHeartBeat::Serve sr;
    while(1)
    {
        sr.exec();
    }

    /*
    or using like this 
    sr.run();
    while(1);
    */
    return 0;
}
