

#include <stdlib.h>
#include "jobber.h"
#include "task.h"

typedef struct Job{
    int jobID;
    JOB_STATUS status;
    JOB_STATUS prev_status;
    pid_t  pid;
    pid_t  pgid;
    TASK* task;
    int exitStatus;
    char* taskspec;
} Job;

extern  Job * jobTable [MAX_JOBS];
volatile extern  int enable;
volatile extern  int running_task;
volatile extern  int child_enable;
volatile extern  int exit_status;
volatile extern  int sigflag;

void handle_interrupt(int sig);

void run_job(Job* job);

void run_waiting();

int handle_job(void);
void handle_enable();
