#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
int main() {
	long long start_ts;
	long long stop_ts;
	long long elapsed_time;
	long lElapsedTime;
	struct timeval ts;

	double sub = 1.0;
	int i = 0;
	double res = 0;

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial

	for(i = 0; i < 2000000000; i++) {
		if(i % 2 == 0)
			res += (1 / sub);
		else 
			res -= (1 / sub);
		sub += 2.0;
	}
	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final

	elapsed_time = stop_ts - start_ts;
	printf("Tiempo %3.6f segundos\n",(float)elapsed_time/1000000.0);
	printf("resultado %f\n", res);
	return 0;
}