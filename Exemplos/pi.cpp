#include <stdio.h>
#include <omp.h>

#include <chrono>
#include <iostream>


static long num_steps = 2000000000;
double step;

int main (){
	
	auto t1 = std::chrono::high_resolution_clock::now();


	int i; 
	double x, pi, sum = 0.0;
	step = 1.0/(double) (num_steps);

	for (i=0;i< (num_steps); i++){
		x = (i+0.5)*step;
		sum = sum + 4.0/(1.0+x*x);
	}
	pi = step * sum;
	
	auto t2 = std::chrono::high_resolution_clock::now();

	auto duration = (std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count());

	float time = (float)duration/1000000;

	std::cout << "Tempo de processamento = " << time << " segundos." << std::endl;


	printf("Pi = %f\n", pi);
}