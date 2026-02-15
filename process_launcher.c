#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid;
    pid = fork();
    if (pid == 0){
        char *args[]={"./myname",NULL};
        execv("./myname",args);
        printf("execv failed. \n");
    } else {
        wait(NULL);
        printf("child process finished execution.\n");
    }
    return 0;
};