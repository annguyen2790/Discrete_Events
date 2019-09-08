#include <stdlib.h>
#include <stdio.h>
#include "process_struct.h"
#include <string.h>
float SEED  = 3; 
float INIT_TIME = 0;
float FIN_TIME = 1000;
float ARRIVE_MIN = 25;
float ARRIVE_MAX = 125;
float QUIT_PROB = 0.2;
float CPU_MIN = 25;
float CPU_MAX = 125;
float DISK1_MIN = 25;
float DISK1_MAX = 125;
float DISK2_MIN = 35;
float DISK2_MAX = 125;
  
/*Open/Close state of CPU, DISK 1, and DISK 2
At the beginning of the program, all are opened  */
int CPU_OPEN = 1;
int DISK_1_OPEN = 1;
int DISK_2_OPEN = 1;

//State of jobs
int stimulation_ended = 0;
int entered_disk_1 =  1;
int entered_disk_2 = 2;
int exit_disk_1 = 3;
int exit_disk_2 = 4;
int entered_CPU = 5;
int exit_CPU = 6;
int arrival = 7;
int quit = 8;

Queue * init_queue(){ /*Create a queuse / pointer to Queue struct */
  Queue * s_queue = (Queue *) malloc( sizeof(Queue));
  s_queue ->size = 0;
  s_queue ->head = NULL;
  s_queue ->tail = NULL;
  return s_queue;
}


void insert_Pqueue(Queue * q, Job * event){ /*This method insert Job in a queue with a priority based on the time of the job*/
  q->size++;
  Job * temp = malloc(sizeof(Job));
  Job * temp2 = malloc(sizeof(Job));
  temp->ID = event->ID;
  temp->state = event->state;
  temp->time = event->time;
  if(q -> head == NULL ||temp->time  <  q->head->time){
    temp->nextPtr = q->head;
    q->head = temp;   
  }else{
    temp2 = q->head;
    while(temp2->nextPtr != NULL && temp2->nextPtr->time <= event->time){
      temp2 = temp2->nextPtr;
    }
    temp->nextPtr = temp2->nextPtr;
    temp2->nextPtr = temp;
  }
  q->size++;
  
}
void insert_queue(Queue * q, Job * event){ /*This method insert a Job at the tail of the queue*/
  q->size++;
  Job * temp = malloc(sizeof(Job));
  temp->ID = event->ID;
  temp->state = event->state;
  temp->time = event->time;
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
  q->size--;
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
  size_t i = 0;
  size_t j = 0;
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
Job * create_job(int job_id, int job_state, int job_time){
  Job * temp = malloc(sizeof(Job));
  temp->ID = job_id;
  temp->state = job_state;
  temp->time = job_time;
  return temp;
}

int get_random(int high, int low){ //get a random in-between high and low range
  return ( rand() % (high - low + 1)) + low;
}
void enter_disk(Queue * disk_queue, Queue * event_queue, int disk, int time){
  
  Job * toGo = delete_head(disk_queue);
  insert_Pqueue(event_queue, toGo);
  if(disk == 1){
    DISK_1_OPEN = 0;
    int time_at = time + get_random(DISK1_MAX, DISK1_MIN); //the time the job enter disk
  }
  if(disk == 2){
    DISK_2_OPEN = 0;
    int time_at = time + get_random(DISK2_MAX, DISK2_MIN); //the time the job enter disk
  }
  
}
void enter_CPU(Queue * event_queue, Job * toGo, int time){
  int time_at = time + get_random(CPU_MAX, CPU_MIN);
  if(time_at < FIN_TIME){
    CPU_OPEN = 0;
    toGo->state = 5;
    toGo->time = time;
    insert_Pqueue(event_queue, toGo);
    
  }
}
 int main(void){
   //Read in the inputs from the inputs.txt
  float SEED  = read_inputs("inputs.txt", "SEED"); 
  float INIT_TIME = read_inputs("inputs.txt","INIT_TIME");
  float FIN_TIME = read_inputs("inputs.txt","FIN_TIME");
  float ARRIVE_MIN = read_inputs("inputs.txt","ARRIVE_MIN");
  float ARRIVE_MAX = read_inputs("inputs.txt","ARRIVE_MAX");
  float QUIT_PROB = read_inputs("inputs.txt","QUIT_PROB");
  float CPU_MIN = read_inputs("inputs.txt","CPU_MIN");
  float CPU_MAX = read_inputs("inputs.txt","CPU_MAX");
  float DISK1_MIN = read_inputs("inputs.txt","DISK1_MIN");
  float DISK1_MAX = read_inputs("inputs.txt","DISK1_MAX");
  float DISK2_MIN = read_inputs("inputs.txt","DISK2_MIN");
  float DISK2_MAX = read_inputs("inputs.txt","DISK2_MAX");
  //Create Four Main Queues Data Structures 
  Queue * CPU_queue = init_queue();
  Queue * DISK_1_queue = init_queue();
  Queue * DISK_2_queue = init_queue();
  Queue * EVENTS_queue = init_queue();

  //Set the start condition
  srand(SEED);// randomize
  int ID = 1;
  int arrival_time = INIT_TIME;
  int current_time = INIT_TIME + get_random(ARRIVE_MAX, ARRIVE_MIN);
  Job * job1 = create_job(ID, 5, INIT_TIME); //5 since everything must start at CPU Queue
  insert_queue(CPU_queue, job1);


  while(current_time != FIN_TIME){
    
      Job * new_job = create_job(++ID, 5, current_time);
      insert_queue(CPU_queue, new_job);
      arrival_time = current_time + get_random(ARRIVE_MAX, ARRIVE_MIN); //randomize the arrival time again for next job
      
    
      current_time++;
    
  }
  //print_queue(CPU_queue);
  
  //print_queue(CPU_queue);
  /*
``1. Create the first job
  2. Add a job into the event Queue
  3. 
   */  
  
  
}
