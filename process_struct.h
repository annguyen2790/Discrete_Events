#define IN_CPU_QUEUE 0
#define IN_CPU 1
#define IN_DISK1 2
#define IN_DISK2 3
#define IN_DISK1_QUEUE 4
#define IN_DISK2_QUEUE 5
#define CPU_QUEUE 7




typedef struct Job{
  int ID;
  int state;
  int time;
  struct Job * nextPtr;
} Job;

typedef struct Queue{
  Job * head;
  Job * tail;
  int size;
}Queue;

Queue * init_queue();
void insert_queue(Queue * q, Job * event);
void printQueue(Queue * q);
int isEmpty(Queue *q);
Job * delete_head(Queue * q);
void insert_Pqueue(Queue * q, Job * event);
void destroy_queue(Queue * q);
float read_inputs(char * filename, char * string);
int get_random(int high, int low);
int get_numJob(Queue * q, int num);

