#include <unistd.h>
#include <fcntl.h>

#include <iostream>

int main(){
  int fd[2];
  int pid;

  pipe(fd);
  pid = fork();
  fcntl(fd[0], F_SETFL, O_NONBLOCK);
  if (pid == 0){
    sleep(1);
    
  }
  close(fd[0]);
  close(fd[1]);
  waitpid(pid, NULL, 0);
  while(1)
    ;
}
