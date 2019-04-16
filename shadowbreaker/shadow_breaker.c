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
  struct crypt_data cdata;
  cdata.initialized = 0;


  thread_in *input = (thread_in *)data;
  char partial_password[9];
  strcpy(partial_password, input->partial_password);
  int prefix_len = getPrefixLength(input->partial_password);
  int unknown_letter_count = strlen(input->partial_password) - prefix_len;

  char* unknowns = &partial_password[prefix_len];


  long start_index, count;
  getSubrange(unknown_letter_count, input->thread_count, input->thread_num, &start_index, &count);
  setStringPosition(unknowns, start_index);
  int hashCount = 0;
  print_thread_parr_start(input->thread_num, input->username, start_index, partial_password);
  const char* hashed;
  hashed = crypt_r(partial_password,"cs2002", &cdata);
  hashCount++;
  while (strcmp(input->hash, hashed) && count) {
    if (done_flag) break; // checks if other threads have found the password
    incrementString(unknowns);
    hashed = crypt_r(partial_password,"cs2002", &cdata);
    hashCount++;
    count--;
  }

  results *returnObject = malloc(sizeof(results));
  returnObject -> hashCount = hashCount;
  if (!strcmp(input->hash, hashed)) {
    print_thread_parr_result(input->thread_num, hashCount, 0);
    done_flag = 1;
    returnObject->result = 0;
    returnObject->password = malloc(9);
    strcpy(returnObject->password, partial_password);
  } else if (!count) {
    print_thread_parr_result(input->thread_num, hashCount, 2);
    returnObject->result = 1;
  } else {
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

  char* username = malloc(9);
  scanf("%s ", username);
  // printf("%s\n", username);
  char* hash = malloc(14);
  scanf("%s", hash);
  // printf("hash: %s\n", hash);
  char* partial_password = malloc(9);
  scanf("%s", partial_password);
  // printf("%c\n", partial_password[3] );
  print_parr_start_user(username);
  pthread_t thr[thread_count];
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
