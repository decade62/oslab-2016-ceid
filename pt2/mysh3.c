#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <termios.h>

#define FIXCHARS " \t\r\n\a"
#define NONCHARS "\r\n"
#define BUFSIZE 64

int count_pipe ( char **args ) {
  int arg = 0;
  int pipes = 0;
  while ( args[arg] != NULL ) { 
    if ( strcmp( args[arg],"|" ) == 0 )
      pipes++;
    arg++;
  }
  return pipes;
}

char **fix_arg_table( char *command ){
  int bufsize = BUFSIZE, 
  position = 0;
  char **buffer;
  char *arg;
  buffer = malloc( BUFSIZE * sizeof(char*) ); /* Allocation */
  if( buffer == NULL ) exit(0); /* Error @ malloc */
  arg = strtok( command, FIXCHARS );
  while ( arg != NULL ) {
    buffer[position] = arg;
    position++;
    if ( position >= bufsize ) {
      bufsize += BUFSIZE;
      buffer = realloc( buffer, bufsize * sizeof(char*) ); /* Re-Allocate the buffer */
      if( buffer == NULL ) exit(0); /* Error @ realloc */
    }
    arg = strtok(NULL, FIXCHARS);
  }
  buffer[position] = NULL;
  return buffer;
}

void do_pipe_command( char ** argv ) {
  pid_t child,pid;
  int status, i = 0, fd[2];
  char **pipe_command;
  char *buffer; /* Stores the user input */
  buffer = (char *)malloc(BUFSIZE * sizeof(char)); /* Allocation */
  if( buffer == NULL ) exit(0); /* Error @ malloc */
  pipe( fd );
  memset( buffer,0,strlen( buffer ) ); /* Clear command buffer */
	while (strcmp(argv[i],"|" )!= 0) { /* Slpit command */
	  strcat(buffer,argv[i]);
	  i++;  
	  if (argv[i]==NULL) /* Parse the command */
	    break;
	  strcat(buffer, " "); /* Fix the space */
	}
  pipe_command=fix_arg_table(buffer); /* Slpit command */
  i++; /* Skip Pipe */
  child = fork();
  if ( child == (pid_t)-1 ) exit(0); /* Error @ fork */
  else if (!child) { /* Child process */
  	close (fd[0]);
    dup2(fd[1],1);
    close (fd[1]);
    execvp(pipe_command[0],pipe_command);
	}
	if (child) {  /* This is the parent */
		pid = fork();
    if (pid == (pid_t)-1 ) exit(0); /* Error @ fork */
    else if (!pid) { /* Child */
      close (fd[1]);
      dup2 (fd[0], 0);
      close (fd[0]);
      memset( buffer,0,strlen( buffer ) ); /* Clear command buffer */
      while ( strcmp(argv[i],"|" ) != 0 ) { /* Slpit command */
		  	strcat( buffer, argv[i] );
		  	i++;  
		  	if (argv[i] == NULL) /* Parse the command */
		    	break;
		  	strcat(buffer," "); /* Fix the space */
	  	}
	  	pipe_command=fix_arg_table(buffer); /* Slpit command */
      execvp(pipe_command[0],pipe_command);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid (pid, &status, 0);
	} 
}

void exec_cd ( char **argv ) { chdir(argv[1]); }

void exec_command ( char **buffer ) {
  pid_t child;
  int status;
  child = fork();
  if(child == (pid_t)-1)
    exit(0);
  if (!child) /* Child */
  	execvp( buffer[0], buffer );
  else 
  	waitpid (child, &status, 0);
}

char *command_reader(void) {
  char *buffer;
  size_t bufsize = BUFSIZE, helper;         
  buffer = (char *)malloc(bufsize*sizeof(char));
  if( buffer == NULL) exit(0);
  helper = getline(&buffer, &bufsize, stdin); /* Read stdin. */
  if ( helper > (ssize_t)0 ) {
    if ( strcspn( buffer, NONCHARS ) > (size_t)0 ) /* Terminate input command before the newline. */
      buffer[strcspn( buffer, NONCHARS )] = '\0';
    return buffer;
  } 
  return NULL;
}

int main( int argc, char **argv ) {
  char *command;
  char **args; 
  do {
    fprintf(stdout, "$ "); /* Delimiter */
    fflush(stdout); 
    command = command_reader();
    if (command != NULL) {
      if ( !strcmp( command, "exit\0" ) )
        break; /* Exit */
      else {
        args = fix_arg_table( command );
        if ( !strcmp( command, "cd\0" ) ) /* Handle "cd" command */ 
          exec_cd( args );
        else {
          if ( count_pipe( args ) == 1 )
            do_pipe_command( args ); /* Do pipe */
          else if ( count_pipe( args ) == 0 )
            exec_command( args ); /* Simple command */
        }
      }
    }
    free(command); /* Clear */
  } while (1);
  return 0;
}