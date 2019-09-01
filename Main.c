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

/*Global pointer to start and end of the queue*/
struct Job * head = NULL;
struct Job * tail = NULL;

Queue * init_queue(){
  Queue * s_queue = (Queue *) malloc( sizeof(Queue));
  s_queue ->size = 0;
  s_queue ->head = NULL;
  s_queue ->tail = NULL;
  return s_queue;
}

void insert_queue(Queue * q, int job_ID, int job_state, int job_time){
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
void print_queue(Queue *  q){
  Job * temp = q->head;
  while(temp != NULL){
    printf("%d", temp->ID);
    printf("%d", temp->state);
    printf("%d", temp->time);
    printf("---->");
    temp = temp->nextPtr;
  }
  printf("NULL\n");
  
}
int main(void){
  Queue * q = init_queue();
  insert_queue(q, 1, 2, 3);
  insert_queue(q, 3, 4, 5);
  print_queue(q);
}
