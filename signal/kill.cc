#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    if(argc != 3){
        exit(1);
    } 
    kill(atoi(argv[2]), atoi(argv[1] + 1));   

    return 0;
}
