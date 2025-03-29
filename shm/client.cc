#include "shm.hpp"
#include "namedpipe.hpp"

int main()
{
    SHM shm(shm_path, proj_id, shm_user);
    char* pshm = (char*)shm.addr();
    FIFO fifo(fifo_path, User);
    fifo.open("w");
    int ch = 'a';
    while(ch <= 'z'){
        pshm[ch - 'a'] = ch;
        ch++;
        fifo.write("wakeup");
        sleep(2);
    }

    return 0;
}
