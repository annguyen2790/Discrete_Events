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
  if(event->size == 0){
    return 0;
  }
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
  temp->ID = job_id;
  temp->state = job_state;
  temp->time = job_time;
  return temp;
}

int get_random(int high, int low){ //get a random in-between high and low range
  return ( rand() % (high - low + 1)) + low;
}
void enter_CPU(Job * toGo, Queue * EVENTS_queue, int system_time, int status){
  int time_spent = system_time + get_random(CPU_MAX, CPU_MIN);
  if(time_spent < FIN_TIME){
    status = 0;
    toGo->time = time_spent;
    toGo->state = FINISH_CPU;
    insert_Pqueue(EVENTS_queue, toGo);
    printf("Job%d arrive at CPU at %d\n", toGo->ID, system_time);
    printf("Job%d leave the  CPU at %d\n", toGo->ID, time_spent);
  }
}
void enter_disk(Queue * disk, Queue * EVENTS_queue, int system_time, int disk_choice, int status){
  Job * toGo = delete_head(disk);
  int time_spent = system_time;
  switch(disk_choice){
  case(IN_DISK1_QUEUE):{
    time_spent += get_random(DISK1_MAX, DISK1_MIN);
    status = 0;
    printf("Job%d arrive at DISK 1 at %d\n", toGo->ID, system_time);
    printf("Job%d leave DISK 1  at %d\n", toGo->ID, time_spent);
  }
    break;
  case(IN_DISK2_QUEUE):{
    time_spent += get_random(DISK1_MAX, DISK1_MIN);
    status = 0;
    printf("Job%d arrive at DISK 1 at %d\n", toGo->ID, system_time);
    printf("Job%d leave DISK 1  at %d\n", toGo->ID, time_spent);
  }
    break;
  default:
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
  //Idle Status
  /*Open/Close state of CPU, DISK 1, and DISK 2
  At the beginning of the program, all are opened  */
  int CPU_OPEN = 1;
  int DISK_1_OPEN = 1;
  int DISK_2_OPEN = 1;
  //Set the start condition
  srand(SEED);// randomize
  int ID = 0;
  int system_time = INIT_TIME;
  int arrival_time = INIT_TIME + get_random(ARRIVE_MAX, ARRIVE_MIN);
  int quit_chance = 10;
  int num_job = 0; //Jobs in EVENTS QUEUE
  //Create begin job
  Job * begin = create_job(ID,ARRIVAL, system_time);
  Job * end = create_job(1000, FINISH, FIN_TIME);
  insert_queue(EVENTS_queue, begin);
  insert_Pqueue(EVENTS_queue, end);
  //FILES TO WRITE
  FILE * fp;
  fp = fopen("outputs.txt", "w+");
  
  //BIG YOSHI LOOP
  while(system_time < FIN_TIME && EVENTS_queue->size){
    Job * temp = create_job(++ID, ARRIVAL, system_time);
    switch(temp->state){
      case(ARRIVAL):{
	printf("Job%d arrive at  %d\n", temp->ID, temp->time);
	fprintf(fp,"Job%d arrive at  %d\n", temp->ID, temp->time);
	insert_queue(CPU_queue, temp);
	temp->ID = ID++;
	temp->state = ARRIVE_CPU;
	temp->time = system_time + get_random(ARRIVE_MAX, ARRIVE_MIN);
	printf("Job%d arrive at CPU at  %d\n", temp->ID, temp->time);
	fprintf(fp,"Job%d arrive at CPU at  %d\n", temp->ID, temp->time );
	insert_Pqueue(EVENTS_queue, temp);
      }
      case(ARRIVE_CPU):{
	if(quit_chance < QUIT_PROB * 50){ //10% to quit
	  printf("Job%d exit the CPU at time %d\n ", temp->ID, temp->time);
	  fprintf(fp,"Job%d exit the CPU at time %d\n ", temp->ID, temp->time );
	  temp->state = FINISH_CPU;
	}else{ //enter disk
	  temp->time += get_random(DISK1_MAX, DISK1_MIN);
	  printf("Job%d arrive at disk at  %d\n", temp->ID, temp->time);
	  fprintf(fp,"Job%d arrive at disk at  %d\n", temp->ID, temp->time);
	  temp->state = ARRIVE_DISK;
	  insert_queue(DISK1_queue, temp);
	}
      }
      case(ARRIVE_DISK):{
	srand(SEED);
	int dice_roll = get_random(1, 6);
	if(dice_roll < 4){
	  printf("Job%d finish at disk 1 at  %d\n", temp->ID, temp->time);
	  fprintf(fp,"Job%d finish at disk 1 at  %d\n", temp->ID, temp->time);
	  temp->state = FINISH_DISK1;
	  temp->time = system_time + get_random(DISK1_MAX, DISK1_MIN);
	}else{
	  printf("Job%d finish at disk 2  at  %d\n", temp->ID, temp->time);
	  fprintf(fp,"Job%d finish at disk 2 at  %d\n", temp->ID, temp->time);
	  temp->state = FINISH_DISK2;
	  temp->time = system_time + get_random(DISK2_MAX, DISK2_MIN);
	  
	}
      }
      case(FINISH_DISK1):{
	temp->state = ARRIVE_CPU;
	temp->time = system_time + get_random(DISK1_MAX, DISK2_MAX);
	break;
      }
	
       
      default:
	break;

    }
    system_time++;
  }     
    
}
  
  



























  /* /    
  // print_queue(CPU_queue);






   



//printf("CHEKC: %d\n", system_time);


// PRINT_QUEUE(CPU_QUEUE);




//printf("CHEKC: %d\n", system_time);






// print_queue(EVENTS_queue);*/
/*
while(system_time < FIN_TIME && EVENTS_queue->size){
    if(arrival_time == system_time){
      Job * new_job = create_job(++ID, ARRIVE_CPU, system_time);
      printf("Job%d arrives at %d\n ", new_job->ID, new_job->time);
      insert_queue(CPU_queue, new_job);
      arrival_time = system_time + get_random(ARRIVE_MAX, ARRIVE_MIN);
    }

    if(CPU_OPEN && CPU_queue->size){
      Job * toGo = delete_head(CPU_queue);
      enter_CPU(toGo, EVENTS_queue, system_time, CPU_OPEN);
    }
    if(system_time == EVENTS_queue->head->time){
      num_job = getnumJob(EVENTS_queue, system_time);
      Job * toHandled[num_job];
      for(size_t i = 0; i < num_job; i++){
	toHandled[i] = delete_head(EVENTS_queue);
      }
      size_t j = 0;
      while(j < num_job){
	switch(toHandled[j]->state){
	  case(ARRIVE_CPU):{
	    printf("Job%d leave the  CPU at %d\n", toHandled[j]->ID, system_time);
	    if(quit_chance < QUIT_PROB * 10){
	      toHandled[j]->time = system_time + 1;
	      toHandled[j]->state = EXIT_CPU;
	      printf("Job%d quit the  CPU at %d\n", toHandled[j]->ID, toHandled[j]->time);
	    }
	    else if(CPU_OPEN && CPU_queue->size != 0){
	      Job * toGo = delete_head(CPU_queue);
	      enter_CPU(toGo, EVENTS_queue, system_time, CPU_OPEN);
	      
	    }
	    else{ //enter the DISK
	      if(DISK1_queue->size > DISK2_queue->size){ //enter disk 2
		toHandled[j]->state = IN_DISK2_QUEUE;
		insert_queue(DISK2_queue, toHandled[j]);
		if(DISK_2_OPEN != 0){
		  enter_disk(DISK2_queue, EVENTS_queue, system_time, IN_DISK2_QUEUE, DISK_2_OPEN );
		}
	      }
	      else if(DISK1_queue->size < DISK2_queue->size){ //enter disk 1
		toHandled[j]->state = IN_DISK1_QUEUE;
		insert_queue(DISK1_queue, toHandled[j]);
		if(DISK_1_OPEN != 0){
		  enter_disk(DISK2_queue, EVENTS_queue, system_time, IN_DISK1_QUEUE, DISK_1_OPEN );
		}
	      }else{
		int dice_roll = get_random(7,1);
		if(dice_roll < 4){
		  toHandled[j]->state = IN_DISK1_QUEUE;
		  insert_queue(DISK1_queue, toHandled[j]);
		  if(DISK_1_OPEN != 0){
		    enter_disk(DISK1_queue, EVENTS_queue, system_time, IN_DISK2_QUEUE, DISK_1_OPEN);
		  }
		}
		else{
		  toHandled[j]->state = IN_DISK2_QUEUE;
		  insert_queue(DISK1_queue, toHandled[j]);
		  if(DISK_2_OPEN != 0){
		    enter_disk(DISK1_queue, EVENTS_queue, system_time, IN_DISK2_QUEUE, DISK_2_OPEN);
		  }
		}
	      }
	    }
	    break;
	  }
	    
	case(ARRIVE_DISK1):{
	  printf("Job%d leaves disk 1 at %d time", toHandled[j]->ID, toHandled[j]->time);
	  DISK_1_OPEN = 1;
	  toHandled[j]->state = ARRIVE_CPU_QUEUE;
	  insert_queue(CPU_queue, toHandled[j]);
	  if(CPU_OPEN != 0){
	    Job * toGo = delete_head(CPU_queue);
	    enter_CPU(toGo, CPU_queue, system_time, CPU_OPEN);
	  }
	  if(DISK1_queue ->size != 0){
	    enter_disk(DISK1_queue, EVENTS_queue, system_time, IN_DISK1_QUEUE, DISK_1_OPEN);
	  }
	  break;
	}

	  case(ARRIVE_DISK2):{
	  printf("Job%d leaves disk 2 at %d time", toHandled[j]->ID, toHandled[j]->time);
	  DISK_2_OPEN = 1;
	  toHandled[j]->state = ARRIVE_CPU_QUEUE;
	  insert_queue(CPU_queue, toHandled[j]);
	  if(CPU_OPEN != 0){
	    Job * toGo = delete_head(CPU_queue);
	    enter_CPU(toGo, EVENTS_queue, system_time, CPU_OPEN);
	  }
	  if(DISK2_queue ->size != 0){
	    enter_disk(DISK2_queue, EVENTS_queue, system_time, IN_DISK2_QUEUE, DISK_2_OPEN);
	  }
	  break;
	  }
	default:
	  break;

	j++;
	}
      }
      
      
    }
    
    system_time++;
  }
*/
