#include "Processbar.h"
#include <unistd.h>

double filesize = 1.0 * 100 * 1024 * 1024;    
void download(double filesize, callback_t cb)
{
	double current = 0.0;
    double bandwidth = 1024 * 1024 * 1.0;
    
    printf("download begin, current: %lf\n", current);
	while(current <= filesize)
	{
        cb(filesize, current);
        //从网络中获取数据
        current += bandwidth;
        usleep(100000);
	}
    printf("\ndownload done, filesize: %lf\n", filesize);

}

int main()
{
    //ForTest();
    //ProcBar(100.0, 56.9);
    //ProcBar(100.0, 1.0);
    //ProcBar(100.0, 100.0);
    //ProcBar(100.0, 99.0);
    download(100*1024*1024, ProcBar);
    download(1*1024*1024, ProcBar);
    download(2*1024*1024, ProcBar);
    download(5*1024*1024, ProcBar);
    download(50*1024*1024, ProcBar);
    return 0;
}
