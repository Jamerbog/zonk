#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define PATH_MAX 1024

int pid;

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
		tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
		strcpy(tokens[tokenNo++], token);
		tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

void execCommand(char ** tokens, int i, char* user)
{
	pid = fork();
	
	if (pid == 0 && strcmp(tokens[0], "cd") != 0)
	{
		if (-1 ==  execlp(tokens[0], tokens[0], tokens[1], tokens[2], tokens[3], tokens[4],
		tokens[5], tokens[6], tokens[7], tokens[8], tokens[9], tokens[10], NULL)) {
			if (i == 0)
			{
				printf("Failed to find program '%s' in PATH.\n", tokens[0]);
			}
		}
	}

	else if (pid == 0 && *tokens[1] != '~') {
		chdir(tokens[1]);
	}

	else if (pid == 0 && *tokens[1] == '~') {
		char homePath[64] = "/home/";
		strcat(homePath, user);
		chdir(homePath);
	}

	if (pid != 0)
	{
		wait(NULL);
	}
}

void displayPrompt(char* user)
{
	char cwd[PATH_MAX];

	getcwd(cwd, sizeof(cwd));

	char hostname[1024];
    gethostname(hostname, 1024);

	char homePath[64] = "/home/";
	strcat(homePath, user);

	if (strcmp(cwd, homePath) == 0)
	{
		strcpy(cwd, "~");
	}

	printf("[%s@%s] - [%s]\n --$ ", user, hostname, cwd);
}

int main(int argc, char* argv[]) 
{
	char  line[MAX_INPUT_SIZE];          
	char  **tokens;   
	int i;

	while(1) 
	{			
		/* BEGIN: TAKING INPUT */

		bzero(line, sizeof(line));
		
		char *user=getenv("USER");
		if(user==NULL) return EXIT_FAILURE;
		
		displayPrompt(user);	
		scanf("%[^\n]", line);
		getchar();

		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

		execCommand(tokens, i, user);		
		
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++)
		{
			free(tokens[i]);
		}

		free(tokens);
	}	

	return 0;
}
