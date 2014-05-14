#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void action( int temp ) {
    exit(1);
}

int main( int arvc, char *argv[] ) {
    if ( arvc < 3 ) {
        printf("Not enough arguments\n");
        return 0;
    }
    int time = atoi(argv[1]);
    int time_sleep = 100 - time;
    int time_work = time;
    int proc_id = fork();
    if ( proc_id == -1 ) {
        printf("Fork error\n");
        exit(0);
    }
    if ( proc_id == 0 ) {
        if ( execvp(argv[2], argv + 2) ) {
			printf("Exec error\n");
			exit(0);
		}
    }
    else {
        struct sigaction act;
        act.sa_handler = action;
        act.sa_flags = SA_NOCLDSTOP;
        int sig_act = sigaction(SIGCHLD, &act , 0);
        /*
        if ( sig_act == -1 ) {
            printf("Sigaction error\n");
            exit(0);
        }
        */
        while (1) {
            usleep( time_work );

			kill(proc_id, SIGSTOP);
			usleep( time_sleep );

			kill(proc_id, SIGCONT);
        }
    }
    //printf("\n The program was successfully finished!\n");
    return 0;
}
