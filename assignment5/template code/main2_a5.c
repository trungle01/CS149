#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>


//thread mutex lock for access to the log index
pthread_mutex_t tlock1 = PTHREAD_MUTEX_INITIALIZER;
//thread mutex lock for critical sections of allocating THREADDATA
pthread_mutex_t tlock2 = PTHREAD_MUTEX_INITIALIZER; 


void* thread_runner(void*);
pthread_t tid1, tid2;
struct THREADDATA_STRUCT
{
  pthread_t creator;
};
typedef struct THREADDATA_STRUCT THREADDATA;

THREADDATA* p=NULL;


//variable for indexing of messages by the logging function
int logindex=0;
int *logip = &logindex;


//A flag to indicate if the reading of input is complete, 
//so the other thread knows when to stop
bool is_reading_complete = false;


/*********************************************************
// function main  ------------------------------------------------- 
*********************************************************/
int main()
{

  printf("create first thread");
  pthread_create(&tid1,NULL,thread_runner,NULL);
  
  printf("create second thread");
  pthread_create(&tid2,NULL,thread_runner,NULL);
  
  printf("wait for first thread to exit");
  pthread_join(tid1,NULL);
  printf("first thread exited");

  printf("wait for second thread to exit");
  pthread_join(tid2,NULL);
  printf("second thread exited");

  exit(0);

}//end main


/**********************************************************************
// function thread_runner runs inside each thread -------------------------------------------------- 
**********************************************************************/
void* thread_runner(void* x)
{
  pthread_t me;

  me = pthread_self();
  printf("This is thread %ld (p=%p)",me,p);
  
  pthread_mutex_lock(&tlock2); // critical section starts
  if (p==NULL) {
    p = (THREADDATA*) malloc(sizeof(THREADDATA));
    p->creator=me;
  }
  pthread_mutex_unlock(&tlock2);  // critical section ends

  if (p!=NULL && p->creator==me) 
    printf("This is thread %ld and I created the THREADDATA %p",me,p);
  else
    printf("This is thread %ld and I can access the THREADDATA %p",me,p);

  // TODO use mutex to make this a start of a critical section 
  if (p!=NULL && p->creator==me)
    printf("This is thread %ld and I didnot touch THREADDATA",me);
  else{
  /**
   * TODO Free the THREADATA object. Freeing should be done by the other thread from the one that created it.
   * See how the THREADDATA was created for an example of how this is done.
   */
    printf("This is thread %ld and I deleted the THREADDATA",me);
  }
  // TODO critical section ends

  pthread_exit(NULL);
  return NULL;

}//end thread_runner

