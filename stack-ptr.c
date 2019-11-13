#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


typedef int value_t;
typedef struct Node
{
    value_t data;
    struct Node *next;
} StackNode;

// my add on
pthread_t tid[200];   // create a thread with 200 elements in it
pthread_mutex_t lockPUSHANDPOP;     // lock for push and pop. I used the

int counterPush=0;    // counter for push to test case our code works
int counterPop=0;     // same as above
                      // I used global variables to increment as we implement our push and pop functions globally

// Stack function declarations
void    push    (value_t v, StackNode **top);
value_t pop     (           StackNode **top);
int     is_empty(           StackNode *top);
// my add on

void* testStack(void* arg);
int main(void)
{
    for(int i =0; i<200; i++){
      pthread_create(&tid[i], NULL, &testStack,NULL);
    }
    int error;    // test case to make sure its working correctly
    if(pthread_mutex_init(&lockPUSHANDPOP,NULL) != 0){
      printf("\n FAILED: something went wrong \n");
      return 1;
    }
    for(int i =0; i <200; i++){
      pthread_join(tid[i],NULL);
    }

    // destrory the lock after completes its tasks
  //  pthread_mutex_destory(&lockPUSHANDPOP);
  // Based from what i know mutex lock should be destroyed after finishing its tasks
  // However, I ran into a problem. It complains when i destory them even after
  // i made share thread its joined
    return 0;
}

void push(value_t v, StackNode **top)
{
  	pthread_mutex_lock(&lockPUSHANDPOP);
  	printf("\nPush %d initiated ",counterPush);

    StackNode * new_node = malloc(sizeof(StackNode));

    new_node->data = v;
    new_node->next = *top;
    *top           = new_node;

    printf("\nPush %d terminated",counterPush);
  	pthread_mutex_unlock(&lockPUSHANDPOP);
    counterPush++;

}

value_t pop(StackNode **top)
{
  	pthread_mutex_lock(&lockPUSHANDPOP);
    printf("\nPop %d initiated",counterPop);

    if (is_empty(*top)) return (value_t)0;

    value_t     data  = (*top)->data;
    StackNode * temp  = *top;
    *top              = (*top)->next;

    free(temp);
    printf("\nPush %d terminated",counterPop);
    pthread_mutex_unlock(&lockPUSHANDPOP);
    counterPop++;

    return data;
}

int is_empty(StackNode *top) {
    if (top == NULL) return 1;
    else             return 0;
}
// my add on
/*
I created this testStack that loop five hundread times
and created a instance of StackNode top to pass value from arg
*/
void* testStack(void* arg){


		StackNode *top = arg;

  for(int i= 0; i<500; i++){
    push(5, &top);
    push(10,&top);
    pop (   &top);
    push(15,&top);
    pop (   &top);
    pop (   &top);
    push(20,&top);


    push(-5, &top);
    pop (    &top);
    push(-10,&top);
    pop (    &top);
    pop (    &top);
    push(-15,&top);
    pop (    &top);
    push(-20,&top);
  }
}
