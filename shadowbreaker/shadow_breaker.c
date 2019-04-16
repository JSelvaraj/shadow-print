#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <crypt.h>
#include "shadow_breaker.h"
#include "utils.h"
#include "output_helpers.h"
#include "queue_r.h"

typedef struct {
  int thread_num;
  char* username;
  char* hash;
  char* partial_password;
  size_t thread_count;
} thread_in;

typedef struct returnObject {
  int hashCount;
  int result;
  char* password;

} results;

int done_flag = 0;
/**
* REQUIRED FUNCTION - This is the start of your threads.
*/
void* thread_start_routine(void *data)
{
  /*
    initializing the crypt_r function.
  */
  struct crypt_data cdata;
  cdata.initialized = 0;

  thread_in *input = (thread_in *)data; //So I wouldn't have to cast data every line
  char partial_password[9];
  strcpy(partial_password, input->partial_password);
  int prefix_len = getPrefixLength(input->partial_password);
  int unknown_letter_count = strlen(input->partial_password) - prefix_len;
  int hashCount = 0;
  const char* hashed;

  char* unknowns = &partial_password[prefix_len];

  long start_index, count;
  getSubrange(unknown_letter_count, input->thread_count, input->thread_num, &start_index, &count);
  setStringPosition(unknowns, start_index);
  print_thread_parr_start(input->thread_num, input->username, start_index, partial_password);
  hashed = crypt_r(partial_password,"cs2002", &cdata);
  hashCount++;
  while (strcmp(input->hash, hashed) && count) {
    if (done_flag) break; // checks if other threads have found the password
    incrementString(unknowns);
    hashed = crypt_r(partial_password,"cs2002", &cdata);
    hashCount++;
    count--;
  }

  results *returnObject = malloc(sizeof(results)); // struct containing return values
  returnObject -> hashCount = hashCount;
  if (!strcmp(input->hash, hashed)) { // i.e. if password found
    print_thread_parr_result(input->thread_num, hashCount, 0);
    done_flag = 1;
    returnObject->result = 0;
    returnObject->password = malloc(9);
    strcpy(returnObject->password, partial_password);
  } else if (!count) { //if all hashes in range explored but password not found
    print_thread_parr_result(input->thread_num, hashCount, 2);
    returnObject->result = 1;
  } else { // only occurs if password found in another thread and current thread cancelled
    print_thread_parr_result(input->thread_num, hashCount, 1);
    returnObject->result = 1;
  }
  free(input);
  return (void *) returnObject;
}


/**
* REQUIRED FUNCTION - Called by main to launch and manage threads.
*/
int start(size_t thread_count) {
  double start_time = getTime();
  double start_cpu_time = getCPUTime();

  /*
    This part gets the username, hash and partial password from stdin.
  */
  char* username = malloc(9);
  scanf("%s ", username);
  char* hash = malloc(14);
  scanf("%s", hash);
  char* partial_password = malloc(9);
  scanf("%s", partial_password);
  print_parr_start_user(username);

  pthread_t thr[thread_count]; // this array will store the thread ids of the spawned threads

  /*
    This For loop mallocs a struct of input arguments and spawns the threads.
  */
  for (int i = 1; i <= thread_count; i++) {
    thread_in *data = malloc(sizeof(thread_in));
    data->username = username;
    data->thread_num = i;
    data->hash = hash;
    data->partial_password = partial_password;
    data->thread_count = thread_count;
    pthread_create(&thr[i-1], NULL, thread_start_routine, (void *)data); // stored in thr[i-1 because for loop begins at 1 but array begins at 0]
  }

  int hashCount = 0;
  void *temp = NULL;
  char* password = malloc(9);
  int result = 1;

  /*
    This for loop waits for each thread to join the main thread. Not perfect as it always waits in order even if the second thread finished first.
  */
  for (int i = 0; i < thread_count; i++) {
    pthread_join(thr[i], &temp);
    hashCount += ((results *)temp)->hashCount;
    if (((results *)temp)->result == 0) {
      strcpy(password, ((results *) temp)->password);
      free(((results *) temp)->password);
      result = 0;
    }
    free(temp);
  }
  double elapsed = getTime() - start_time;
  double total_cpu_time = getCPUTime() - start_cpu_time;
  print_parr_summary(username, password, hashCount, elapsed, total_cpu_time, result);
  free(username);
  free(hash);
  free(partial_password);
  free(password);
  return 0; // 0 indicates success
}
