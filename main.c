#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

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
  char** strArr = (char**)malloc(sizeOfArray*sizeof(char*));
  strArr = getAllArgs(strArr, sizeOfArray, sizeOfString);

  //printf("First command: %s\n", command);
  int g=0;
  while(strArr[g] != NULL)
  {
    printf("Arg %d: %s\n", g, strArr[g]);
    g++;
  }

  int retval = execvp(strArr[0], strArr);
  printf("retval: %d", retval);
  free(strArr);
  return 0;
}

