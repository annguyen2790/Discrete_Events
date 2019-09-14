#include <stdlib.h>
#include <stdio.h>
#include "process_struct.h"
#include <string.h>


#define ARRIVAL 1
#define ARRIVE_CPU 2
#define FINISH_CPU 3
#define ARRIVE_DISK 4
#define FINISH_DISK1 5
#define FINISH_DISK2 6
#define FINISH 7
//Use for testing only!
float SEED  = 3;
float INIT_TIME = 0;
float FIN_TIME = 1000;
float ARRIVE_MIN = 0;
float ARRIVE_MAX = 12;
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
int system_time;
int ID;
//State of jobs


Queue * init_queue(){ /*Create a queuse / pointer to Queue struct */
  Queue * s_queue = (Queue *) malloc( sizeof(Queue));
  s_queue ->size = 0;
  s_queue ->head = NULL;
  s_queue ->tail = NULL;
  return s_queue;
}
Queue * CPU_queue;
Queue * DISK1_queue;
Queue * DISK2_queue;
Queue * EVENTS_queue;


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
  
  Job * temp = malloc(sizeof(Job));
  temp->ID = event->ID;
  temp->state = event->state;
  temp->time = event->time;
  temp->nextPtr = NULL;

  if(q->head == NULL && q->tail == NULL){
    q->head = q->tail =  temp;

  }else{
    q->tail->nextPtr = temp;
    q->tail = temp;
  }
  q->size++;
}
Job * delete_head(Queue * q){ /*Return reference to to head Job that popped*/
  if(isEmpty(q)){
    return NULL;
  }
  q->size--;
  Job * holder = (Job *)malloc(sizeof(Job));
  holder = q->head;
  holder->ID = q->head->ID;
  holder->state = q->head->state;
  holder->time = q->head->time;
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
void print_job(Job * toPrint){
    printf("ID:%d ", toPrint->ID);
    printf("State:%d ", toPrint->state);
    printf("Time:%d", toPrint->time);
}
int isEmpty(Queue * q){ /*This method checks if the queue is empty; 1 for yes and 0 for no */
  return q->head == NULL && q->size == 0;
}
void destroy_queue(Queue * q){
  q->size = 0;
  free(q);
}
int getnumJob(Queue *event, int time){
  Job * temp = event->head;
  int count = 0;
  while(temp->time == time){
    count++;
    temp = temp->nextPtr;
  }
  return count;
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
      for(i = strlen(string); i < strlen(temp) + 1; i++ ,j++){ //convert number  after the space  the search string to the end null  into number
	string_holder[j] = temp[i];

      }
      value = atof(string_holder);
    }
  }
  fclose(filePtr);
  return value;
}
Job  * create_job(int job_id, int job_state, int job_time){
  Job * temp = malloc(sizeof(Job));
  temp->ID = ++job_id;
  temp->state = job_state;
  temp->time = job_time;
  return temp;
}

int get_random(int high, int low){ //get a random in-between high and low range
  return ( rand() % (high - low + 1 + 1)) + low;
}
void enter_CPU(Job * toGo, Queue * EVENTS_queue){
  if(CPU_OPEN){
    CPU_OPEN = 0;
    int arrived_time = toGo->time;
    toGo->time = arrived_time + get_random(CPU_MAX, CPU_MIN);
    toGo->state = FINISH_CPU;
    insert_Pqueue(EVENTS_queue, toGo);

  }else{
    insert_queue(CPU_queue, toGo);
  }
}
void exit_CPU(Job * toGo, Queue * CPU_queue, Queue * EVENTS_queue){
  CPU_OPEN = 0;
  if(!isEmpty(CPU_queue)){
    Job * toPop = delete_head(CPU_queue);
    enter_CPU(toPop, EVENTS_queue);
  }
  if(get_random(10,1) <= QUIT_PROB * 10){
    printf("Job%d exit CPU at time %d\n ", toGo->ID, toGo->time);
  }else{
    toGo->state = ARRIVE_DISK;
    insert_Pqueue(EVENTS_queue, toGo);
  }
}
void enter_disk1(Job * toGo, Queue * DISK1_queue){
  printf("Job%d arrive at disk at %d time\n", toGo->ID, toGo->time);
  //toGo->state = FINISH_DISK1;
  if(DISK_1_OPEN){
    DISK_1_OPEN = 0;
    toGo->time = system_time + get_random(DISK1_MAX, DISK1_MIN);
    toGo->state = FINISH_DISK1;
  }else{
    insert_queue(DISK1_queue, toGo);
  }
}
void enter_disk2(Job * toGo, Queue * DISK2_queue){
  printf("Job%d finish at disk 2 at %d time\n", toGo->ID, toGo->time);
  if(DISK_2_OPEN){
    DISK_2_OPEN = 0;
    toGo->time = system_time + get_random(DISK2_MAX, DISK2_MIN);
    toGo->state = FINISH_DISK2;
  }else{
    insert_queue(DISK2_queue, toGo);
  }
}

void exit_disk(Job * toGo, Queue * EVENTS_queue){
  printf("Job%d finish at disk at %d time\n", toGo->ID, toGo->time);
  toGo->state = ARRIVE_CPU;
  insert_Pqueue(EVENTS_queue, toGo);
}
void exit_DISK1(Job * toGo, Queue * DISK1_queue, Queue * EVENTS_queue){
  printf("Job%d finish at disk 1 at %d time\n", toGo->ID, toGo->time);
  DISK_1_OPEN = 1;
  if(!isEmpty(DISK1_queue)){
    Job * handled = delete_head(DISK1_queue);
    enter_disk1(handled, DISK1_queue);
  }
  exit_disk(toGo, EVENTS_queue);

}
void exit_DISK2(Job * toGo, Queue * DISK2_queue, Queue * EVENTS_queue){
  printf("Job%d finish at disk 2 at %d time\n", toGo->ID, toGo->time);
  DISK_2_OPEN = 1;
  if(!isEmpty(DISK2_queue)){
    Job * handled = delete_head(DISK2_queue);
    enter_disk2(handled, DISK2_queue);
  }
  exit_disk(toGo, EVENTS_queue);

}
//insert_Pqueue(event_queue, toGo);

