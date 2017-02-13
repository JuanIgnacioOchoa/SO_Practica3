#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>

#define NTHREADS 4
#define NSIZE 2000000000

void sum(int seg, int tnum) {
	double *pidArr;
	double res = 0.0;
	pidArr = shmat(seg,NULL,0);
	int inicio=tnum*(NSIZE/NTHREADS);
	int fin=inicio+(NSIZE/NTHREADS);
	int i;
		for(i = inicio; i < fin; i++) {
		if(i % 2 == 0) {
			res += (1 / (((double)i*2)+1));
			//printf("%f %d %f \n",(1 / (((double)i*2)-1)), tnum, res[tnum]);
		}
		else {
			res -= (1 / (((double)i*2)+1));
			//printf("%f %d %f \n",((((double)i*2)-1)), tnum, res[tnum]);
		}
	}
	printf("%.15lf, %.15lf\n",*pidArr, res );
	*pidArr += res;
	//printf("fork %d, %d, %d, %.15lf\n", seg, tnum, getpid(), res);
	return;
}

int main()
{
	long long start_ts;
	long long stop_ts;
	long long elapsed_time;
	long lElapsedTime;
	struct timeval ts;

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial

    int pid;
    double *pidArr;
    int segment_id = shmget(IPC_PRIVATE, sizeof(pidArr), S_IRUSR | S_IWUSR);
    int i;
    int status;
    pidArr = shmat(segment_id, NULL, 0);

    if(fork()){
  		if(fork()){
  			if(fork()) {
  				if(fork()){

  				} else {
  					sum(segment_id,3);
  					exit(0);
  				}
  			} else {
  				sum(segment_id,2);
  				exit(0);
  			}
  		} else {
  			sum(segment_id,1);
  			exit(0);
  		}
    } else {
    	sum(segment_id,0);
    	exit(0);
    }
    for(i = 0; i < NTHREADS; i++){
    	wait(&status);
    }
    wait(NULL);
    gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final

	elapsed_time = stop_ts - start_ts;
	printf("Tiempo %3.6f segundos\n",(float)elapsed_time/1000000.0);
   	printf("resultado %.15lf\n",*pidArr );
    return 0;
}