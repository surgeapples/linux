#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define DEFAULT_FD -1
#define Creater 0
#define User 1
#define SIZE 4096

const std::string fifo_path = "./myfifo";
class FIFO
{
    public:
        FIFO(const std::string& path, int whoami)
            : m_path(path), m_whoami(whoami), m_fd(DEFAULT_FD)
        {
            if(m_whoami == Creater){
                int ret = mkfifo(m_path.c_str(), 0666);
                if(ret == 0)
                    std::cout << "FIFO created successfully." << std::endl;
                else
                    std::cerr << "Failed to create FIFO." << std::endl;
            }
        }

        ~FIFO()
        {
            if(m_whoami == Creater){
                ssize_t ret = unlink(m_path.c_str());
                if(ret == 0)
                    std::cout << "FIFO removed successfully." << std::endl;
                else
                    std::cerr << "Failed to remove FIFO." << std::endl;
            }
            if(m_fd != DEFAULT_FD)
                close(m_fd);
        }
    public:
        bool open(const std::string& mode)
        {
            if(mode == "r"){
                m_fd = ::open(m_path.c_str(), O_RDONLY);
                if(m_fd > 0)
                    return true;
                else
                    return false;
            }
            else if(mode == "w"){
                m_fd = ::open(m_path.c_str(), O_WRONLY);
                if(m_fd > 0)
                    return true;
                else
                    return false;
            }
        }

        ssize_t read(std::string& reader)
        {
            char buffer[SIZE] = {0};
            ssize_t ret = ::read(m_fd, buffer, SIZE - 1);
            if(ret){
                buffer[ret] = '\0';
                reader = buffer;
            }
            return ret;
        }
        ssize_t write(const std::string& writer)
        {
            return ::write(m_fd, writer.c_str(), writer.size());
        }
    private:
       int m_whoami;
       int m_fd;
       const std::string m_path; 
        
};