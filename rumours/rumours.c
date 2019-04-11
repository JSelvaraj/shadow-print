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
  int length = strlen(str);
  srand(time(NULL));
  pipe(fd);
  while (count < 10 && child_id != 0) {
    if (count != 0) {

      read(fd[1], str, sizeof(str));
      parent_id = child_id;
      printf("pid: %d  received string: %s\n", getpid(), str );
    } else {
      parent_id = getpid();
    }
    // printf("in loop %d\n", count);
    // printf("PID:  %d\n",child_id);
    child_id = fork();

    if (child_id != 0) { //parent
      printf("pid: %d", parent_id);
      stringswap(str, length);
      write(fd[0], str, sizeof(str));
    } else { //child
      printf("New Process: %d   Parent: %d\n", getpid() , parent_id);
    }
    count++;
  }
  return 0;
}

void stringswap(char* string, int string_length){
  int index1, index2;
  do  {
    index1 = rand() % string_length;
    index2 = rand() % string_length;
  } while (index1 == index2);
  printf("  Swapped indices: %d, %d\n", index1, index2);
  char temp;
  temp = string[index1];
  string[index1] = string[index2];
  string[index2] = temp;
}
