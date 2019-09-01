#include <stdlib.h>
#include <stdio.h>
#include "process_struct.h"

/*Global Variables  */
int INIT_TIME = 0;
int FIN_TIME = 1000;
int ARRIVE_MIN;
int ARRIVE_MAX;
int QUIT_PROB;
int CPU_MIN;
int CPU_MAX;
int DISK1_MIN;
int DISK1_MAX;
int DISK2_MIN;
int DISK2_MAX;
/*END OF GLOBAL VARIABLES */


Queue * init_queue(){ /*Create a queuse / pointer to Queue struct */
  Queue * s_queue = (Queue *) malloc( sizeof(Queue));
  s_queue ->size = 0;
  s_queue ->head = NULL;
  s_queue ->tail = NULL;
  return s_queue;
}

void insert_queue(Queue * q, int job_ID, int job_state, int job_time){ /*This method insert a Job at the tail of the queue*/
  Job * temp = malloc(sizeof(Job));
  temp->ID = job_ID;
  temp->state = job_state;
  temp->time = job_time;
  temp->nextPtr = NULL;
  
  if(q->head == NULL){
    q->head = temp;
  }else{
    q->tail->nextPtr = temp;
  }

  q->tail = temp;
  
}
Job * delete_head(Queue * q){ /*Return reference to to head Job that popped*/
  if(isEmpty(q)){
      return NULL;
  }
  Job * holder = malloc(sizeof(Job));
  holder = q->head;
  q->head = holder->nextPtr;
  
  return holder;
    
}
void print_queue(Queue *  q){ /*This methof is just used to test if queue is working*/
  Job * temp = q->head;
  while(temp != NULL){
    printf("ID:%d ", temp->ID);
    printf("State:%d ", temp->state);
    printf("Time:%d", temp->time);
    printf("---->");
    temp = temp->nextPtr;
  }
  printf("NULL\n");
  
}
int isEmpty(Queue * q){ /*This method checks if the queue is empty; 1 for yes and 0 for no */
  return q->head == NULL;
}
int main(void){
  Queue * q = init_queue();
  insert_queue(q, 1, 2, 3);
  insert_queue(q, 3, 4, 5);
  int check = isEmpty(q);
  Job * thrown = delete_head(q);
  printf("%d", thrown->ID );
  print_queue(q);
}
