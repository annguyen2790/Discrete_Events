
#define inCPUqueue 1
#define inCPUexecute 2
#define FINISHED 3

struct Job{
  int ID;
  int current_state;
  int time;
};
  
