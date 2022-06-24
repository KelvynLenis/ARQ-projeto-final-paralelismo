#include <iostream>
#include <algorithm>
#include <omp.h>
#include <math.h>
#include <chrono>
#include <set>
#include <vector>

#define NUM_THREADS 4

using namespace std;
static long num_steps = 100;
static const long sz = 10000;
int nthreads;

int search(int arr[], int value){
  for(int i = 0; i < sz; i++){
    if(i == value){
      return i;
    }
  }

  return 0;
}

void searchParallel(int arr[], int value){
  // int slice1[sz / 4];
  // int slice2[sz / 4];
  // int slice3[sz / 4];
  // int slice4[sz / 4];

  int first = sz * 0.25;
  int second = sz * 0.50;
  int third = sz * 0.75;

  // #pragma omp parallel
  // {
  //   #pragma omp for
  //   for(int i = 0; i < first; i++){
  //     slice1[i] = arr[i];
  //   }
  //   #pragma omp for
  //   for(int j = first; j < second; j++){
  //     slice2[j] = arr[j];    
  //   }
  //   #pragma omp for
  //   for(int k = second; k < third; k++){
  //     slice3[k] = arr[k];
  //   }
  //   #pragma omp for
  //   for(int l = third; l < sz; l++){
  //     slice4[l] = arr[l];
  //   }
  // }

  // #pragma omp parallel for shared(arr)
  // for(int i = 0; i < first; i++){
  //   slice1[i] = arr[i];
  // }
  // for(int j = first; j < second; j++){
  //   slice2[j] = arr[j];    
  // }
  // for(int k = second; k < third; k++){
  //   slice3[k] = arr[k];
  // }
  // for(int l = third; l < sz; l++){
  //   slice4[l] = arr[l];
  // }

  #pragma omp parallel shared(arr)
  {
    // busca
    #pragma omp for nowait
    for(int i = 0; i < first; i++){
      if(arr[i] == value){
        // cout <<  i << endl;
        #pragma exit searchParallel 30;
      }
    }
    #pragma omp for nowait
    for(int j = first; j < second; j++){
      if(arr[j] == value){
        // cout << j << endl;
        #pragma exit searchParallel 30;
      }    
    }
    #pragma omp for nowait
    for(int k = second; k < third; k++){
      if(arr[k] == value){
        #pragma exit searchParallel 30;
        // cout << k << endl;
      }
    }
    #pragma omp for nowait
    for(int l = third; l < sz; l++){
      if(arr[l] == value){
        #pragma exit searchParallel 30;
        // cout << l << endl;
      }
    }
  }
}

vector<int> init(){
  vector<int> vec(sz, 0);
  for(int i = 0; i < sz; i++) {
    vec[i] = rand()%100;
  }

  return vec;
}

vector<int> initParallel(){
  vector<int> vec(sz, 0);

  #pragma omp parallel shared(vec)
  {
    #pragma omp for
    for(int i = 0; i < (sz / 2); i++) {
      vec[i] = rand()%100;
    }
    #pragma omp for 
    for(int j = (sz / 2); j < sz; j++){
      vec[j] = rand()%100;
    }
  } 

  return vec;
}

int searchVector(vector<int> vec, int value){
  for( auto i : vec){
    if(i == value){
      return value;
    }
  }
  return 0;
}

int searchVectorParallel(vector<int> vec, int value){
  #pragma omp parallel
  {
    for(auto i : vec){
      if(i == value){
      }
    }
  }
  return value;
}

int main() {

	srand((unsigned) time(0));

	int myArr[sz];
  vector<int> myVec;

	omp_set_num_threads(NUM_THREADS);
	auto arr_size = sizeof(myArr) / sizeof(myArr[0]);

	// ================ Inicialização do array ==================
	auto tinit= std::chrono::high_resolution_clock::now();

	for(int i=0;i<sz;i++){
		myArr[i] = rand()%100;
	}

	auto tinit2 = std::chrono::high_resolution_clock::now();
	auto durationInit = (chrono::duration_cast<chrono::microseconds>( tinit2 - tinit ).count());
	float timeInit = (float)durationInit/1000000;
	cout << "Init Array time: " << timeInit << endl;

  // ================== Inicialização do Vetor ==================

  auto tinitVector = std::chrono::high_resolution_clock::now();

  vector<int> sequential;
  sequential = init();

	auto tinitVector2 = std::chrono::high_resolution_clock::now();
	auto durationInitVector = (chrono::duration_cast<chrono::microseconds>( tinitVector2 - tinitVector ).count());
	float timeInitVector = (float)durationInitVector/1000000;
	cout << "Init Vector sequential time: " << timeInit << endl;

	// ========= Inicialização do Vetor paralela ==================

  auto tinitParallel = std::chrono::high_resolution_clock::now();

  vector<int> parallel;
  parallel = initParallel();

	auto tinitParallel2 = std::chrono::high_resolution_clock::now();
	auto durationInitParallel = (chrono::duration_cast<chrono::microseconds>( tinitParallel2 - tinitParallel ).count());
	float timeInitParallel = (float)durationInitParallel/1000000;
	cout << "Init Vector Parallel time: " << timeInit << endl;
  

	// ================ Busca no array ==================
	auto t1 = std::chrono::high_resolution_clock::now();

	// for(int i = 0; i < num_steps; i++){
  //   search(myArr, 7);
	// }
  searchVector(sequential, 8);

	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = (std::chrono::duration_cast<std::chrono::nanoseconds>( t2 - t1 ).count());
	float time = (float)duration/1000000;

	std::cout << "Tempo de Busca sequential = " << time << " segundos." << std::endl;


  // =================== Busca Paralela ==========================
	auto tParallel1 = std::chrono::high_resolution_clock::now();

	for(int i = 0; i < num_steps; i++){
    searchParallel(myArr, 7);
	}

	auto tParallel2 = std::chrono::high_resolution_clock::now();
	auto durationParallel = (std::chrono::duration_cast<std::chrono::nanoseconds>( tParallel2 - tParallel1 ).count());
	float timeParallel = (float)durationParallel/1000000;

	std::cout << "Tempo de processamento paralelo = " << time << " segundos." << std::endl;

}
