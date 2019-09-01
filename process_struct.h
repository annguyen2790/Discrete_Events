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
void insert_queue(Queue * q, int job_ID, int job_state, int job_time);
void printQueue(Queue * q);
int isEmpty(Queue *q);
Job * delete_head(Queue * q);

