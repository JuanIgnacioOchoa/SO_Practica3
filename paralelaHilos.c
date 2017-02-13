#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

#define NTHREADS 4
#define NSIZE 2000000000

double res[NTHREADS];

void *threadfunc(void *arg) {
	int tnum=*((int *) arg);
	int inicio=tnum*(NSIZE/NTHREADS);
	int fin=inicio+(NSIZE/NTHREADS);
	int i;
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
	printf("Hilo %d, inicio=%d, fin=%d, res = %.15lf\n",tnum,inicio,fin, res[tnum]);
}

int main() {
	long long start_ts;
	long long stop_ts;
	long long elapsed_time;
	long lElapsedTime;
	struct timeval ts;

	double resultado = 0.0;
	int i = 0;

	int args[NTHREADS];
	pthread_t tid[NTHREADS];
	gettimeofday(&ts, NULL);
	for(i = 0; i < NTHREADS; i++) 
		res[i] = 0.0;
	start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial

	for(i=0;i<NTHREADS;i++)
	{
		args[i]=i;
		pthread_create(&tid[i],NULL,threadfunc,(void *) &args[i]);
	}
	
	for(i=0;i<NTHREADS;i++)
		pthread_join(tid[i],NULL);

	for(i = 0; i < NTHREADS; i++) {
		resultado += res[i];
		//printf("resultado %f, %f\n", resultado, res[i]);
	}

	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final

	elapsed_time = stop_ts - start_ts;
	printf("Tiempo %3.6f segundos\n",(float)elapsed_time/1000000.0);
	printf("resultado %.10lf\n", resultado);
	return 0;
}