/**
* @file rumours.c
* @author 180012798
* @brief
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h> 

int main(int argc, char *argv[])
{
  int id = fork();
  if (id == 0) {
    printf("%s\n", argv[1]);
  } else {
    printf("%s\n", argv[2]);
  }
  return 0;
}
