#include "namedpipe.hpp"

int main()
{
    FIFO fifo(path, User);
    if(fifo.open("w")){
        while(true){
            std::string writer;
            std::cout << "Enter> ";
            std::getline(std::cin, writer);
            fifo.write(writer);
        }

    }
    return 0;
}