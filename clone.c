#define _GNU_SOURCE 

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <linux/sched.h>
#include <sched.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>

#define NTHREADS 4
#define NSIZE 2000000000
#define STACK_SIZE 65536

double res[NTHREADS];

int Child(void *);
int main() {
	long long start_ts;
	long long stop_ts;
	long long elapsed_time;
	long lElapsedTime;
	struct timeval ts;


    pid_t pid[NTHREADS];
    int status;
    char *stack[NTHREADS];
    int i = 0;
    int args[NTHREADS];


	gettimeofday(&ts, NULL);
    for(i = 0; i < NTHREADS; i++) {
    	res[i] = 0.0;
    	stack[i] = malloc(STACK_SIZE);
    }
    start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial
    for(i = 0; i < NTHREADS;i++) { 
    	args[i] = i;
    	pid[i] = clone(Child,stack[i] + STACK_SIZE, CLONE_SIGHAND|CLONE_FS|CLONE_VM|SIGCHLD,&args[i]);
    	//printf("debug %d\n", pid[i]);
    }
 	for(i = 0; i < NTHREADS; i++)
 		waitpid(pid[i], &status, 0);
 	double resultado =0-0;	
    for(i = 0; i < NTHREADS; i++) {
		resultado += res[i];
		//printf("resultado %f, %f\n", resultado, res[i]);
	}
    gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final

	elapsed_time = stop_ts - start_ts;
	printf("Tiempo %3.6f segundos\n",(float)elapsed_time/1000000.0);
    printf("Back to parent: Value of n: %.15lf\n", resultado);
    
    return 0;
}
int Child(void *args) {

	int tnum=*((int *) args);
	int inicio=tnum*(NSIZE/NTHREADS);
	int fin=inicio+(NSIZE/NTHREADS);
	int i;
	//printf("Hilo %d, inicio=%d, fin=%d, res = %.15lf\n",getpid(),inicio,fin, res[tnum]);
		for(i = inicio; i < fin; i++) {
		if(i % 2 == 0) {
			res[tnum] += (1 / (((double)i*2)+1));
			//printf("%f %d %f \n",(1 / (((double)i*2)-1)), tnum, res[tnum]);
		}
		else {
			res[tnum] -= (1 / (((double)i*2)+1));
			//printf("%f %d %f \n",((((double)i*2)-1)), tnum, res[tnum]);
		}
	}
	//printf("Hilo %d, inicio=%d, fin=%d, res = %.15lf\n",tnum,inicio,fin, res[tnum]);

}
