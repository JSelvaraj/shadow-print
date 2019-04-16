/**
* @file rumours.c
* @author 180012798
* @brief
*
*/
#include "rumours.h"

#define NUM_OF_PROCESSES 10

int count = 0;
int main(int argc, char *argv[])
{
  int child_id = 0, parent_id = getpid();
  int fd[2];
  char* str;

  for(int i = 1; i <= argc - 1; i++) { //combines the command line arguments into a single string
    str = realloc(str, strlen(argv[i]));
    strcat(str, argv[i]);
    strcat(str, " ");
  }
  int length = strlen(str); //calculates the length of the string so strlen isn't called multiple times
  char string[length];
  strcpy(string, str);
  pipe(fd);
  while (!child_id) { //controls the number of children made and ensures when a child becomes a parent the process closes. !child_id same as child_id == 0
    if (count != 0) {
      read(fd[0], str, length);
      strcpy(string, str);
      parent_id = getpid();
      printf("pid: %d  received string: %s child process number: %d\n", getpid(), string, count);
    }

    count++;
    if (count < NUM_OF_PROCESSES + 1) child_id = fork();
    else break;

    if (child_id != 0) { //parent
      srand(time(NULL) + count); // seeds the rand() function
      str = stringswap(string, length);
      write(fd[1], str, length);
    } else { //child
      printf("New Process: %d   Parent: %d\n", getpid() , parent_id);
    }
  }
  if(count != NUM_OF_PROCESSES + 1) wait(NULL); // formatting so the terminal directory doesn't intersect stdout
  return 0;
}

/**
This function takes in a string and swaps two random characters.

@Param string the string which has two character swapped.
@Param string_length the length of the string argument.
*/

char* stringswap(char string[], int string_length){
  int index1, index2;
  do  {
    index1 = rand() % (string_length - 1);
    index2 = rand() % (string_length - 1);
  } while (index1 == index2);
  printf("pid: %d Swapped indices: %d, %d\n",getpid(), index1, index2);
  char temp;
  temp = string[index1];
  string[index1] = string[index2];
  string[index2] = temp;
  return string;
}
