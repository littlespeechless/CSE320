#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "handler.h"

/*
 * "Jobber" job spooler.
 */

int main(int argc, char *argv[])
{
    // TO BE IMPLEMENTED
    if (jobs_init()) {
        printf("%s\n", "Error at initialize");
        exit(EXIT_FAILURE);
    }
    int flag = 1;
    //signal(SIGTSTP,handle_stop);

    while(flag){
        char * inputs = sf_readline("jobber>");
        if (inputs==NULL){
            //case of eof
            //free(inputs);
            break;
        }
        if (strlen(inputs)!=0){
            // trim last space
            while (*(inputs + strlen(inputs) - 1)== ' '){
                *(inputs + strlen(inputs)-1)='\0';
            }
            // check argument
            char *indexPtr = strchr(inputs,' ');
            if (indexPtr==NULL){
                //case help
                if (strcmp(inputs,"help")==0){
                    printf("%s","Available commands:\n"
                                "help (0 args) Print this help message\n"
                                "quit (0 args) Quit the program\n"
                                "enable (0 args) Allow jobs to start\n"
                                "disable (0 args) Prevent jobs from starting\n"
                                "spool (1 args) Spool a new job\n"
                                "pause (1 args) Pause a running job\n"
                                "resume (1 args) Resume a paused job\n"
                                "cancel (1 args) Cancel an unfinished job\n"
                                "expunge (1 args) Expunge a finished job\n"
                                "status (1 args) Print the status of a job\n"
                                "jobs (0 args) Print the status of all jobs\n");
                } else if (strcmp(inputs, "quit")==0) {
                    flag = 0;
                } else if (strcmp(inputs, "enable")==0){
                    jobs_set_enabled(1);
                    run_waiting();
                    //printf("%d\n",jobs_set_enabled(1));
                } else if (strcmp(inputs, "disable")==0){
                    jobs_set_enabled(0);
                } else if (strcmp(inputs, "jobs")==0){
                    int enableStatus = jobs_get_enabled();
                    if (enableStatus == 1){
                        printf("%s\n","Starting jobs is enabled");
                    } else{
                        printf("%s\n","Starting jobs is disabled");
                    }
                    for (int i = 0; i < MAX_JOBS; ++i) {
                        if(jobTable[i]!=NULL){
                            int status = job_get_status(i);
                            if (status != -1){
                                printf("%s%d","job ", i);
                                printf("%s"," [");
                                if (status == COMPLETED){
                                    printf("%s%s%d%s",job_status_names[status]," ("
                                            ,job_get_result(i), ")]: ");
                                } else if (status == ABORTED){
                                    if (job_was_canceled(i)){
                                        printf("%s%s",job_status_names[status]," (canceled)]: ");
                                    } else{
                                        printf("%s%s",job_status_names[status]
                                                ,"]: ");
                                    }
                                }else {
                                    printf("%s%s",job_status_names[status]
                                            ,"]: ");
                                }
                                printf("%s\n",job_get_taskspec(i));
                            } else {
                                printf("%s\n", "Error: Jobs");
                            }
                        }
                    }
                } else{
                    if (strcmp(inputs, "spool")==0||
                        strcmp(inputs, "pause")==0 ||
                        strcmp(inputs, "resume")==0 ||
                        strcmp(inputs, "cancel")==0 ||
                        strcmp(inputs, "expunge")==0||
                        strcmp(inputs, "status")==0){
                        printf("%s\n","Invalid argument amount.");
                    } else{
                        printf("%s\n","Unrecognized command.");
                    }
                }
            } else{
                if (indexPtr - inputs == 0){
                    printf("Unrecognized command.\n");
                } else {
                    char* options = (char*)malloc(sizeof(char)*((int)(indexPtr - inputs))+1);
                    //memset(options,0,((int)(indexPtr - inputs)));
                    memcpy(options,inputs,indexPtr - inputs);
                    *(options+(indexPtr - inputs))='\0';
                    //find next argument
                    while (*indexPtr==' '){
                        indexPtr+=1;
                    }
                    if (strcmp(options,"spool")==0){
                        if (*indexPtr == '\''){
                            char * quote = strchr(indexPtr+1,'\'');
                            if ((inputs+strlen(inputs)-1)==quote){
                                //case valid
                                indexPtr+=1;
                                *((inputs+strlen(inputs)-1)) = '\0';
                                printf("%s\n",indexPtr);
                                if (job_create(indexPtr)){
                                    printf("%s\n","Error: spool");
                                }
                            } else{
                                printf("%s\n","Invalid argument amount.");
                            }
                        }else{
                            char * space = strchr(indexPtr,' ');
                            if (space != NULL){
                                printf("%s\n","Invalid argument amount.");
                            } else {
                                if (job_create(indexPtr)){
                                    printf("%s\n","Error: spool");
                                }
                            }
                        }
                    } else if (strcmp(options,"pause")==0){
                        if (indexPtr!=(inputs + strlen(inputs)-1)){
                            printf("%s\n","Invalid argument amount.");
                        } else{
                            if ((*indexPtr - 48) > MAX_JOBS || (*indexPtr -48)<0){
                                printf("%s\n","Invalid ID Range.");
                            } else{
                                if (job_pause(*indexPtr - 48)){
                                    printf("%s\n","Error: pause");
                                }
                            }
                        }
                    }else if (strcmp(options,"resume")==0){
                        if (indexPtr!=(inputs + strlen(inputs)-1)){
                            printf("%s\n","Invalid argument amount.");
                        } else{
                            if ((*indexPtr - 48) > MAX_JOBS || (*indexPtr -48)<0){
                                printf("%s\n","Invalid ID Range.");
                            } else{
                                if (job_resume(*indexPtr - 48)){
                                    printf("%s\n","Error: resume");
                                }
                            }
                        }
                    }else if (strcmp(options,"cancel")==0){
                        if (indexPtr!=(inputs + strlen(inputs)-1)){
                            printf("%s\n","Invalid argument amount.");
                        } else{
                            if ((*indexPtr - 48) > MAX_JOBS || (*indexPtr -48)<0){
                                printf("%s\n","Invalid ID Range.");
                            } else{
                                if (job_cancel(*indexPtr - 48)){
                                    printf("%s\n","Error: cancel");
                                }
                            }
                        }
                    }else if (strcmp(options,"expunge")==0){
                        if (indexPtr!=(inputs + strlen(inputs)-1)){
                            printf("%s\n","Invalid argument amount.");
                        } else{
                            if ((*indexPtr - 48) > MAX_JOBS || (*indexPtr -48)<0){
                                printf("%s\n","Invalid ID Range.");
                            } else{
                                if (job_expunge(*indexPtr -48)!=0){
                                    printf("%s\n","Error: expunge");
                                }
                            }
                        }
                    }else if (strcmp(options,"status")==0){
                        if (indexPtr!=(inputs + strlen(inputs)-1)){
                            printf("%s\n","Invalid argument amount.");
                        } else{
                            if ((*indexPtr - 48) > MAX_JOBS || (*indexPtr -48)<0){
                                printf("%s\n","Invalid ID Range.");
                            } else{
                                int status = job_get_status((*indexPtr) - 48);
                                if (status != -1){
                                    printf("%s%s","job ", indexPtr);
                                    printf("%s"," [");
                                    if (status == COMPLETED){
                                        printf("%s%s%d%s",job_status_names[status]," ("
                                                ,job_get_result((*indexPtr) - 48), ")]: ");
                                    }else if (status == ABORTED){
                                        if (job_was_canceled((*indexPtr) - 48)){
                                            printf("%s%s",job_status_names[status]," (cancelled)]: ");
                                        } else{
                                            printf("%s%s",job_status_names[status]
                                                    ,"]: ");
                                        }
                                    } else {
                                        printf("%s%s",job_status_names[status]
                                                ,"]: ");
                                    }
                                    printf("%s\n",job_get_taskspec((*indexPtr) - 48));
                                } else {
                                    printf("%s\n", "Error: Status");
                                }
                            }
                        }
                    } else{
                        if (strcmp(options,"help")==0 ||
                            strcmp(options,"quit")==0 ||
                            strcmp(options,"enable")==0 ||
                            strcmp(options,"disable")==0 ||
                            strcmp(options,"jobs")==0){
                            printf("%s\n","Invalid argument amount.");
                        } else{
                            printf("%s\n","Unrecognized command.");
                        }
                    }
                    free(options);
                }
            }
        }
        free(inputs);
    }
    jobs_fini();
    exit(EXIT_SUCCESS);
}

/*
 * Just a reminder: All non-main functions should
 * be in another file not named main.c
 */
