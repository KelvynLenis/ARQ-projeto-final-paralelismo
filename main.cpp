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
int nthreads; 

void swap(int* a, int* b){
    int t = *a;
    *a = *b;
    *b = t;
}
 
/* This function takes last element as pivot, places
the pivot element at its correct position in sorted
array, and places all smaller (smaller than pivot)
to left of pivot and all greater elements to right
of pivot */
int partition (int arr[], int low, int high){
	int pivot = arr[high]; // pivot
	int i = (low - 1); // Index of smaller element and indicates the right position of pivot found so far

	for (int j = low; j <= high - 1; j++){
		// If current element is smaller than the pivot
		if (arr[j] < pivot){
			i++; // increment index of smaller element
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}
 
/* The main function that implements QuickSort
arr[] --> Array to be sorted,
low --> Starting index,
high --> Ending index */
void quickSort(int arr[], int low, int high){
	if (low < high){
		#pragma omp parallel shared(arr, low, high)
		{
			/* pi is partitioning index, arr[p] is now
			at right place */
			int pi = partition(arr, low, high);

			// Separately sort elements before
			// partition and after partition
			#pragma omp nowait
			quickSort(arr, low, pi - 1);
			#pragma omp
			quickSort(arr, pi + 1, high);
		}
	}
}

void merge(int array[], int const left, int const mid, int const right){
	auto const subArrayOne = mid - left + 1;
	auto const subArrayTwo = right - mid;

	// Create temp arrays
	auto *leftArray = new int[subArrayOne],
				*rightArray = new int[subArrayTwo];

	// Copy data to temp arrays leftArray[] and rightArray[]
	#pragma omp parallel shared(array)
	{
		#pragma omp for nowait
		for (auto i = 0; i < subArrayOne; i++){
			leftArray[i] = array[left + i];
		}

		#pragma omp for nowait
		for (auto j = 0; j < subArrayTwo; j++){
			rightArray[j] = array[mid + 1 + j];
		}
 
		auto indexOfSubArrayOne = 0; // Initial index of first sub-array
		auto indexOfSubArrayTwo = 0; // Initial index of second sub-array
		int indexOfMergedArray = left; // Initial index of merged array

		// Merge the temp arrays back into array[left..right]
		#pragma omp while
		while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
			if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
					array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
					indexOfSubArrayOne++;
			}
			else {
					array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
					indexOfSubArrayTwo++;
			}
			indexOfMergedArray++;
		}
		// Copy the remaining elements of
		// left[], if there are any
		#pragma omp while nowait shared(indexOfMergedArray)
		while (indexOfSubArrayOne < subArrayOne) {
			array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
			indexOfSubArrayOne++;
			indexOfMergedArray++;
		}
		// Copy the remaining elements of
		// right[], if there are any
		#pragma omp while nowait shared(indexOfMergedArray)
		while (indexOfSubArrayTwo < subArrayTwo) {
			array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
			indexOfSubArrayTwo++;
			indexOfMergedArray++;
		}
	}
}

void mergeSort(int array[], int const begin, int const end){
	if (begin >= end)
			return; // Returns recursively

	auto mid = begin + (end - begin) / 2;

	// Com paralelismo
	#pragma omp parallel shared(array, mid, begin, end)
	{
		#pragma omp nowait
		mergeSort(array, begin, mid);
		#pragma omp nowait
		mergeSort(array, mid + 1, end);
		#pragma omp nowait
		merge(array, begin, mid, end);
	}

	//  Sem paralelismo
	// mergeSort(array, begin, mid);
	// mergeSort(array, mid + 1, end);
	// merge(array, begin, mid, end);

}

int main() {

	srand((unsigned) time(0));
	const int sz = 1000000;
	int myArr[sz];
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
	cout << "Init time: " << timeInit << endl;

	// ================ Ordenação do array ==================
	auto t1 = std::chrono::high_resolution_clock::now();

	for(int i = 0; i < num_steps; i++){
		// mergeSort(myArr, 0, arr_size - 1);
		quickSort(myArr, 0, arr_size -1);
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = (std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count());
	float time = (float)duration/1000000;

	std::cout << "Tempo de processamento = " << time << " segundos." << std::endl;

	// #pragma omp parallel
	// {
	// 	#pragma omp for
	// 	for(int i = 0; i < 10; i++){
	// 		cout << "i: " << i << endl;
	// 	}
	// }

}
