#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

void print_pending(sigset_t& pending_set)
{
    for(int i = 1; i < 32; i++){
        int ret = sigismember(&pending_set, i);
        if(ret == 1){
            std::cout << "1";
        }
        else{
            std::cout << "0"; 
        }
    }
    std::cout << std::endl;
}

int main()
{
    signal(SIGINT, SIG_IGN);
    sigset_t block_set, old_block_set;
    sigemptyset(&block_set);
    sigemptyset(&old_block_set);

    sigaddset(&block_set, SIGINT);
    sigprocmask(SIG_BLOCK, &block_set, &old_block_set);
    kill(getpid(), SIGINT);

    int cnt = 10;

    while(true){
        sigset_t pending_set;
        sigpending(&pending_set);
        print_pending(pending_set);
        sleep(1);
        if(cnt == 0){
            sigprocmask(SIG_UNBLOCK, &block_set, &old_block_set);
        }
        cnt--;
    }


    return 0;
}