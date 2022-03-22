/*
 * Assignment 5
 * By: Trung Le and Roshini Malempati
 * Group 30
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include "CommandNode.c"

//thread mutex lock for access to the log index
pthread_mutex_t tlock1 = PTHREAD_MUTEX_INITIALIZER;
//thread mutex lock for critical sections of allocating THREADDATA
pthread_mutex_t tlock2 = PTHREAD_MUTEX_INITIALIZER;
// thread mutext lock for critical sections of setting head node
pthread_mutex_t hlock = PTHREAD_MUTEX_INITIALIZER;


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

// Initialize node for linked list
CommandNode *headNode;
CommandNode *newNode;
// time_t is arithmetic time type
time_t now;
struct tm *local;
// variables to store date and time components
int hours, minutes, seconds, day, month, year;

// set local time from struct tm *local
void setLocalTime(struct tm *local){
    hours = local->tm_hour;      	// get hours since midnight (0-23)
    minutes = local->tm_min;     	// get minutes passed after the hour (0-59)
    seconds = local->tm_sec;     	// get seconds passed after minute (0-59)

    day = local->tm_mday;        	// get day of month (1 to 31)
    month = local->tm_mon + 1;   	// get month of year (0 to 11)
    year = local->tm_year + 1900;	// get year since 1900
}

// method printMessage
// use to print the log message
// lock the logindex everytime print out the log message
void printMessage(char *msg, pthread_t me){
    pthread_mutex_lock(&tlock1);
    logindex++;
    time(&now);
    local = localtime(&now);
    setLocalTime(local);
    // print local time
    if (hours < 12)	// before midday
        printf("Logindex %d, thread %ld, PID %d, %02d/%02d/%d %02d:%02d:%02d am: %s (p = %p)\n",logindex, me, getpid(), day, month, year, hours, minutes, seconds, msg, p);
    else	// before midday
        printf("Logindex %d, thread %ld, PID %d, %02d/%02d/%d %02d:%02d:%02d pm: %s (p = %p)\n",logindex, me, getpid(), day, month, year, hours, minutes, seconds, msg, p);

    pthread_mutex_unlock(&tlock1);
}

// free all nodes in the linked list
void freeLinkedList(CommandNode *headNode){
    if(headNode != NULL)
    {
        freeLinkedList(headNode->nextCommandPtr);
        free(headNode);
    }
}

/*********************************************************
// function main  -------------------------------------------------
*********************************************************/
int main()
{
    printf("create first thread\n");
    pthread_create(&tid1,NULL,thread_runner,NULL);

    printf("create second thread\n");
    pthread_create(&tid2,NULL,thread_runner,NULL);

    printf("wait for first thread to exit\n");
    pthread_join(tid1,NULL);
    printf("first thread exited\n");

    printf("wait for second thread to exit\n");
    pthread_join(tid2,NULL);
    printf("second thread exited\n");

    exit(0);

}//end main


/**********************************************************************
// function thread_runner runs inside each thread --------------------------------------------------
**********************************************************************/
void* thread_runner(void* x)
{
    list_t *L = (list_t *)malloc(sizeof(list_t));
    pthread_t me;
    me = pthread_self();
    // print out first log message
    printMessage("", me);

    pthread_mutex_lock(&tlock2); // critical section starts
    if (p==NULL) {
        p = (THREADDATA*) malloc(sizeof(THREADDATA));
        p->creator=me;
    }
    pthread_mutex_unlock(&tlock2);  // critical section ends

    if (p!=NULL && p->creator==me)
    {
        printMessage("I created the THREADDATA", me);

        // First thread: Read input from user and add to linked list
        int index = 0;
        char *result;
        char buf[100];
        // initialize linked list
        headNode = (CommandNode*)malloc(sizeof(CommandNode));
        CreateCommandNode(headNode, "start", -1, NULL);
        L->head = headNode;
        // get lines from stdin and store in linked list
        while((result = fgets(buf, 100, stdin)) != NULL) {
            if(*result == '\n') break;
            // add new node to head of linked list
            newNode = (CommandNode *)malloc(sizeof(CommandNode));
            CreateCommandNode(newNode, buf, index, NULL);
            pthread_mutex_lock(&hlock);
            InsertCommand(newNode, headNode);
            headNode = newNode;
            pthread_mutex_unlock(&hlock);
            index++;
        }
        is_reading_complete = true;
    }
    else
    {
        printMessage("I can access the THREADDATA", me);
        while(!is_reading_complete)
        {
            sleep(2);
            pthread_mutex_lock(&hlock);
            if(L->head != headNode)
            {
                PrintHeadNode(headNode);
                L->head = headNode;
            }
            pthread_mutex_unlock(&hlock);
        }
    }

    // TODO use mutex to make this a start of a critical section
    pthread_mutex_lock(&tlock2); // critical section starts
    if (p!=NULL && p->creator==me)
    {
        printMessage("I didnot touch THREADDATA", me);
    }
    else{
        printMessage("I deleted the THREADDATA", me);
        // free the THREADDATA
        free(p);
    }
    pthread_mutex_unlock(&tlock2);  // critical section ends
    // TODO critical section ends

    freeLinkedList(L->head);
    free(L);

    pthread_exit(NULL);
    return NULL;

}//end thread_runner

