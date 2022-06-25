#include <iostream>
#include <algorithm>
#include <omp.h>
#include <math.h>
#include <chrono>
#include <set>
#include <vector>

#define NUM_THREADS 4

using namespace std;
static long num_steps = 5;
const int sz = 1000000;

int main() {

	srand((unsigned) time(0));
	int myArr[sz];
	// omp_set_num_threads(NUM_THREADS);
	auto arr_size = sizeof(myArr) / sizeof(myArr[0]);

	// ================ Inicialização do array ==================
	auto tinit= std::chrono::high_resolution_clock::now();

	for(int i=0;i<sz;i++){
		myArr[i] = rand()%100;
	}

	auto tinit2 = std::chrono::high_resolution_clock::now();
	auto durationInit = (chrono::duration_cast<chrono::microseconds>( tinit2 - tinit ).count());
	float timeInit = (float)durationInit/1000000;
	cout << "Init time: " << timeInit << endl;

	// ================ Ordenação do array ==================
	auto t1 = std::chrono::high_resolution_clock::now();

	#pragma omp parallel for
	for(int i=0;i<sz;i++){
		myArr[i] = rand()%100;
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = (std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count());
	float time = (float)duration/1000000;

	std::cout << "Init Parallel: " << time << " microsegundos." << std::endl;
}
