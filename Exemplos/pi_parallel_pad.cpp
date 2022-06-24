#include <stdio.h>
#include <omp.h>
#include <ctime>
#include <chrono>
#include <iostream>

#define NUM_THREADS 4

#define PAD 8  // assume 64 byte L1 cache line size

static long num_steps = 2000000000;
double step;

int main (){

	auto t1 = std::chrono::high_resolution_clock::now();

	int i, nthreads;  
	double pi, sum[NUM_THREADS][PAD];	
	
	step = 1.0/(double) num_steps;
	omp_set_num_threads(NUM_THREADS);

	#pragma omp parallel 
	{
		int i, id, NT;
		double x;
		id = omp_get_thread_num();
		NT = omp_get_num_threads();
		if (id == 0)   
			nthreads = NT;

		for (i=id, sum[id][0]=0.0;i< num_steps; i=i+ NUM_THREADS){
			x = (i+0.5)*step;
			sum[id][0] += 4.0/(1.0+x*x);
		}
		//printf("Thread %d Sum = %f\n", id, sum[id]);
	}


	for(i=0, pi=0.0; i<NUM_THREADS; i++) 
		pi += sum[i][0] * step;

	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = (std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count());
	float time = (float)duration/1000000;

	std::cout << time << std::endl;	
}