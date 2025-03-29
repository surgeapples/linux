#include "namedpipe.hpp"

int main()
{
    FIFO fifo(path, Creater);
    if(fifo.open("r")){
        while(true){
            std::string reader;
            ssize_t ret = fifo.read(reader);
            if(ret > 0){
                std::cout << "client says> " << reader << std::endl;
            }
            else if(ret == 0){
                std::cout << "client quit, server quit" << std::endl;
                break;
            }
            else{
                std::cerr << "error reading from pipe" << std::endl;
                break;
            }
        }
    }
    return 0;
}