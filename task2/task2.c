#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>

void* worker(void *data){
  sleep(1);
  if(data) {
    pthread_join((pthread_t)data, NULL);
  }
  printf("thread_id being waited on: %d current thread_id: %d\n", (int) data, pthread_self());
  return NULL;
}
int main(){
  pthread_t thread = (pthread_t)NULL; //1st argument stores id of the thread created, 3rd argument is function being made into new thread
  for(int i = 0; i < 5; ++i){
    pthread_create(&thread, NULL, worker, (void*)thread);
    printf("Thread (%d) id: %d\n", i, thread );
  }
  pthread_join(thread, NULL);
  printf("thread_id being waited on: %d current thread_id: %d\n",  (int) thread, pthread_self());
}
