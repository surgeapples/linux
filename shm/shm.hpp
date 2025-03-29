#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

const int shm_creater = 0;
const int shm_user = 1;
const std::string shm_path = "./";
const int proj_id = 0x66;
const int shm_size = 4096;

class SHM
{
    private:
        key_t get_key(const std::string& path, int proj_id)
        {
            key_t k = ftok(path.c_str(), proj_id);
            if(k == -1){
                perror("ftok");
                exit(1);
            }
            return k;
        }
    public:
        SHM(const std::string& path, int proj_id, int whoami)
            : m_whoami(whoami), m_shmaddr(nullptr)
        {
            m_key = get_key(path, proj_id);
            if(m_whoami == shm_creater){
                m_shmid = shmget(m_key, shm_size, IPC_CREAT | IPC_EXCL | 0666);
            }
            else if(m_whoami == shm_user){
               m_shmid = shmget(m_key, shm_size, IPC_CREAT | 0666); 
            }
            if(m_shmid == -1){
                perror("shmget");
                exit(1);
            }
            m_shmaddr = shmat(m_shmid, nullptr, 0);
            if(m_shmaddr == (void*)-1){
                perror("shmat");
                exit(1);
            }
        }

        ~SHM()
        {
            if(m_shmaddr != nullptr){
                int ret = shmdt(m_shmaddr);
                if(ret == -1){
                    perror("shmdt");
                    exit(1);
                }
            }
            if(m_whoami == shm_creater){
                int ret = shmctl(m_shmid, IPC_RMID, nullptr);
                if(ret == -1){
                    perror("shmctl");
                    exit(1);
                }
            }
        }

        void* addr()
        {
            return m_shmaddr;
        }

        void debug()
        {
            shmid_ds shm_info;
            int ret = shmctl(m_shmid, IPC_STAT, &shm_info);
            if(ret == -1){
                perror("shmctl");
                exit(1);
            }   
            std::cout << "shm_info.shm_perm.__key = " << shm_info.shm_perm.__key << std::endl;
            std::cout << "shm_info.shm_nattch =" << shm_info.shm_nattch << std::endl;
        }
    private:
        key_t m_key;
        int m_shmid;
        int m_whoami;
        void* m_shmaddr;
};
