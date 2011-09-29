#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

void childProcess(char **strArr)
{
  int retval = execvp(strArr[0], strArr);
}
int num_args(char **strArr)
{
  int numArgs = 0;
  while(strArr[numArgs] != NULL)
  {
    numArgs++;
  }
  return numArgs;
}

int* checkIoOperations(char** strArr, int numArgs, int* retVal)
{
  retVal[0] = 0;
  retVal[1] = 0;
  int i = 0;
  for(i = numArgs-1; i >= 0; i--)
  {
    if(strcmp(strArr[i], ">") == 0)
    {
      strArr[i] = (char*)NULL;
      int outputFile = open(strArr[i+1], O_CREAT | O_WRONLY, 0666);
      retVal[0] = outputFile;
      if(outputFile < 0)
        retVal[0] = -1;
      if(dup2(outputFile, 1) < 0)
        retVal[0] = -1;
      strArr[i+1] = (char*)NULL;
    }
    else if(strcmp(strArr[i], "<") == 0)
    {

    }
  }
  return retVal;
}

int does_have(char letter, char **strArr)
{
  int i = 0;
  int exists = 0;
  while(strArr[i] != NULL)
  {
    if(strArr[i][0] == letter)
    {
      exists = 1;
      strArr[i] = (char*)NULL;
    }
    i++;
  }

  return exists;
}

void parentProcess(int pid, int ampersand)
{
 if(!ampersand)
 {
    waitpid(pid, NULL, 0);
 }
}

char** getAllArgs(char **strArr, int sizeOfArray, int sizeOfString)
{
  char c;
  int numArgs = 0;
  char* input = (char*)malloc(sizeOfString*sizeof(char));
  int charIndex = 0;
  while(c != '\n')
  {
      c = getchar();
      if(c == ' ' || c == '\n')
      {
        input[charIndex] = '\0';
        strArr[numArgs] = input;
        numArgs++;
        if(numArgs == sizeOfArray)
        {
          sizeOfArray = sizeOfArray+1;
          strArr = (char**)realloc(strArr, sizeOfArray*sizeof(char*));
        }
        input = (char*)malloc(sizeOfString*sizeof(char));
        charIndex = 0;
      }
      else
      {
        if(c != -1)
        {
          input[charIndex] = c;
          charIndex++;
          if(charIndex+1 == sizeOfString)
          {
            sizeOfString = sizeOfString*2;
            input = (char*)realloc(input, sizeOfString*sizeof(char));
          }
        }
      }
  }
  strArr[numArgs] = (char*)NULL;
  return strArr;
}

int main(int argc, char ** argv)
{
  int sizeOfString= 16;
  int sizeOfArray = 2;
  int quit = 0;

  while(!quit)
  {
    char* * strArr = (char**)malloc(sizeOfArray*sizeof(char*));
    int* retVal = (int*)malloc(2*sizeof(int));
    printf("CS350sh: ");
    strArr = getAllArgs(strArr, sizeOfArray, sizeOfString);
    int numArgs = num_args(strArr);
    int ampersand = does_have('&', strArr);
    if(strcmp(strArr[0], "quit") != 0)
    {
      int pid = fork();
      if(pid == 0)
      {
        retVal = checkIoOperations(strArr, numArgs, retVal);
        childProcess(strArr);
        if(retVal[0] != -1)
          close(retVal[0]);
      }
      else
      {
        parentProcess(pid, ampersand);
      }
    }
    else
    {
      ampersand = 0;
      quit = 1;
    }
  free(strArr);
  free(retVal);
  }

  return 0;
}

