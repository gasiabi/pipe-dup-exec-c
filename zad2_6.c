#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int fd[2], status, done=0; // fd[0] -> reading, fd[1] -> writing
    pid_t pid;

    if (argc != 3) {
        fprintf(stderr, "Użycie: %s <command1> <command2>\n", argv[0]);
        exit(1);
    }

    pipe(fd);

    pid = fork(); // proces numer 1
    if (pid < 0) {
        perror("Wystąpił błąd podczas tworzenia procesu.\n");
        return 1;
    } else if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        execlp(argv[1], argv[1], (char *) NULL); 
    }

    pid = fork(); // proces numer 2
    if (pid < 0) {
        perror("Wystąpił błąd podczas tworzenia procesu.\n");
        return 1;
    } else if (pid == 0) {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        execlp(argv[2], argv[2], (char *) NULL); 
    }

    close(fd[0]);
    close(fd[1]);

    waitpid(-1,&status, 0);
    waitpid(-1,&status,0);
  
}