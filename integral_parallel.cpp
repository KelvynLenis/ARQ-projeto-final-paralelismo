#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <omp.h>
#include <list>
#include <utility>
#include <exception>
#include <math.h>
#include <chrono>
#include <vector>

using namespace std;
#define NUM_THREADS 4
#define PAD 8

struct Result
{
	double timestamp, area;
};

double f(const double x);
const Result rectangleMethod(const double, const double, const double);
const Result trapezoidalMethod(const double, const double, const double);

int main()
{
	const short steps = 100;
	short method;
	double x1, x2, dx;

	omp_set_num_threads(NUM_THREADS);
	cout << fixed << setprecision(8) << endl;

	cout << "   X1: "; cin >> x1;
	cout << "   X2: "; cin >> x2;
	cout << "   dx: "; cin >> dx;
	cout << "   Method (1 - rectangle, 2 - trapezoidal): "; 
	cin >> method;

	auto t1 = chrono::high_resolution_clock::now();

	vector<pair<short, Result>> results;
	double resultArea;

	for (int i = 0; i < steps; i++){
		Result result = (method == 1) ?
			rectangleMethod(x1, x2, dx) :
			trapezoidalMethod(x1, x2, dx);

		pair<short, Result> s_result(i + 1, result);
		resultArea = result.area;
		results.push_back(s_result);
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = (std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count());
	float time = (float)duration/1000000;

	cout << endl << "   Results:" << endl;
	std::cout << "  Tempo de processamento = " << time << " microsegundos." << std::endl;

	cout << "  area: " << resultArea << endl;
	cout << endl;
	cin.get();
	return 0;
}

const Result rectangleMethod(const double x1, const double x2, const double dx){
	const int N = static_cast<int>((x2 - x1) / dx);
	double now = omp_get_wtime();
	double s = 0;

	omp_set_num_threads(NUM_THREADS);
	int id, NT;
	id = omp_get_thread_num();
	NT = omp_get_num_threads();

	#pragma omp parallel for reduction(+: s)
	for (int i = 1; i <= N; i++){
		s += f(x1 + i * dx);	
	} 

	// double sum[NUM_THREADS][PAD];	
	// int i;

	// #pragma omp parallel
	// {
	// 	#pragma omp for reduction(+: s)
	// 	for(int i = id; i <= N; i = i + NUM_THREADS){
	// 		// sum[id][0] = 0.0;
	// 		sum[id][0] += f(x1 + i * dx);	
	// 	}

	// } 

	#pragma barrier
	s *= dx;
	 
	return { omp_get_wtime() - now, s };
}

const Result trapezoidalMethod(const double x1, const double x2, const double dx){
	const int N = static_cast<int>((x2 - x1) / dx);
	double now = omp_get_wtime();
	double s = 0;

	#pragma omp parallel for reduction(+: s)
	for (int i = 1; i < N; i++) {
		s += f(x1 + i * dx);
	}

	#pragma barrier
	s = (s + (f(x1) + f(x2)) / 2) * dx;
	 
	return { omp_get_wtime() - now, s };
}

double f(const double x)
{
	return sin(x);
}
