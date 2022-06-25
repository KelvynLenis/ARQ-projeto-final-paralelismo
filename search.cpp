#include <iostream>
#include <algorithm>
#include <omp.h>
#include <math.h>
#include <chrono>
#include <set>
#include <vector>

#define NUM_THREADS 4
#define PAD 8  // assume 64 byte L1 cache line size

using namespace std;
static long num_steps = 100;
static const long sz = 1000000;
int nthreads;

int search(int arr[], int value){
  for(int i = 0; i < sz; i++){
    if(i == value){
      // cout << i << endl;
      return 1;
    }
  }

  return 0;
}

void parallel_search (int slice1[], int slice2[], int value) {
  #pragma omp parallel
  {
    int nthd=omp_get_thread_num();
    
    //check whether cancellation has been activated before proceed
    #pragma omp cancellation point parallel
    bool found = search(slice1, value);
  
    if(found){
        //cancel the parallel construct
        #pragma omp cancel parallel
    }

    //check whether cancellation has been activated before proceed
    #pragma omp cancellation point parallel

    //If the first level search is not successful, go on to the second level search
    search(slice2, value); 
  }
}

void searchParallel(int arr[], int value){
  int i;
  #pragma omp parallel default(none) shared(arr, value) private(i)
  {
    #pragma omp for nowait
    for(i = 0; i < sz/2; i++){
      if (arr[i] == value)
      {
        /* code */
      }
      
    }

    #pragma omp for nowait
    for(i = sz/2; i < sz; i++){
      if (arr[i] == value)
      {
        /* code */
      }

    }
  }
}

void initArray(int arr[]){
  omp_set_num_threads(NUM_THREADS);

  int i;

  #pragma omp parallel default(none) shared(arr) private(i)
  {
    // #pragma omp for nowait
    for(i = 0; i < sz; i++){
      arr[i] =  rand()%100;
    }
    
    // #pragma omp for nowait
    // for(i = first; i < second; j++){
    //   arr[i] =  rand()%100;    
    // }
    // #pragma omp for nowait
    // for(i = second; i < third; k++){
    //   arr[i] =  rand()%100;
    // }
    // #pragma omp for nowait
    // for(i = third; i < sz; l++){
    //   arr[i] =  rand()%100;
    // }
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

  #pragma omp parallel // shared(vec)
  {
    #pragma omp for nowait
    for(int i = 0; i < (sz / 2); i++) {
      vec[i] = rand()%100;
    }
    #pragma omp for nowait
    for(int j = (sz / 2); j < (sz / 3); j++){
      vec[j] = rand()%100;
    }
    #pragma omp for nowait
    for(int k = (sz / 3); k < sz; k++){
      vec[k] = rand()%100;
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
  // int first = sz * 0.25;
  // int second = sz * 0.50;
  // int third = sz * 0.75;

  // vector<int> firstPart;
  // vector<int> secondPart;
  // vector<int> thirdPart;
  // vector<int> fourthPart;

  // copy(vec.begin(), vec.begin() + first, firstPart);
  // copy(vec.begin() + first, vec.begin() + second, secondPart);
  // copy(vec.begin() + second, vec.begin() + third, thirdPart);
  // copy(vec.begin() + third, vec.end(), fourthPart);

  // #pragma omp parallel
  // {
  //   #pragma omp for nowait 
  //   for(auto i : firstPart){
  //     if(i == value){
  //     }
  //   }
  //   #pragma omp for nowait
  //   for(auto j : secondPart){
  //     if(j == value){
  //     }
  //   }
  //   #pragma omp for nowait
  //   for(auto k : thirdPart){
  //     if(k == value){
  //     }
  //   }
  //   #pragma omp for nowait
  //   for(auto l : fourthPart){
  //     if(l == value){
  //     }
  //   }
  // }

  for(auto x : vec){
    if(x == value){
      return x;
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

  // ============= Inicialização paralela do array =============
	auto tinitParallelArray = std::chrono::high_resolution_clock::now();

	initArray(myArr);

	auto tinitParallelArray2 = std::chrono::high_resolution_clock::now();
	auto durationInitParallelArray = (chrono::duration_cast<chrono::microseconds>( tinitParallelArray2 - tinitParallelArray ).count());
	float timeInitParallelArray = (float)durationInitParallelArray/1000000;
	cout << "Init Parallel Array time: " << timeInitParallelArray << endl;

  // ================== Inicialização do Vetor ==================

  // auto tinitVector = std::chrono::high_resolution_clock::now();

  // vector<int> sequential;
  // for(int i = 0; i < num_steps; i++){
  //   sequential = init();
  // }

	// auto tinitVector2 = std::chrono::high_resolution_clock::now();
	// auto durationInitVector = (chrono::duration_cast<chrono::microseconds>( tinitVector2 - tinitVector ).count());
	// float timeInitVector = (float)durationInitVector/1000000;
	// cout << "Init Vector sequential time: " << timeInitVector << endl;

	// ========= Inicialização do Vetor paralela ==================

  // auto tinitParallel = std::chrono::high_resolution_clock::now();

  // vector<int> parallel;
  // parallel = initParallel();

	// auto tinitParallel2 = std::chrono::high_resolution_clock::now();
	// auto durationInitParallel = (chrono::duration_cast<chrono::microseconds>( tinitParallel2 - tinitParallel ).count());
	// float timeInitParallel = (float)durationInitParallel/1000000;
	// cout << "Init Vector Parallel time: " << timeInitParallel << endl;
  

	// ================ Busca no array ==================
	auto t1 = std::chrono::high_resolution_clock::now();

	for(int i = 0; i < num_steps; i++){
    search(myArr, 7);
  // searchVector(sequential, 8);
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = (std::chrono::duration_cast<std::chrono::nanoseconds>( t2 - t1 ).count());
	float time = (float)duration/1000000;

	std::cout << "Tempo de Busca sequential = " << time << " segundos." << std::endl;


  // =================== Busca Paralela ==========================
	auto tParallel1 = std::chrono::high_resolution_clock::now();

	for(int i = 0; i < num_steps; i++){
    searchParallel(myArr, 7);
    // searchVectorParallel(parallel, 7);
	}

	auto tParallel2 = std::chrono::high_resolution_clock::now();
	auto durationParallel = (std::chrono::duration_cast<std::chrono::nanoseconds>( tParallel2 - tParallel1 ).count());
	float timeParallel = (float)durationParallel/1000000;

	std::cout << "Tempo de Busca paralela = " << timeParallel << " segundos." << std::endl;

}
