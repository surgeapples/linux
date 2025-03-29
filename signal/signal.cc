#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

void sighandler(int signum)
{
    std::cout << "Received signal " << signum << std::endl;
}

int main()
{
    signal(2, sighandler); 
    while(true)
    {
        std::cout << getpid() << std::endl;
        sleep(1);
    }
    return 0;
}