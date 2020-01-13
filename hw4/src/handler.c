//
// Created by student on 11/7/19.
//
#include "handler.h"
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


Job * jobTable [MAX_JOBS]={NULL};
volatile int enable;
volatile int running_task=0;
volatile int child_enable=0;
volatile int exit_status=0;
volatile int sigflag =0;

void run_job( Job* job){
    pid_t pid;
    TASK *task = job->task;
    if ((pid = fork()) == 0) {
        PIPELINE_LIST *pipelineList = task->pipelines;
        if (pipelineList->rest == NULL) {
            info("hi");
        }
        while (pipelineList != NULL) {
            // case one pi
            info("running");
            PIPELINE *pipeline = pipelineList->first;
            COMMAND_LIST *commandList = pipeline->commands;
            pipelineList = pipelineList->rest;
            if (commandList->rest == NULL) {
                info("one command");
                COMMAND *command1 = commandList->first;
                WORD_LIST *wordList = command1->words;
                int size = 0;
                int counter = 0;
                while (wordList != NULL) {
                    size += (int) (strlen(wordList->first)+1);
                    counter++;
                    wordList = wordList->rest;
                }
                // count for null!
                counter++;
                char* argv[counter];
                wordList = command1->words;
                for (int j = 0; j < counter-1; ++j) {
                    argv[j]=wordList->first;
                    wordList = wordList->rest;
                }
                // assign null
                argv[counter-1] = NULL;

                int redirect_infd = -1;
                int redirect_outfd = -1;
                if (pipeline->input_path != NULL) {
                    //case we have input
                    redirect_infd = open(pipeline->input_path, O_RDONLY);
                    if (redirect_infd != -1) {
                        dup2(redirect_infd, 0);
                    } else {
                        close(redirect_infd);
                        abort();
                    }
                }
                if (pipeline->output_path != NULL) {
                    redirect_outfd = open(pipeline->output_path,
                                          O_CREAT | O_WRONLY | O_TRUNC,
                                          S_IRWXU | S_IRWXG | S_IRWXO);
                    if (redirect_outfd != -1) {
                        dup2(redirect_outfd, 1);
                    } else {
                        close(redirect_outfd);
                        abort();
                    }
                }
                info("%s%s%s", argv[0], argv[1],argv[2]);
                if (fork() == 0) {
                    if ((execvp(argv[0], argv)) != 0) {
                        abort();
                    }
                } else {
                    //
                    int status;
                    wait(&status);
                    if (redirect_infd != -1) {
                        close(redirect_infd);
                    }
                    if (redirect_outfd != -1) {
                        close(redirect_outfd);
                    }
                    if (WIFEXITED(status)) {
                        //child_enable ++;
                        info("%s%d\n", "child exit: ", WEXITSTATUS(status));
                        exit_status = WEXITSTATUS(status);
                    } else if (WIFSIGNALED(status)) {
                        info("Child aborted");
                        abort();
                    } else{
                        abort();
                    }
                }
            } else {
                info("multiple commands");
                int commandsCounter = 0;
                while (commandList != NULL) {
                    commandsCounter++;
                    commandList = commandList->rest;
                }
                commandList = pipeline->commands;
                int infd = -1;
                int outfd = -1;
                int child_count = commandsCounter;
                int fdc[2 * (child_count - 1)];
                for (int j = 0; j < (child_count - 1) * 2; j += 2) {
                    pipe(fdc + j);
                }
                child_enable = 0;
                int pipe_index = 0;
                // first command
                if ((fork()) == 0) {
                    if (pipeline->input_path != NULL) {
                        infd = open(pipeline->input_path, O_RDONLY);
                        if (infd == -1) {
                            abort();
                        }
                    }
                    if (infd == -1) {
                        dup2(fdc[pipe_index + 1], STDOUT_FILENO);
                        for (int j = 0; j < (child_count - 1) * 2; ++j) {
                            close(fdc[j]);
                        }
                    } else {
                        dup2(infd, STDIN_FILENO);
                        dup2(fdc[pipe_index + 1], STDOUT_FILENO);
                        for (int j = 0; j < (child_count - 1) * 2; ++j) {
                            close(fdc[j]);
                        }
                    }
                    COMMAND *command1 = commandList->first;
                    WORD_LIST *wordList = command1->words;
                    int counter = 0;
                    while (wordList != NULL) {
                        counter++;
                        wordList = wordList->rest;
                    }
                    // count for null!
                    counter++;
                    char* argv[counter];
                    wordList = command1->words;
                    for (int j = 0; j < counter-1; ++j) {
                        argv[j]=wordList->first;
                        wordList = wordList->rest;
                    }
                    // assign null
                    argv[counter-1] = NULL;

                    info("%s%s", argv[0], argv[1]);
                    int execvpResult = 0;
                    execvpResult = execvp(argv[0], argv);
                    if (execvpResult != 0) {
                        if (infd != -1) {
                            close(infd);
                        }

                        abort();
                    }
                } else {
                    commandsCounter--;
                    commandList = commandList->rest;
                    pipe_index += 2;
                    while (commandsCounter != 1) {
                        // nth command
                        if ((fork()) == 0) {
                            dup2(fdc[pipe_index + 1], STDOUT_FILENO);
                            dup2(fdc[pipe_index - 2], STDIN_FILENO);
                            for (int j = 0; j < (child_count - 1) * 2; ++j) {
                                close(fdc[j]);
                            }
                            COMMAND *command1 = commandList->first;
                            WORD_LIST *wordList = command1->words;
                            int counter = 0;
                            while (wordList != NULL) {
                                counter++;
                                wordList = wordList->rest;
                            }
                            // count for null!
                            counter++;
                            char* argv[counter];
                            wordList = command1->words;
                            for (int j = 0; j < counter-1; ++j) {
                                argv[j]=wordList->first;
                                wordList = wordList->rest;
                            }
                            // assign null
                            argv[counter-1] = NULL;

                            info("%s%s", argv[0], argv[1]);
                            int execvpResult = 0;
                            execvpResult = execvp(argv[0], argv);
                            if (execvpResult != 0) {

                                abort();
                            }
                        } else {
                            commandsCounter--;
                            pipe_index += 2;
                            commandList = commandList->rest;
                        }
                    }
                    //case last command
                    if ((fork()) == 0) {
                        if (pipeline->output_path != NULL) {
                            outfd = open(pipeline->output_path,
                                         O_CREAT | O_WRONLY | O_TRUNC,
                                         S_IRWXU | S_IRWXG | S_IRWXO);
                            if (outfd == -1) {
                                abort();
                            }
                        }
                        if (outfd == -1) {
                            dup2(fdc[pipe_index - 2], STDIN_FILENO);
                            for (int j = 0; j < (child_count - 1) * 2; ++j) {
                                close(fdc[j]);
                            }

                        } else {
                            dup2(fdc[pipe_index - 2], STDIN_FILENO);
                            dup2(outfd, STDOUT_FILENO);
                            for (int j = 0; j < (child_count - 1) * 2; ++j) {
                                close(fdc[j]);
                            }
                        }
                        COMMAND *command1 = commandList->first;
                        WORD_LIST *wordList = command1->words;
                        int counter = 0;
                        while (wordList != NULL) {
                            counter++;
                            wordList = wordList->rest;
                        }
                        // count for null!
                        counter++;
                        char* argv[counter];
                        wordList = command1->words;
                        for (int j = 0; j < counter-1; ++j) {
                            argv[j]=wordList->first;
                            wordList = wordList->rest;
                        }
                        // assign null
                        argv[counter-1] = NULL;

                        info("%s%s", argv[0], argv[1]);
                        int execvpResult = 0;
                        execvpResult = execvp(argv[0], argv);
                        if (execvpResult != 0) {
                            if (outfd != -1) {
                                close(outfd);
                            }

                            abort();
                        }
                    } else {
                        // close all fd
                        for (int j = 0; j < (child_count - 1) * 2; ++j) {
                            close(fdc[j]);
                        }
                        //killpg(getpid(), SIGUSR1);
                        int status;
                        sigset_t mask_all, prev_all;
                        sigfillset(&mask_all);
                        // wait all child to finish
                        while (( waitpid(-1, &status, WNOHANG | WSTOPPED | WCONTINUED)) > 0) {
                            sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
                            if (WIFEXITED(status)) {
                                child_enable ++;
                                info("%s%d\n","child exit: ",WEXITSTATUS(status));
                                exit_status = WEXITSTATUS(status);
                            } else if (WIFSIGNALED(status)) {
                                info("Child aborted");
                                abort();
                            } else {
                                abort();
                            }
                            sigprocmask(SIG_SETMASK,&prev_all,NULL);
                        }
                    }
                }
            }
        }
        info("end");
        exit(exit_status);
    } else {
        sigset_t mask_all, prev_all;
        sigfillset(&mask_all);
        sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        job->pid = pid;
        if (setpgid(pid, pid) == 0) {
            job->pgid = pid;
        } else {
            job->pgid = -1;
        }
        running_task++;
        sf_job_status_change(job->jobID, job->status, RUNNING);
        job->prev_status = job->status;
        job->status = RUNNING;
        sf_job_start(job->jobID, job->pgid);
        sigprocmask(SIG_SETMASK, &prev_all, NULL);
    }
}

