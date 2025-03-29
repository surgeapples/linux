#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>
#include<cerrno>
#include<cstring>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
using std::cout;
using std::cerr;
using std::vector;
using std::string;


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
        int count = 5;
        string message = "process1 here, pid: ";
        message += std::to_string(getpid());
        while(count){
            write(pipefd[1], message.c_str(), message.size());
            count--;
            sleep(1);
        }
        write(pipefd[1], "quit", strlen("quit"));
        close(pipefd[0]);       
        exit(0);
    }

    rids.push_back(fork());
    
    if(rids.back() == 0){
        //关闭  writefd
        close(pipefd[1]);
        char reader[1024] = {'\0'};
        while(true){
            ssize_t s = read(pipefd[0], reader, sizeof(reader) - 1);                    
            reader[s] = '\0';
            cout << "process2 get from process1: " << reader << '\n';
            if(strcmp("quit", reader) == 0)
                break;
        }
        exit(0);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    //3.等待子进程
    for(auto& rid : rids){
        waitpid(rid, NULL, 0);
    }

    return 0;
}
