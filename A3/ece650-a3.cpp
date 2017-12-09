#include<vector>
#include<sys/types.h>
#include<sys/wait.h>
#include <unistd.h>
#include<iostream>
#include <fstream>
#include <cstring>
#include <cstdio>

using namespace std;

int procA(int argc,char**argv) {

    execv("./rgen",argv);
    perror("Error: can not open rgen.cpp");
    return 0;
}


int main(int argc,char**argv)
{
    std::vector<pid_t> kids;
    char *argpython[3];
    argpython[0] = (char *) "python";
    argpython[1] = (char *) "ece650-a1.py";
    argpython[2] = nullptr;
    // create a pipe
    int ABtoC[2];
    pipe(ABtoC);

    pid_t child_pid;
    pid_t child_pid1;
    pid_t child_pid2;

    child_pid = fork ();
    if (child_pid == 0)
    {
        dup2(ABtoC[1], STDOUT_FILENO);
        close(ABtoC[0]);
        close(ABtoC[1]);

        // read from rgen
        return procA(argc,argv);
    }
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }

    close(ABtoC[1]);
    // kids.push_back(child_pid);

    int pipe_python[2];
    pipe(pipe_python);

    child_pid1 = fork();
    if (child_pid1 == 0)
    {
        // read from pipe and write to pipe_python
        dup2(ABtoC[0], STDIN_FILENO);
        close(ABtoC[0]);
        close(pipe_python[0]);
        dup2(pipe_python[1],STDOUT_FILENO);
        close(pipe_python[1]);

        execvp("python", argpython);
        perror("Error:can not open the ece650-a1.py");
        return 0;
    }
    else if (child_pid1 < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }
    close(ABtoC[0]);
    close(pipe_python[1]);

    //kids.push_back(child_pid);


    FILE* stream=popen("./ece650-a2","w");
    child_pid2 = fork();

    //write into a2
    if (child_pid2 == 0)
    {
        char* line=NULL;
        //read from a1
        size_t bytes=0;
        FILE* a1_out = fdopen(pipe_python[0], "r");
        while(getline(&line,&bytes,a1_out)!=-1){
            printf("%s",line);
            fputs(line,stream);
            fflush(stream);
        }
        fclose(a1_out);
        fclose(stream);
        //kill (getppid(), SIGTERM);
        exit(EXIT_SUCCESS);
    }
    else if (child_pid2 < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }


    close(pipe_python[0]);

    //kids.push_back(child_pid);
    //child_pid = 0;

    //read from keyboard and write to a2
    char* line1=NULL;
    size_t byte=0;
    while(getline(&line1,&byte,stdin)!=-1){
        fputs(line1,stream);
        fflush(stream);
    }



    kill(child_pid,SIGTERM);
    kill(child_pid1,SIGTERM);
    kill(child_pid2,SIGTERM);
    pclose(stream);
    return EXIT_SUCCESS;
}
