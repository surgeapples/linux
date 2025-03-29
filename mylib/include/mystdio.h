#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>

#define SIZE 1024

struct myFILE{
    int _fileno;
    int _end;
    char _buf[SIZE];
};
typedef struct myFILE MyFILE;

MyFILE* myfopen(const char* , const char* );
int myfwrite(MyFILE* , const char*, int);
void myfflush(MyFILE* );
int myfclose(MyFILE* );


