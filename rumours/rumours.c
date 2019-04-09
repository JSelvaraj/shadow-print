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
#include <sys/stat.h>
#include <string.h>


int count = 0;
int main(int argc, char *argv[])
{
  int id = -1;
  int fd[2];
  int v = 0;
  char* str;
  // printf("%s\n", string );
  for(int i = 1; i <= argc - 1; i++) {
    str = realloc(str, strlen(argv[i]));
    strcat(str, argv[i]);
    strcat(str, " ");
  }
  printf("string: %s\n", str);
  pipe(fd);
  while (count < 10 && id != 0) {
    if (id >= 0) {
      // write(fd[1], str, sizeof(argv));
    }
    
    id = fork();
    if (id != 0) { //parent
      close(fd[1]);
      open(fd[0]);

    } else { //child
      close(fd[0]);
      open(fd[1]);
    }

    count++;
  }

  return 0;
}
