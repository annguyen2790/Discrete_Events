#include <stdlib.h>
#include <stdio.h>
#include "process_struct.h"
#include <string.h>
//Use for testing only!
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
  
  if(disk == 1){ //in DISK 1
    int time_at = time + get_random(DISK1_MAX, DISK1_MIN); //the time the job enter disk
    if(time_at < FIN_TIME){
      toGo->state = 1;
      toGo->time = time_at;
      DISK_1_OPEN = 0;
      printf("Job%d arrive in disk1 at %d and leave at %d\n", toGo->ID, time,time_at);
    }
    
  }
  if(disk == 2){ //in DISK 2
    int time_at = time + get_random(DISK2_MAX, DISK2_MIN); //the time the job enter disk
    if(time_at < FIN_TIME){
      toGo->state = 2;
      toGo->time = time_at;
      DISK_2_OPEN = 0;
      printf("Job%d arrive in disk2 at %d and leave at %d\n", toGo->ID, time,time_at);
    }
  }
  insert_Pqueue(event_queue, toGo);
}

void enter_CPU(Queue * event_queue, Job * toGo, int time){
  int time_at = time + get_random(CPU_MAX, CPU_MIN);
  if(time_at < FIN_TIME){
    CPU_OPEN = 0;
    toGo->state = 5;
    toGo->time = time;
    insert_Pqueue(event_queue, toGo);
    printf("Job%d arrive in CPU at %d and leave at %d\n", toGo->ID, time,time_at);
  }
  //insert_Pqueue(event_queue, toGo);
}
void handle_event(Job * jobs_in_events[], Queue * EVENTS_queue, Queue * DISK_1_queue, Queue * DISK_2_queue, Queue * CPU_queue, int job_amount, int current_time){
  //declare quit probability
  int quit_chance = get_random(10, 1);
  int i ;
  //for all jobs to be processed, switch case their state to determine what to do
   while(i < job_amount){
     if(jobs_in_events[i]->state == 0){
       printf("Job%d ended simulation at time %d\n", jobs_in_events[i]->ID, jobs_in_events[i]->time );
       printf("Job%d leave CPU at  %d time\n", jobs_in_events[i]->ID, jobs_in_events[i]->time );
     }
     if(quit <= QUIT_PROB * 20){
       jobs_in_events[i] -> state = exit_CPU;

       if(CPU_OPEN){ //when CPU queue is available
	 Job * toGo = delete_head(CPU_queue); 
	 enter_CPU(EVENTS_queue, toGo, current_time); //pop the top job right back to events queue
       }

     }
   
     if(jobs_in_events[i] -> state == 6 ){
       if(DISK_2_queue -> size >= DISK_1_queue ->size){
	 jobs_in_events[i] -> state = entered_disk_1;
	 insert_queue(DISK_1_queue, jobs_in_events[i]);
	 if(DISK_1_OPEN){
	   enter_disk(DISK_1_queue, EVENTS_queue, entered_disk_1, current_time);
	 }
       }
       if(DISK_2_queue -> size <= DISK_1_queue ->size){
	 jobs_in_events[i] -> state = entered_disk_2;
	 insert_queue(DISK_2_queue, jobs_in_events[i]);
	 if(DISK_2_OPEN){
	   enter_disk(DISK_2_queue, EVENTS_queue, entered_disk_2, current_time);
	 }
       }
       CPU_OPEN = 1; 
     }
      
    i++; 
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
  //insert_queue(CPU_queue, job1); //add job to CPU queue
  Job * testJob1 = create_job(++ID, 0, 3 );
  //enter_CPU(EVENTS_queue, testJob1, testJob1->time);
  Job * testJob2 = create_job(++ID, 0, 4);
  insert_queue(DISK_2_queue, testJob2 );
  enter_disk(DISK_2_queue, EVENTS_queue, 2, testJob2->time);
  print_queue(EVENTS_queue);
}










/* while(current_time != FIN_TIME){
    
    if(arrival_time == current_time){
      Job * new_job = create_job(++ID, 5, current_time);
      insert_queue(CPU_queue, new_job);
      arrival_time = current_time + get_random(ARRIVE_MAX, ARRIVE_MIN); //randomize the arrival time again for next job
    }  
    
    
    //check if CPU Open then pop a job from CPU queue to event Queue
     if(CPU_OPEN){
       Job * toGo = delete_head(CPU_queue);
       enter_CPU(EVENTS_queue, toGo, current_time);
     }

    //check the time of job at event queue then handle events according to state
    if(current_time == EVENTS_queue->head->time){
      
      //check how many jobs at event queue
      size_t counter = 0;
      Job * temp = EVENTS_queue->head;
      while(temp->time == current_time){ //how many jobs are the same time 
	counter = counter + 1;
	temp = temp->nextPtr; 
      }
      //create an array of Jobs
      Job * JOBS[counter];
      //fill the array with jobs from event queue
      for(int i = 0; i < counter; i++){
	JOBS[i] = delete_head(EVENTS_queue);
      }
      //handle events accordig to state : 
     }
    current_time++;
  }
  // print_queue(EVENTS_queue);*/
  
