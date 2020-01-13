/*
 * Job manager for "jobber".
 */

#include <stdlib.h>
#include <string.h>
#include "signal.h"
#include "handler.h"


int jobs_init(void) {
    signal(SIGCHLD,handle_interrupt);
    sf_set_readline_signal_hook(handle_job);
    memset(jobTable, 0, sizeof(Job *) * MAX_JOBS);
    enable = 0;
    running_task = 0;
    child_enable = 0;
    return 0;
}

void jobs_fini(void) {
    // TO BE IMPLEMENTED
    int cancelAll = 1;
    while (cancelAll){
        cancelAll = 0;
        for (int i = 0; i < MAX_JOBS; ++i) {
            if (jobTable[i]!=NULL){
                if (jobTable[i]->status!=ABORTED && jobTable[i]->status!=COMPLETED) {
                    cancelAll =1;
                    job_cancel(i);
                    handle_job();
                }
            }
        }
        handle_job();
    }
    for (int i = 0; i < MAX_JOBS; ++i) {
        if (jobTable[i]!=NULL){
            job_expunge(i);
        }
    }
}

int jobs_set_enabled(int val) {
    // TO BE IMPLEMENTED
    int temp = enable;
    enable = val;
    return temp;
}

int jobs_get_enabled() {
    // TO BE IMPLEMENTED
    return enable;
}

int job_create(char *command) {
    // TO BE IMPLEMENTED
    int jobId = -1;
    for (int i = 0; i < MAX_JOBS ; ++i) {
        if (jobTable[i] == NULL){
            jobId = i;
            break;
        }
    }
    if (jobId ==-1){
        return -1;
    }
    char* temp = (char*) malloc((sizeof(char)*strlen(command)+1));
    memcpy(temp,command,strlen(command)+1);
    TASK * task = parse_task(&command);
    if (task == NULL){
        free(temp);
        return  -1;
    }
    Job * job = malloc(sizeof(Job));
    job->task = task;
    job->status = NEW;
    job->jobID = jobId;
    job->exitStatus = -1;
    jobTable[jobId] = job;
    job->taskspec = temp;
    sf_job_create(jobId);
    //printf("%s%d%s\n","job ",jobId," created.");
    //pid_t  pid;
    signal(SIGUSR1, handle_enable);
    sf_job_status_change(jobId,job->status,WAITING);
    job->prev_status = job->status;
    job->status = WAITING;
    if (enable==1){
        if (running_task<MAX_RUNNERS){
            run_job(job);
            return 0;
        }
    } else{
        return 0;
    }
    return -1;
}

int job_expunge(int jobid) {
    // TO BE IMPLEMENTED
    if (jobid>=0&&jobid<MAX_JOBS){
        if (jobTable[jobid]!=NULL){
            if (jobTable[jobid]->status == ABORTED ||
                    jobTable[jobid]->status== COMPLETED){
                free_task(jobTable[jobid]->task);
                free(jobTable[jobid]->taskspec);
                free(jobTable[jobid]);
                jobTable[jobid] = NULL;
                sf_job_expunge(jobid);
                return 0;
            }
        }
    }
    return -1;
}

int job_cancel(int jobid) {
    // TO BE IMPLEMENTED
    sigset_t mask_all, prev_all;
    sigfillset(&mask_all);
    if (jobid>=0&&jobid<MAX_JOBS){
        if (jobTable[jobid]!=NULL){
            if (jobTable[jobid]->status==RUNNING || jobTable[jobid]->status==PAUSED ){
                sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
                killpg(jobTable[jobid]->pgid,SIGKILL);
                sf_job_status_change(jobid,jobTable[jobid]->status,CANCELED);
                jobTable[jobid]->prev_status = jobTable[jobid]->status;
                jobTable[jobid]->status=CANCELED;
                sigprocmask(SIG_SETMASK, &prev_all, NULL);
            } else if (jobTable[jobid]->status == WAITING ){
                sf_job_status_change(jobid,jobTable[jobid]->status,ABORTED);
                jobTable[jobid]->prev_status = jobTable[jobid]->status;
                jobTable[jobid]->status=ABORTED;
            }

            return 0;
        }
    }
    return -1;
}

int job_pause(int jobid) {
    // TO BE IMPLEMENTED
    sigset_t mask_all, prev_all;
    sigfillset(&mask_all);
    if (jobid>=0&&jobid<MAX_JOBS) {
        if (jobTable[jobid] != NULL) {
            if (jobTable[jobid]->status == RUNNING){
                sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
                sf_job_pause(jobid,job_get_pgid(jobid));
                killpg(job_get_pgid(jobid),SIGSTOP);
                sigprocmask(SIG_SETMASK, &prev_all, NULL);
                return 0;
            }
        }
    }
    return -1;
}

int job_resume(int jobid) {
    // TO BE IMPLEMENTED
    sigset_t mask_all, prev_all;
    sigfillset(&mask_all);
    if (jobid>=0&&jobid<MAX_JOBS) {
        if (jobTable[jobid] != NULL) {
            if (jobTable[jobid]->status == PAUSED){
                sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
                sf_job_resume(jobid,job_get_pgid(jobid));
                killpg(job_get_pgid(jobid),SIGCONT);
                sigprocmask(SIG_SETMASK, &prev_all, NULL);
                return 0;
            }
        }
    }
    return -1;
    abort();
}

int job_get_pgid(int jobid) {
    if (jobid>=0&&jobid<MAX_JOBS){
        //check valid id
        if (jobTable[jobid]!=NULL){
            if (jobTable[jobid]->status==RUNNING ||
                jobTable[jobid]->status==PAUSED ||
                jobTable[jobid]->status==CANCELED){
                return jobTable[jobid]->pgid;
            }
        }
    }
    return -1;
}

JOB_STATUS job_get_status(int jobid) {
    // TO BE IMPLEMENTED
    if (jobid>=0&&jobid<MAX_JOBS){
        if (jobTable[jobid]!=NULL){
            return jobTable[jobid]->status;
        }
    }
    return -1;
}

int job_get_result(int jobid) {
    // TO BE IMPLEMENTED
    if (jobid>=0&&jobid<MAX_JOBS){
        if (jobTable[jobid]!=NULL){
            if (jobTable[jobid]->status==COMPLETED){
                return jobTable[jobid]->exitStatus;
            }
        }
    }
    return -1;
}

int job_was_canceled(int jobid) {
    // TO BE IMPLEMENTED
    if (jobid>=0 && jobid< MAX_JOBS){
        if (jobTable[jobid]!= NULL){
            if (jobTable[jobid]->status==ABORTED && jobTable[jobid]->prev_status == CANCELED){
                return 1;
            }
        }
    }
    return  0;
}

char *job_get_taskspec(int jobid) {
    // TO BE IMPLEMENTED
    if (jobid>=0&& jobid<8){
        if (jobTable[jobid]!=NULL){
            return jobTable[jobid]->taskspec;
        }
    }
    return NULL;
}
