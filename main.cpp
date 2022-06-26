#include <iostream>
#include <algorithm>
#include <omp.h>
#include <math.h>
#include <chrono>
#include <set>
#include <math.h>


#define NUM_THREADS 4
#define M_PI 3.14159265358979323846
#define PAD 8

using namespace std;
static long num_steps = 5;
const int sz = 100000;

float givenFunction(float x, float y)
{
    return pow(pow(x, 4) + pow(y, 5), 0.5);
}
  
// Function to find the double integral value
float doubleIntegral(float h, float k,
                     float lx, float ux,
                     float ly, float uy)
{
	int nx, ny;

	// z stores the table
	// ax[] stores the integral wrt y
	// for all x points considered
	float z[50][50], ax[50], answer;

	// Calculating the number of points
	// in x and y integral
	nx = (ux - lx) / h + 1;
	ny = (uy - ly) / k + 1;

	// Calculating the values of the table
	for (int i = 0; i < nx; ++i) {
		for (int j = 0; j < ny; ++j) {
				z[i][j] = givenFunction(lx + i * h,
																ly + j * k);
		}
	}

	// Calculating the integral value
	// wrt y at each point for x
	for (int i = 0; i < nx; ++i) {
		ax[i] = 0;
		for (int j = 0; j < ny; ++j) {
			if (j == 0 || j == ny - 1)
				ax[i] += z[i][j];
			else if (j % 2 == 0)
				ax[i] += 2 * z[i][j];
			else
				ax[i] += 4 * z[i][j];
		}
		ax[i] *= (k / 3);
	}

	answer = 0;

	// Calculating the final integral value
	// using the integral obtained in the above step
	for (int i = 0; i < nx; ++i) {
			if (i == 0 || i == nx - 1)
				answer += ax[i];
			else if (i % 2 == 0)
				answer += 2 * ax[i];
			else
				answer += 4 * ax[i];
	}
	answer *= (h / 3);

	return answer;
}

double integral(double(*f)(double x), double a, double b, int n) {
	double step = (b - a) / n;  // width of each small rectangle
	double area = 0.0;  // signed area
	for (int i = 0; i < n; i ++) {
			area += f(a + (i + 0.5) * step) * step; // sum up each small rectangle
	}
	return area;
}

int main() {

	srand((unsigned) time(0));
	int myArr[sz];
	int myArr2[sz][PAD];
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

	// ================ Init Parallel do array ==================
	auto t1 = std::chrono::high_resolution_clock::now();

	#pragma omp parallel for
	for(int i=0;i<sz;i++){
		myArr[i] = rand()%100;
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = (std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count());
	float time = (float)duration/1000000;

	std::cout << "Init Parallel: " << time << " microsegundos." << std::endl;
	
	// ================ Init Parallel do array ==================
	auto t3 = std::chrono::high_resolution_clock::now();

	int nthreads;  

	#pragma omp parallel
	{
		int i, id, NT;
		double x;
		id = omp_get_thread_num();
		NT = omp_get_num_threads();
		if (id == 0){
			nthreads = NT;
		}

		for(int i=id; i<sz; i=i+NUM_THREADS){
			myArr2[i][0] = rand()%100;
		}
	}

	auto t4 = std::chrono::high_resolution_clock::now();
	auto duration2 = (std::chrono::duration_cast<std::chrono::microseconds>( t4 - t3 ).count());
	float time2 = (float)duration2/1000000;

	std::cout << "Init Parallel Pad: " << time2 << " microsegundos." << std::endl;



	auto tIntegral = std::chrono::high_resolution_clock::now();

	// lx and ux are upper and lower limit of x integral
	// ly and uy are upper and lower limit of y integral
	// h is the step size for integration wrt x
	// k is the step size for integration wrt y
	float h, k, lx, ux, ly, uy;
  
	lx = 0.0, ux = 1.0, ly = 0.0,
	uy = 1.0, h = 0.1, k = 0.15;

	printf("Integral: %f\n", doubleIntegral(h, k, lx, ux, ly, uy));	

	auto tIntegral2 = std::chrono::high_resolution_clock::now();
	auto durationInt = (std::chrono::duration_cast<std::chrono::microseconds>( tIntegral2 - tIntegral ).count());
	float timeInt = (float)durationInt/1000000;

	std::cout << "Time Integral: " << timeInt << " microsegundos." << std::endl;


	cout.precision(7);
	cout << integral(cos, 0, M_PI / 2, 10) << endl;

}