int handle_job() {
    if (!sigflag){
        return 0;
    }
    int status;
    sigset_t mask_all, prev_all;
    pid_t pid;
    sigfillset(&mask_all);
    debug("HANDLE JOB");
    while ((pid = waitpid(-1, &status, WNOHANG | WSTOPPED | WCONTINUED)) > 0) {
        sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        // case jobber level
        int jobid = -1;
        for (int i = 0; i < MAX_JOBS; ++i) {
            if (jobTable[i]->pid == pid) {
                jobid = i;
                break;
            }
        }
        if (WIFEXITED(status)) {
            sf_job_status_change(jobid, jobTable[jobid]->status, COMPLETED);
            jobTable[jobid]->prev_status = jobTable[jobid]->status;
            jobTable[jobid]->status = COMPLETED;
            jobTable[jobid]->exitStatus = WEXITSTATUS(status);
            sf_job_end(jobid, jobTable[jobid]->pgid, status);
            running_task--;
            run_waiting();
        } else if (WIFSIGNALED(status)) {
            // case of waitting we forked so it will signal
            sf_job_status_change(jobid, jobTable[jobid]->status, ABORTED);
            jobTable[jobid]->prev_status = jobTable[jobid]->status;
            jobTable[jobid]->status = ABORTED;
            sf_job_end(jobid, jobTable[jobid]->pgid, status);
            running_task--;
            run_waiting();
        } else if (WIFCONTINUED(status)) {
            sf_job_status_change(jobid, jobTable[jobid]->status, RUNNING);
            jobTable[jobid]->prev_status = jobTable[jobid]->status;
            jobTable[jobid]->status = RUNNING;
            //running_task++;
        } else if (WIFSTOPPED(status)) {
            sf_job_status_change(jobid, jobTable[jobid]->status, PAUSED);
            jobTable[jobid]->prev_status = jobTable[jobid]->status;
            jobTable[jobid]->status = PAUSED;
            //running_task--;
        } else {
            error();
            abort();
        }
        sigflag = 0;
        sigprocmask(SIG_SETMASK, &prev_all, NULL);
    }
    return 0;
}

void run_waiting (){
    for (int i = 0; i < MAX_JOBS; ++i) {
        if (jobTable[i] != NULL) {
            if (jobTable[i]->status == WAITING && running_task < MAX_RUNNERS) {
                run_job(jobTable[i]);
            }
        }
    }
}

void handle_enable(int sig) {
    if (sig == SIGUSR1) {
        child_enable = 1;
    }
}

void handle_interrupt(int sig){
    debug("SIGCHLD");
    sigflag=1;
    //child_enable += 1;
}