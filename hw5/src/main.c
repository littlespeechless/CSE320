#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "client_registry.h"
#include "exchange.h"
#include "trader.h"
#include "debug.h"
#include "csapp.h"
#include "server.h"

extern EXCHANGE *exchange;
extern CLIENT_REGISTRY *client_registry;
static void terminate(int status);
static void sighup_handler(int sig);
int listenfd;
/*
 * "Bourse" exchange server.
 *
 * Usage: bourse <port>
 */
int main(int argc, char* argv[]){
    // Option processing should be performed here.
    // Option '-p <port>' is required in order to specify the port number
    // on which the server should listen.
    if(argc!=3){
        printf("%s\n","Usage: bourse -p <port>");
        exit(EXIT_FAILURE);
    }
    if (strcmp(argv[1],"-p")!=0){
        printf("%s\n","Usage: bourse -p <port>");
        exit(EXIT_FAILURE);
    }
    char* port = argv[2];
    // Perform required initializations of the client_registry,
    // maze, and player modules.
    client_registry = creg_init();
    exchange = exchange_init();
    trader_init();
    info("%d",getpid());
    // TODO: Set up the server socket and enter a loop to accept connections
    // on this socket.  For each connection, a thread should be started to
    // run function brs_client_service().  In addition, you should install
    // a SIGHUP handler, so that receipt of SIGHUP will perform a clean
    // shutdown of the server.
    struct sigaction action;
    memset(&action,0, sizeof(action));
    action.sa_handler = &sighup_handler;
    action.sa_flags = SA_NODEFER;
    if (sigaction(SIGHUP,&action,NULL)<0){
        fprintf(stderr,"Error on binding sigaction");
        terminate(EXIT_FAILURE);
    }

    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;
    listenfd = Open_listenfd(port);
    int *connfdp;
    while (true){
        clientlen=sizeof(struct sockaddr_storage);
        int connfd = Accept(listenfd,(SA *) &clientaddr, &clientlen);
        if(connfd>0){
            connfdp = malloc(sizeof(int*));
            *connfdp = connfd;
            pthread_create(&tid, NULL, brs_client_service, connfdp);
        }
    }
    fprintf(stderr, "You have to finish implementing main() "
	    "before the Bourse server will function.\n");

    terminate(EXIT_FAILURE);
}

/*
 * Function called to cleanly shut down the server.
 */
static void terminate(int status) {
    // Shutdown all client connections.
    // This will trigger the eventual termination of service threads.
    creg_shutdown_all(client_registry);
    
    debug("Waiting for service threads to terminate...");
    creg_wait_for_empty(client_registry);
    debug("Closing connection port");
    close(listenfd);
    debug("All service threads terminated.");
    // Finalize modules.
    creg_fini(client_registry);
    exchange_fini(exchange);
    trader_fini();

    debug("Bourse server terminating");
    exit(status);
}

static void sighup_handler(int sig){
    terminate(EXIT_SUCCESS);
}