/*************************************************************************
	> File Name: MiniShell.c
	> Author: 
	> Mail: 
	> Created Time: Sat 23 Mar 2024 08:41:31 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<fcntl.h>
#define SIZE   512
#define DELIM  " "
#define END    '\0'
#define READ   0
#define WRITE  1
#define APPEND 2
char *filename = NULL;
int REDIR =    -1;
char input[SIZE];
char* argv[SIZE];
char cwd[SIZE + 10];   
int EXIT = 0;
int FD = -1;

char* getUser(){
    return getenv("USER");
}

char* getHost(){
    //return getenv("HOST");
    return "119.91.249.213";
}

char* getCwd(){
    return getenv("PWD");
}

void MakePrompt(){
    char* user = getUser();
    char* host = getHost();
    char* cwd = getCwd();
    cwd = strrchr(cwd, '/');
    printf("[%s@%s %s] ", user, host, strlen(cwd) == 1 ? cwd : cwd + 1);
    fflush(stdout);
}

void GetCommand(){
    fgets(input, SIZE, stdin);
    input[strlen(input) - 1] = END;
}

void CheckRedirection(){
    int end = strlen(input); 
    int pos = 0;
    while(pos < end){
        if(input[pos] == '>'){
            if(input[pos + 1] == '>'){
                input[pos] = '\0';
                pos += 2;
                while(input[pos] == ' ')pos++;
                REDIR = APPEND;
                filename = input + pos;
                //FD = open((const  char*)input + pos, O_CREAT | O_WRONLY | O_APPEND, 0666);
            }else {
                input[pos] = '\0';
                pos++;
                while(input[pos] == ' ') pos++;
                REDIR = WRITE;
                filename = input + pos;
                //FD = open((const char*)&input[pos], O_CREAT | O_WRONLY | O_TRUNC, 0666);
            }
            break;
        }else if(input[pos] == '<'){
            input[pos] = '\0'; 
            pos++;
            while(input[pos] == ' ')pos++; 
            REDIR = READ;
            filename = input + pos;
            //FD = open((const char*)input + pos, O_RDONLY);
            break;
        }else{
            pos++;
        }
    }
}

void SplitCommand(){
    if(input[0] == '\0')return;
    argv[0] = strtok(input, DELIM); 
    int i = 1;
    while(argv[i++] = strtok(NULL, DELIM));
    //for(i = 0; argv[i]; i++){
    //    printf("argv[%d] : %s\n", i, argv[i]);
    //}
}

int RunBuiltin(){
    int yes = 0;
    if(argv[0] == NULL)return 0;
    //strcmp会解引用
    if(strcmp("exit", argv[0]) == 0){
        if(argv[1] == NULL)
            exit(0); 
        else 
            exit(atoi(argv[1]));
    }else if(strcmp("cd", argv[0]) == 0){
        if(argv[1] == NULL || strcmp(argv[1], "~") == 0){
            //argv[1] = getCwd();
            //char tmp[SIZE] = "";
            //int pos = strchr(strchr(argv[1] + 1, '/') + 1, '/') - argv[1];
            ////+1是为了给'\0'留空间
            //snprintf(tmp, pos + 1, "%s", argv[1]);
            //argv[1] = tmp;
            char* user = getUser();
            char tmp[SIZE] = "/home/";
            snprintf(tmp + strlen(tmp), strlen(user) + 1, "%s", user);
            argv[1] = tmp;
        }
        chdir(argv[1]);

        //更新环境变量
        char tmp[SIZE] = ""; 
        getcwd(tmp, sizeof tmp);
        snprintf(cwd, sizeof cwd, "PWD=%s", tmp);
        putenv(cwd);
        yes = 1;
    }else if(strcmp("pwd", argv[0]) == 0){
        printf("%s\n", getenv("PWD"));
        yes = 1;
    }else if(strcmp("echo", argv[0]) == 0 && strcmp("$?", argv[1]) == 0){
        printf("%d\n", EXIT);
        EXIT = 0;
        yes = 1;
    }
    return yes;
}

void ExecuteCommand(){
    if (argv[0] == NULL) return;
    pid_t rid = fork();
    if (rid == 0) {
        if (REDIR == READ) {
            FD = open((const char*)filename, O_RDONLY); 
            dup2(FD, 0);
        }else if (REDIR == WRITE) {
            FD = open((const char*)filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
            dup2(FD, 1);
        }else if (REDIR == APPEND) {
            FD = open((const char*)filename, O_CREAT | O_WRONLY | O_APPEND, 0666);
            dup2(FD, 1);
        }
        char* file = argv[0];
        execvp(file, argv); 
        /**
         *  The exec() functions return only if an error has occurred.  
         *  The return value is -1, and errno is set to indicate the error.        
         */
        exit(errno);
    }
    if(FD != -1)close(FD);
    int status = 0;
    int ret = waitpid(rid, &status, 0); 
    if(ret > 0){
        EXIT = WEXITSTATUS(status); 
        if(EXIT)printf("%s : %s : %d\n", argv[0],  strerror(EXIT), EXIT);
    }
    
    //子进程状态
    //if(!WIFEXITED(status)){
    //    printf("abnormal exit\n"); 
    //}else{
    //    //存在着问题
    //    if(WEXITSTATUS(status)){
    //        printf("%s : %s\n", argv[0], strerror(errno));
    //    }    
    //}
}


int main(){
    int quit = 0;
    while(!quit){
        //prepare reset
        REDIR = -1;
        filename = NULL;
        FD = -1;
        //0.Prompt    
        MakePrompt();
        //1.get input
        GetCommand();
        //1.1 check redirection
        CheckRedirection();
        //2.split input
        SplitCommand();
        //3.run builtin
        if(RunBuiltin()) 
            continue;
        //4.execute noraml command
        ExecuteCommand();
    }
     return 0;
}
