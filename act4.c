#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    int fd[2];
    FILE* f1 = fopen("/etc/passwd", "r");
    int ff1 = fileno(f1);

    FILE* f2 = fopen("user.txt", "a");
    int ff2 = fileno(f2);
    
    char *p1[] = {"grep", "sshd", NULL};
    char *p2[] = {"$(whoami)", NULL};

    if (pipe(fd)<0){
        perror("Error de creació del pipe fd[]");
        return EXIT_FAILURE;
        /* NOTREACHED */
    }

    pid_t pid1, pid2;

    switch (pid1 = fork()){
        case -1:
            perror("Error fork() - pid2");
            return EXIT_FAILURE;
        case 0:
            // Fill -> ls -> imprimeix a stdout
            // Tanquem stdout i redireccionem stdout a l'escriptura fd[1] de la pipe
            ////dup2(fd[0],STDIN_FILENO);
            dup2(ff1, 0);
            close(fd[1]);
           // close(fd[0]);

            // llegir de la pipe, malloc y etc
            char * filtro;
            size_t *p1 = malloc(3*sizeof(int));
            read(fd[0], filtro, p1);
            strcpy(p1[1],filtro);
            close(fd[0]);
            execvp(p1[0], p1);
            return EXIT_FAILURE;
    }

    switch (pid2 = fork()){
        case -1:
            perror("Error fork() - pid1");
            return EXIT_FAILURE;
        case 0:
            // Fill -> wc -l -> llegeix stdin i imprimeix a stdout
            // Tanquem stdin per llegir i redireccionem stdin per llegir de la pipe fd[0]
            //dup2(fd[1],STDOUT_FILENO);
            dup2(ff2, 1);
            close(fd[0]);
            char * filtro2;
            size_t *p2 = malloc(3*sizeof(int));
            write(fd[1], filtro2, p2);
            strcpy(p2[0],filtro2);
            close(fd[1]);
            execvp(p2[1], p2);
            return EXIT_FAILURE;
    }

    close(fd[0]);
    close(fd[1]);
    waitpid(pid1,0,0);
    waitpid(pid2,0,0);
    return EXIT_SUCCESS;
}