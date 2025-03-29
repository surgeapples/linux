#include <iostream>
#include <signal.h>

void sighandler(int signum)
{
    std::cout << "Received signal " << signum << std::endl;
}

int main()
{
    //signal(SIGFPE, sighandler);
    //signal(SIGSEGV, sighandler);
    int a = 2;
    a /= 0;

    //int* p = nullptr;
    //*p = 10;
    while(true)
        ;

    return 0;
}