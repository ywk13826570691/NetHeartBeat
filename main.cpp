#include <iostream>
#include "Serve.h"
#include "Client.h"

using namespace std;
int offline()
{
    printf("server offline\n");
    return 0;
}

int online()
{
    printf("server online\n");
    return 0;
}

int main()
{

    NetHeartBeat::Serve serve;
    serve.run();
    NetHeartBeat::Client cl(offline, online);
    cl.run();
    cout << "Hello World!" << endl;
    while(1);
    return 0;
}

