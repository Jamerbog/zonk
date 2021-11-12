#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

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

char* catTokens(char ** tokens)
{
	static char tokenString[MAX_INPUT_SIZE];

	for (int i=0; tokens[i] != NULL; i++)
	{
		strcat(tokenString, tokens[i]);
		strcat(tokenString, ", ");
	}

	int size = strlen(tokenString);
	tokenString[size-2] = '\0';

	return tokenString;
}



void execCommand(char ** tokens, int i)
{
	
	char tokenString[MAX_INPUT_SIZE];
	
	strcpy(tokenString, catTokens(tokens));

	printf("%s\n", tokenString);

	

	pid = fork();

	
	if (pid == 0 && strcmp(tokens[0], "cd") != 0)
	{
		if (-1 ==  execlp(tokens[0], tokenString, NULL)) {

			if (i == 0)
			{
				printf("Failed to find program '%s' in PATH.\n", tokens[0]);
			}
		}
	}

	else if (pid == 0) {
		chdir(tokens[1]);
	}

	tokenString[0] = '\0';

	if (pid != 0)
	{
		wait(NULL);
	}
}

int main(int argc, char* argv[]) {

	char  line[MAX_INPUT_SIZE];            
	char  **tokens;   
	int i;

	while(1) 
	{			
		/* BEGIN: TAKING INPUT */

		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();

		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

		execCommand(tokens, i);		
		
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++)
		{
			free(tokens[i]);
		}

		free(tokens);
	}	

	return 0;
}
