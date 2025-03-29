#include <iostream>
#include <signal.h>

void sighandler(int signum)
{
    std::cout << "Received signal " << signum << std::endl;
}

int main()
{
    signal(2, sighandler);
    while(true)
    {
        raise(SIGINT);
    }
}