#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef void (*task_t)();
#define TASK_NUM 3
std::array<task_t, TASK_NUM> tasks;

void download_task()
{
    std::cout << "Downloading..." << std::endl;
}

void upload_task()
{
    std::cout << "Uploading..." << std::endl;
}

void tar_task()
{
    std::cout << "Archiving..." << std::endl;
}

void load()
{
    srand(time(nullptr) ^ getpid() ^ 177777);
    tasks[0] = download_task;
    tasks[1] = upload_task;
    tasks[2] = tar_task;
}

void execute(int task_num)
{
    if(0 <= task_num && task_num < TASK_NUM){
        tasks[task_num]();
    }
}

void run()
{
    while(true){
        int task_num = 0;
        ssize_t ret = read(0, &task_num, sizeof(task_num));
        if(ret == sizeof(int)){
            std::cout << "subprocess_" << getpid() << " run task: ";
            execute(task_num);
        }
        else if(ret == 0){
            break;
        }
    }
}