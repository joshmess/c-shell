#include<iostream>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include<fcntl.h>
using namespace std;

pid_t childPID;

/*
 * This class implements a basic shell.
 */
int main() {

  char* line = NULL;
  size_t len = 0;
  char* envp = NULL;
  bool exit = false;

  const string home = (string)getenv("HOME");
  string finalp = "";

  //big do-while to facilitate shell looping
  do{

    line = NULL;
    //create/display prompt
    envp = getenv("PWD");
    string fullp = (string)envp;

    //home dir is contained
    if(home.compare(fullp.substr(0,home.length())) == 0) {

      finalp = "~" + fullp.substr(home.length());
    }else{
      finalp = fullp;
    }

    cout << "1730sh:" << finalp << "$ ";
    getline(&line, &len, stdin);
    line[strlen(line)-1] = '\0';
	
	if(strlen(line)==0){
      continue;
    }

    //populate vector with args
    int numArgs = 0, k=0;
    char* check = line;
    char** argv = new char*[10];
    char* splitter = strtok(line, " ");

    while(splitter != NULL){
        if(numArgs ==  10){
          cout << "Too many arguments!" << endl;
          return 3;
        }

        //set up IO redir
        if((strcmp("<", splitter)==0) || (strcmp(">", splitter)==0) || (strcmp(">>", splitter)==0)){
          k = numArgs;
          check = splitter;
        }

        argv[numArgs++] = splitter;
        splitter = strtok(NULL, " ");
	}

    char** tokens;
    if(k > 0){
      tokens = new char*[10];

      for(int i = 0; i < k; i++){
        tokens[i] = argv[i];
      }

      tokens[k] = '\0';
    }

    argv[numArgs++] = '\0';

    if(strcmp("exit",argv[0]) == 0){
      exit = true;
      break;
    }

    //set env var
    if(strcmp("export",argv[0]) == 0){
	
	 //split up var name and new val
      string update = (string)argv[1];
      bool beforeeq = true;
      string bvar = "";
      string bval = "";
      for(uint i=0;i<update.length();i++) {

        if(update.at(i) == '='){
          beforeeq = false;
        }else if(beforeeq){
          bvar += update.at(i);

        }else{
          bval += update.at(i);
        }

        } //for

      char var[100];
      char val[100];

      //make cstrings for method format
      strcpy(var,bvar.c_str());
      strcpy(v  if(setenv(var,val,1) == -1){
        perror("Problem setting env variable.");
        }


    }else if(strcmp("<", check)==0){       //IO redir 1
      tokens[k] = argv[k+1];
      tokens[k+1] = '\0';

      int pid = fork();

      if(pid == 0){  //in child
        int fd;

        if((fd = open(argv[k+1], O_RDWR)) == -1) perror("Open:");

        close(fd);
        dup(fd);

        execvp(tokens[0], tokens);

      }else{
        waitpid(pid, nullptr, 0);
      }

    }else if(strcmp(">", check) == 0){     //IO redir 2
      int fd;
      int pid = fork();

      if(pid == 0){   //in child
        if((fd = open(argv[k+1], O_RDWR | O_CREAT | O_TRUNC)) == -1) perror("Open");

        int j, k;
        if((j = dup2(fd, 1)) == -1) perror("dup2");
        if((k = dup2(fd, 2)) == -1) perror("dup2");

        close(fd);

        execvp(tokens[0], tokens);
        cout<< "exec error" << endl;

      }else{
        waitpid(pid, NULL, 0);
      }


    }else if(strcmp(">>", check) == 0){    //IO redir 3
	
	inr fd;
	 int pid = fork();

      if(pid == 0){     //in child
      if((fd = open(argv[k+1], O_RDWR | O_APPEND)) == -1) perror("Open");

      dup2(fd, 1);
      dup2(fd, 2);

      close(fd);

      execvp(tokens[0], tokens);
      cout<< "exec error" << endl;

      }else{
        waitpid(pid, NULL, 0);
      }

    }else{

      //fork and exec
        childPID = fork();
        int statloc;

        if(childPID == 0){
          execvp(argv[0], argv);
          cout << "Error execing: No such file or directory." << endl;
          break;
        }else{
          waitpid(childPID, &statloc, 0);

        }
      }


  }while(!exit);

  return 0;
}



