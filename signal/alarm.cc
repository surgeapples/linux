#include <iostream>
#include <unistd.h>
#include <signal.h>

void sighandler(int signum)
{
    std::cout << "Received signal " << signum << std::endl;
    exit(1);
}

int main()
{
    alarm(5);
    sleep(1);
    int remain = alarm(2);
    std::cout << "remain: " << remain << std::endl;
    signal(SIGALRM,sighandler);
    while(true){
        //std::cout << "Looping..." << std::endl;
    }
}