void process_arrival(Job * toGo, Queue * EVENTS_queue, int system_time){
  Job * temp= toGo;
  temp = create_job(++ID, 1, system_time);
  toGo->time = system_time + get_random(ARRIVE_MAX, ARRIVE_MIN);
  toGo->state = ARRIVE_CPU;
  insert_Pqueue(EVENTS_queue, toGo);


}

void event_handler(Job * toGo, Queue * EVENTS_queue, Queue * DISK1_queue, Queue * DISK2_queue, Queue * CPU_queue, int time){
  switch(toGo->state){
  case ARRIVAL:
    process_arrival(toGo, EVENTS_queue,  time);
    printf("Job%d arrive at time %d\n", toGo->ID, toGo->time);
    break;
  case ARRIVE_CPU:
    printf("Job%d arrive at CPU at %d time\n", toGo->ID, toGo->time);
    enter_CPU(toGo, EVENTS_queue);
    break;
  case FINISH_CPU:
    printf("Job%d leave CPU at %d time\n", toGo->ID, toGo->time);
    exit_CPU(toGo, CPU_queue, EVENTS_queue);
    break;
  case ARRIVE_DISK:
    enter_disk1(toGo, DISK1_queue);
    break;
  case FINISH_DISK1:
    printf("Job%d finish at disk 1 at %d time\n", toGo->ID, toGo->time);
    exit_DISK1(toGo, DISK1_queue, EVENTS_queue);
    break;
  case FINISH_DISK2:
    printf("Job%d finish at disk 2 at %d time\n", toGo->ID, toGo->time);
    exit_DISK2(toGo, DISK2_queue, EVENTS_queue);
    break;
  case FINISH:
    printf("Job%d end the simulation at time %d\n", toGo->ID, toGo->time);
    break;
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
  Queue * DISK1_queue = init_queue();
  Queue * DISK2_queue = init_queue();
  Queue * EVENTS_queue = init_queue();

  //Set the start condition
  srand(SEED);// randomize
  int ID = 0;
  int system_time = INIT_TIME;
  int quit_chance = 10;
  Job * first_job = create_job(ID,ARRIVAL, INIT_TIME);
  Job * first_job1 = create_job(ID,ARRIVAL, INIT_TIME);
  Job * first_job2 = create_job(ID,ARRIVAL, INIT_TIME);
  Job * end_job = create_job(1000,FINISH, FIN_TIME );
  //Job * cpu_job  = create_job(++ID,ARRIVE_CPU, system_time);
  //Job * disk1_job = create_job(++ID,ARRIVE_DISK, system_time);
  //Job * disk2_job= create_job(++ID,ARRIVE_DISK, system_time);
  insert_queue(EVENTS_queue, first_job);
  insert_queue(EVENTS_queue, first_job1);
  insert_queue(EVENTS_queue, first_job2);
  insert_Pqueue(EVENTS_queue, end_job);
  int check = getnumJob(EVENTS_queue, INIT_TIME);
  printf("%d\n", check);
  //insert_queue(CPU_queue, first_job);
  //print_queue(CPU_queue);
  //insert_queue(DISK1_queue, disk1_job);
  //insert_queue(DISK2_queue, disk2_job);
  //print_queue(DISK1_queue);
  
  /* while(EVENTS_queue ->size && system_time < FIN_TIME){
    Job * temp = create_job(++ID, ARRIVAL, system_time);
    switch(temp->state){
       case(ARRIVAL):{
	 printf("Job%d arrive at  %d\n", temp->ID, temp->time);
         insert_queue(CPU_queue, temp);
	 temp->ID = ID++;
         temp->state = ARRIVE_CPU; 
	 temp->time = system_time + get_random(ARRIVE_MAX, ARRIVE_MIN); 
	 printf("Job%d arrive at CPU at  %d\n", temp->ID, temp->time);
         insert_Pqueue(EVENTS_queue, temp);
       }
	 break;
       case(ARRIVE_CPU):{
	 if(quit_chance < QUIT_PROB * 50){ //10% to quit
	   printf("Job%d exit the CPU at time %d\n ", temp->ID, temp->time);
	   temp->state = FINISH_CPU;
          }else{ //enter disk
	   temp->time += get_random(DISK1_MAX, DISK1_MIN);
	   printf("Job%d arrive at disk at  %d\n", temp->ID, temp->time);
	   temp->state = ARRIVE_DISK;
	   insert_queue(DISK1_queue, temp);
 	   
	  }
       }
	 break;
        case(ARRIVE_DISK):{
	  printf("Job%d finish at disk at  %d\n", temp->ID, temp->time);
	  temp->state = FINISH_DISK1;
	  temp->time = system_time + get_random(DISK1_MAX, DISK2_MAX);
        }
	  break;
        case(FINISH_DISK1):{
	  temp->state = ARRIVE_CPU;
	  temp->time = system_time + get_random(DISK1_MAX, DISK2_MAX);
        }
	  break;
        default:
          break;
      
      
    }
    system_time = temp->time;
 
    }*/    
  // print_queue(CPU_queue);






   

}


//printf("CHEKC: %d\n", system_time);


// PRINT_QUEUE(CPU_QUEUE);




//printf("CHEKC: %d\n", system_time);






// print_queue(EVENTS_queue);*/

