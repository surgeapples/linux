#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void sig_handler(int signo)
{
    std::cout << "Received signal " << signo << std::endl;
    sleep(10);
}

int main()
{
    struct sigaction act;
    act.sa_handler = sig_handler;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, 3);
    act.sa_flags = 0;
    struct sigaction oldact;
    sigaction(SIGINT, &act, &oldact);
    while(true){
        std::cout << getpid() << std::endl;
        sleep(1);
    }
    return 0;
}
