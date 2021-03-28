#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1


char** parse(char* s) {
  static char* words[500];
  memset(words, 0, sizeof(words));
  
  char break_chars[] = " \t";

  int i = 0;
  char* p = strtok(s, break_chars);
  words[i++] = p;
  while (p != NULL) {
    p = strtok(NULL, break_chars);
    words[i++] = p;
  }
  return words;
}


int main(int argc, const char * argv[]) {  
  char input[BUFSIZ];
  char last_command[BUFSIZ];
  
  memset(input, 0, BUFSIZ * sizeof(char));
  memset(input, 0, BUFSIZ * sizeof(char));
  bool finished = false;
  
  while (!finished) {
    printf("osh> ");
    fflush(stdout);

    if ((fgets(input, BUFSIZ, stdin)) == NULL) {   // or gets(input, BUFSIZ);
      fprintf(stderr, "no command entered\n");
      exit(1);
    }
    input[strlen(input) - 1] = '\0';          // wipe out newline at end of string
    printf("input was: \n'%s'\n", input);

    // check for history (!!) command
    if (strncmp(input, "!!", 2) == 0) {
      if (strlen(last_command) == 0) {
        fprintf(stderr, "no last command to execute\n");
      }
      printf("last command was: %s\n", last_command);
    } else if (strncmp(input, "exit", 4) == 0) {   // only compare first 4 letters
      finished = true;
    } else {
      strcpy(last_command, input);
      printf("You entered: %s\n", input);   // you will call fork/exec
      parse(input);
			printf("after parse, what is input: %s\n", input);
      printf("\n");
      
      pid_t pid = fork();

        if (pid == 0) {
                printf("I am the child.\n");
      
        }
        if (pid > 0) {
                printf("I am the parent, the child is %d.\n", pid);

        }
        if (pid < 0) {
                perror("In fork():");
                char *args[]={"./exec",NULL};
                execv(args[0],args);

        }
      	wait(NULL);
  
    }
  }
  
  printf("osh exited\n");
  printf("program finished\n");
  
  return 0;
}
