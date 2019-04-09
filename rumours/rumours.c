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
  int i, v = 0, size = argc - 1;
  char* str = (char *)malloc(v);
  // printf("%s\n", string );
  for(i = 1; i <= size; i++) {
    str = (char *)realloc(str, (v + strlen(argv[i])));
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
