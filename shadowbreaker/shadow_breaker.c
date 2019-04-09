#include <stdio.h>
#include <string.h>
#include "shadow_breaker.h"
#include "utils.h"
#include "output_helpers.h"
#include "queue_r.h"

/******* OPTIONAL TEMPLATE CODE STARTS HERE -- MODIFY OR DELETE *******/

/**
 * You may wish to define a struct for your task here.
 *  Remember that enqueue/dequeue funcs take and return (void *)!
 * 
 * [Advanced] Separately, you may wish to define a de-structor function for
 *  use by queue_destroy() in "queue.h" -- see that header for details.
 */

typedef struct {
  char a[22];
  char *b;
} myX;

void myX_destroy(void *data)
{
  myX *x = (myX *) data;
  // We don't free(x->a) since a[22] was never mallec'ed.
  free(x->b);
  free(x);
}
/******* OPTIONAL POSSIBLE TEMPLATE CODE __ENDS__ HERE *******/



/**
 * REQUIRED FUNCTION - This is the start of your threads.
 */
int thread_start_routine() 
{
  // TODO thread execution starts with this function
  return 0;
}


/**
 * REQUIRED FUNCTION - Called by main to launch and manage threads.
 */
int start(size_t thread_count) {

  // TODO your code here, make sure to use thread_count!
  // Remember, this is only the thread launching point, i.e. this function:
  //   - calls pthread_create(..., ..., &thread_start_routine, ...) 
  //   - reads tasks (one per line) from stdin, etc.
  //   - example queue code is 'just in case' anyone wishes to handle
  //     multiple passwords.

  
  // All code below is for demonstrative use of queue only.
  queue *q = queue_create(-1);

  myX *data;

  for(int i = 0; i < 1000; ++i) {
    data = malloc(sizeof(myX));
    
    // Contents may or may not need to be handled in myX_destroy()
    (void)strncpy(data->a, "Hello ", sizeof(data->a));
    data->b = strdup("World! "); // strdup mallocs
    fprintf(stderr, "%d ", i);

    enqueue(q, (void *)data);
  }

  data = NULL;

  for(int i = 500; i > 0; --i) {
    data = (myX *) dequeue(q);
    if(!data) perror("__Impossible__ since dequeue() blocks when empty!\n");
    fprintf(stderr, "%d - %s%s\n", i, data->a, data->b);
  }

  // Here, still 500 nodes remain in the queue.
  //  myX_destroy will be called to free individual nodes in the queue.
  queue_destroy(q, &myX_destroy);
  q = NULL;

  return 0; // 0 indicates success
}

