#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

void childProcess(char **strArr, int* ioVal, int amp)
{
  if(ioVal[1] > 0)
  {
    int pipeArr[2];
    if(pipe(pipeArr) < 0)
      exit(1);
    int pid = fork();
    if(pid < 0)
      exit(1);
    if(pid == 0)
    {
      close(pipeArr[0]);
      int sizeOfString = 100;
      char* input = (char*)malloc(sizeOfString*sizeof(char));
      int ret = read(ioVal[1], input, sizeOfString);
      if(ret < 0)
        exit(1);
      if(write(pipeArr[1], input, sizeOfString) < 0)
        exit(1);

      exit(0);
    }
    else
    {
      if(dup2(pipeArr[0], 0) < 0)
        exit(1);
      close(pipeArr[1]);
      //ALways will wait for now
      if(!amp)
      {
        waitpid(pid, NULL, 0);
      }
      int retval = execvp(strArr[0], strArr);
      if(retval<0)
        exit(1);

      exit(0);
    }
  }
  else
  {
    int retval = execvp(strArr[0], strArr);
    if(retval < 0)
      exit(1);

    exit(0);
  }
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
      {
        retVal[0] = -1;
        exit(1);
      }
      if(dup2(outputFile, 1) < 0)
      {
        retVal[0] = -1;
        exit(1);
      }
      strArr[i+1] = (char*)NULL;
    }
    else if(strcmp(strArr[i], "<") == 0)
    {
      strArr[i] =  (char*)NULL;
      int inputFile = open(strArr[i+1], O_RDONLY, 0666);
      if(inputFile < 0)
        exit(1);

      retVal[1] = inputFile;
      if(inputFile < 0)
        retVal[1] = -1;
      strArr[i+1] = (char*)NULL;
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
        childProcess(strArr, retVal, ampersand);
        if(retVal[0] != -1)
          close(retVal[0]);
        if(retVal[1] != 0)
        {
          close(retVal[1]);
        }
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

