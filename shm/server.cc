#include "shm.hpp"
#include "namedpipe.hpp"

int main()
{
    SHM shm(shm_path, proj_id, shm_creater);

    char* pshm = (char*)shm.addr();
    FIFO fifo(fifo_path, Creater);
    fifo.open("r");
    shm.debug(); 
    while(pshm[25] != 'z'){
        std::string tmp;
        fifo.read(tmp);
        std::cout << "shm content:" << pshm << std::endl;
    }
    shm.debug();
    

}
