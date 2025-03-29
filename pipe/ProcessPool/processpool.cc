#include "task.hpp"
using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;

class Channel
{
    public:
        Channel(int wfd, int subid, const string& name)
        : m_wfd(wfd), m_subid(subid), m_name(name)
        {}
        int getWfd() const { return m_wfd; }
        pid_t getSubid() const { return m_subid; }
        const string& getName() const { return m_name; }
    public:
        void closedown()const{ close(m_wfd); }
        void clean() const{
            pid_t pid = waitpid(m_subid, NULL, 0);
            if(pid){
                cout << "wait " << pid << " success" << endl; 
            }
        } 
    private:
        int m_wfd;
        pid_t m_subid;
        string m_name;
};

void create_channel(vector<Channel>& channels, int num, task_t task)
{
    for(int i = 0; i < num; i++){
        int pipefd[2];
        int ret = pipe(pipefd);
        if(ret < 0){
            cerr << "create pipe error" << endl;
            exit(1);
        }
        pid_t pid = fork();
        if(pid == 0){
            //关闭上一个进程的写端,不必担心这是第0号channel
            for(auto channel : channels)
                channel.closedown();
            close(pipefd[1]);
            dup2(pipefd[0], 0); 
            close(pipefd[0]);
            task(); 
            exit(0);
        }
        close(pipefd[0]);
        channels.emplace_back(pipefd[1], pid, "channel-" + std::to_string(i));
    }
}

int next(const vector<Channel>& channels)
{
    static int i = 0;
    int channeli = i;
    i++;
    i %= channels.size();
    return channeli;
}

void assign_one_task(const vector<Channel>& channels)
{
    sleep(1);
    int task_num = rand() % TASK_NUM;
    int channel_num = next(channels);
    cout << "assign task " << task_num << " to channel-" << channel_num << 
    " subprocess_" <<  channels[channel_num].getSubid() << endl; 
    write(channels[channel_num].getWfd(), (char*)&task_num, sizeof(int));
}

void assign_tasks(const vector<Channel> & channels, int times = -1)
{
    if(times){
        while(times--)
            assign_one_task(channels);
    }
    else{
        while(true)
            assign_one_task(channels);
        
    }

}

void cleanUp_channel(const vector<Channel>& channels)
{
    for(auto channel : channels){
        channel.closedown();
        channel.clean(); 
    }
}

int main(int argc, char* argv[])
{
    if(argc != 2){
        cerr << "Usage: " << "./processpool <pipe_number>" << endl;
        exit(1);
    }
    load();
    vector<Channel> channels;
    create_channel(channels, atoi(argv[1]), run);
    assign_tasks(channels, 5);
    cleanUp_channel(channels);
    return 0;
}