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
#define SIZE 512
#define DELIM " "
#define END '\0'
char input[SIZE];
char* argv[SIZE];
char cwd[SIZE + 10];   
int EXIT = 0;

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

void SplitCommand(){
    if(input[0] == '\0')return;
    argv[0] = strtok(input, DELIM); 
    int i = 1;
    while(argv[i++] = strtok(NULL, DELIM));
    //for(i = 0; argv[i]; i++){
    //    printf("argv[%d] : %s\n", i, argv[i]);
    //}
}

void ExecuteCommand(){
    if(argv[0] == NULL)return;
    pid_t rid = fork();
    if(rid == 0){
        char* file = argv[0];
        execvp(file, argv); 
        /**
         *  The exec() functions return only if an error has occurred.  
         *  The return value is -1, and errno is set to indicate the error.        
         */
        exit(errno);
    }
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

int main(){
    int quit = 0;
    while(!quit){
        //0.Prompt    
        MakePrompt();
        //1.get input
        GetCommand();
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
