/**
* @file rumours.c
* @author 180012798
* @brief
*
*/
#include "rumours.h"

int count = 0;
int main(int argc, char *argv[])
{
  int child_id = -1, parent_id = 0;
  int fd[2];
  char* str;
  // printf("%s\n", string );
  for(int i = 1; i <= argc - 1; i++) {
    str = realloc(str, strlen(argv[i]));
    strcat(str, argv[i]);
    strcat(str, " ");
  }
  // printf("string: %s\n", str);
  pipe(fd);
  while (count < 10 && child_id != 0) {
    if (count != 0) {
      read(fd[1], str, sizeof(str));
      parent_id = child_id;
      // printf("%s\n",str );
    } else {
      parent_id = getpid();
    }
      // printf("in loop %d\n", count);
      // printf("PID:  %d\n",child_id);
   child_id = fork();

    if (child_id != 0) { //parent
      write(fd[0], str, sizeof(str));
    } else { //child
      printf("New Process: %d   Parent: %d\n", getpid() , parent_id);
    }
    count++;
  }
  return 0;
}

char* stringswap(char* string){

}
