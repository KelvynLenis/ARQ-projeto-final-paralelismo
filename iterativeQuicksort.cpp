#include <iostream>
#include <algorithm>
#include <omp.h>
#include <math.h>
#include <chrono>
#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>

#define NUM_THREADS 4

using namespace std;
static long num_steps = 5;
static const long sz = 1000000;

int partition(int a[], int start, int end){
  // Pick the rightmost element as a pivot from the array
  int pivot = a[end];

  // elements less than the pivot goes to the left of `pIndex`
  // elements more than the pivot goes to the right of `pIndex`
  // equal elements can go either way
  int pIndex = start;

  // each time we find an element less than or equal to the pivot, `pIndex`
  // is incremented, and that element would be placed before the pivot.
  for (int i = start; i < end; i++){
    if (a[i] <= pivot){
        swap(a[i], a[pIndex]);
        pIndex++;
    }
  }

  // swap `pIndex` with pivot
  swap (a[pIndex], a[end]);

  // return `pIndex` (index of the pivot element)
  return pIndex;
}

// Iterative Quicksort routine
void iterativeQuicksort(int a[], int n){
  // create a stack of `std::pairs` for storing subarray start and end index
  stack<pair<int, int>> s;

  // get the starting and ending index of the given array
  int start = 0;
  int end = n - 1;

  // push the start and end index of the array into the stack
  s.push(make_pair(start, end));

  // loop till stack is empty
  while (!s.empty()){
    // remove top pair from the list and get subarray starting
    // and ending indices
    start = s.top().first, end = s.top().second;
    s.pop();

    // rearrange elements across pivot
    int pivot = partition(a, start, end);

    // push subarray indices containing elements that are
    // less than the current pivot to stack
    if (pivot - 1 > start) {
        s.push(make_pair(start, pivot - 1));
    }

    // push subarray indices containing elements that are
    // more than the current pivot to stack
    if (pivot + 1 < end) {
        s.push(make_pair(pivot + 1, end));
    }
  }
}

int main() {

	srand((unsigned) time(0)); // seed para gerar números aleatórios

	int myArr[sz]; // array
	auto arr_size = sizeof(myArr) / sizeof(myArr[0]); // pivot do array pro quicksort

  
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

  iterativeQuicksort(myArr, arr_size);
	// for(int i = 0; i < num_steps; i++){
	// }

	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = (std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count());
	float time = (float)duration/1000000;

	std::cout << "Tempo de processamento = " << time << " segundos." << std::endl;
}
