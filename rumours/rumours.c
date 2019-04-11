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
  int child_id = 0, parent_id = getpid();
  int fd[2];
  char* str;
  // printf("%s\n", string );
  for(int i = 1; i <= argc - 1; i++) { //combines the command line arguments into a single string
    str = realloc(str, strlen(argv[i]));
    strcat(str, argv[i]);
    strcat(str, " ");
  }
  int length = strlen(str); //calculates the length of the string so strlen isn't called multiple times
  char string[length];
  strcpy(string, str);
  pipe(fd);
  while (count < 10 && !child_id) { //controls the number of children made and ensures when a child becomes a parent the process closes.
    if (count != 0) {
      read(fd[0], str, sizeof(str));
      parent_id = getpid();
      printf("pid: %d  received string: %s\n", getpid(), str );
    }
    child_id = fork();
    if (child_id != 0) { //parent
      srand(time(NULL) + count); // seeds the rand() function
      stringswap(str, length);
      // printf("modified str: %s\n", str);
      write(fd[1], str, sizeof(str));
    } else { //child
      printf("New Process: %d   Parent: %d\n", getpid() , parent_id);
    }
    count++;
  }
  if(count == 1) { // formatting so the terminal directory doesn't intersect stdout
    sleep(1);
  }
  return 0;
}

/**
This function takes in a string and swaps two random characters.

@Param string the string which has two character swapped.
@Param string_length the length of the string argument.
*/

void stringswap(char string[], int string_length){
  int index1, index2;
  do  {
    index1 = rand() % string_length;
    index2 = rand() % string_length;
  } while (index1 == index2);
  printf("pid: %d Swapped indices: %d, %d\n",getpid(), index1, index2);
  char temp;
  temp = string[index1];
  string[index1] = string[index2];
  string[index2] = temp;
}
