#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

void childProcess(char **strArr)
{
  int retval = execvp(strArr[0], strArr);
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

void parentProcess(int ampersand)
{
 if(!ampersand)
 {
    wait(NULL);
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
        input[charIndex] = c;
        charIndex++;
        if(charIndex >= sizeOfString)
        {
          sizeOfString = sizeOfString*2;
          input = (char*)realloc(input, sizeOfString*sizeof(char));
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
  char* * strArr = (char**)malloc(sizeOfArray*sizeof(char*));
  int quit = 0;

  while(!quit)
  {
    printf("CS350sh: ");
    strArr = getAllArgs(strArr, sizeOfArray, sizeOfString);

    int ampersand = does_have('&', strArr);
    if(strcmp(strArr[0], "quit") != 0)
    {
      int pid = fork();
      if(pid == 0)
      {
        childProcess(strArr);
      }
      else
      {
        parentProcess(ampersand);
      }
    }
    else
    {
      ampersand = 0;
      quit = 1;
    }
  }

  free(strArr);
  return 0;
}

