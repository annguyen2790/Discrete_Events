#include <stdlib.h>
#include <stdio.h>
#include "process_struct.h"
#include <string.h>
/*Global Variables  */
int INIT_TIME = 0;
int FIN_TIME = 1000;
int ARRIVE_MIN;
int ARRIVE_MAX;
int QUIT_PROB;
int CPU_MIN = 0;
int CPU_MAX;
int DISK1_MIN;
int DISK1_MAX;
int DISK2_MIN;
int DISK2_MAX;
/*END OF GLOBAL VARIABLES */

/*Open/Close state of CPU, DISK 1, and DISK 2
At the beginning of the program, all are opened  */
int CPU_OPEN = 1;
int DISK_1_OPEN = 1;
int DIS_2_OPEN = 1;


//State of jobs


int stimulation_ended = 0;
int entered_disk_1 =  1;
int entered_disk_2 = 2;
int exit_disk_1 = 3;
int exit_disk_2 = 4;
int entered_CPU = 5;
int exit_CPU = 6;

Queue * init_queue(){ /*Create a queuse / pointer to Queue struct */
  Queue * s_queue = (Queue *) malloc( sizeof(Queue));
  s_queue ->size = 0;
  s_queue ->head = NULL;
  s_queue ->tail = NULL;
  return s_queue;
}
void insert_Pqueue(Queue * q, int job_ID, int job_state, int job_time){ /*This method insert Job in a queue with a priority based on the time of the job*/
  Job * temp = malloc(sizeof(Job));
  Job * temp2 = malloc(sizeof(Job));
  temp->ID = job_ID;
  temp->state = job_state;
  temp->time = job_time;
  if(q -> head == NULL ||temp->time  <  q->head->time){
    temp->nextPtr = q->head;
    q->head = temp;   
  }else{
    temp2 = q->head;
    while(temp2->nextPtr != NULL && temp2->nextPtr->time <= job_time){
      temp2 = temp2->nextPtr;
    }
    temp->nextPtr = temp2->nextPtr;
    temp2->nextPtr = temp;
  }
  q->size++;
  
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
  q->size++;
}
Job * delete_head(Queue * q){ /*Return reference to to head Job that popped*/
  if(isEmpty(q)){
      return NULL;
  }
  Job * holder = malloc(sizeof(Job));
  holder = q->head;
  q->head = holder->nextPtr;
  q->size--;
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
  printf("Size: %d\n", q->size);
  
}
int isEmpty(Queue * q){ /*This method checks if the queue is empty; 1 for yes and 0 for no */
  return q->head == NULL && q->size == 0;
}
void destroy_queue(Queue * q){
  q->size = 0;
  free(q);
}
    
//arrival, cpu_enter, cpu_finished, process_exit, disk1_start, disk1_end, disk2_start, disk2_end, ending

float read_inputs(char *file_name, char *string){ /*This method will read and search for value coresspond to the search string and return that value*/ 
  FILE *filePtr;  
  char temp[200];         
  char string_holder[200]; 
  float value = 0;
  int i = 0;
  int j = 0;
    
  if((filePtr = fopen(file_name, "r")) == NULL){
    return(0);
  }
  while(fgets(temp,200, filePtr) != NULL){
    if((strstr(temp, string)) != NULL){
      for(i = strlen(string); i < strlen(temp) + 1; i++){ //convert number  after the space  the search string to the end null  into number
	string_holder[j] = temp[i];
	j++;
      }
      value = atof(string_holder);
      
            
    }
        
  }
  fclose(filePtr);
  return value;
    
}
 int main(void){

  //Create Four Main Queues Data Structures 
  Queue * CPU_queue = init_queue();
  Queue * DISK_1_queue = init_queue();
  Queue * DISK_2_queue = init_queue();
  Queue * EventQueue = init_queue();
  float check = read_inputs("inputs.txt", "QUIT_PROB");
  printf("%f\n", check);
}
