#include<iostream>
#include<vector>
#include<cstdlib>
#include<cerrno>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
using std::cout;
using std::cerr;
using std::vector;


int main()
{
    //1.创建匿名管道文件
    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret == -1){
        cerr << "create pipe fail!" << '\n';
        cerr << "errno: " << errno << '\n';
    }

    //2.创建两个子进程
    vector<int> rids;
    rids.push_back(fork());

    if(rids.back() == 0){
        //关闭 readfd
        close(pipefd[0]);       
        dup2(pipefd[1], 1);
        close(pipefd[1]);
        execlp("head", "head", "-10", "/home/captainCpp/linux/Linux/pipe/Anonymous_pipe/pipe.cpp", NULL);
        cerr << "execlp fail" << '\n';
        exit(1);
    }

    rids.push_back(fork());
    
    if(rids.back() == 0){
        //关闭  writefd
        close(pipefd[1]);
        dup2(pipefd[0], 0);
        close(pipefd[0]);
        execlp("tail", "tail", "-5", NULL);
        cerr << "execlp fail" << '\n';
        exit(1);
    }

    //父不关闭, tail进程会卡住
    close(pipefd[0]);
    close(pipefd[1]);
    //3.等待子进程
    for(auto& rid : rids){
        waitpid(rid, NULL, 0);
    }

    return 0;
}
