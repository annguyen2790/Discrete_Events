typedef struct Job{
  int ID;
  int state;
  int time;
  struct Job * nextPtr;
} Job;

typedef struct Queue{
  struct Job * head;
  struct Job * tail;
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
void enter_disk(Queue * disk_Queue, Queue * event, int disk, int time);
