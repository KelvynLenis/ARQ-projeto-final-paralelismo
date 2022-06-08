#include <iostream>
#include <algorithm>
#include <omp.h>
#include <math.h>
#include <chrono>
#include <set>

#define ARRAY_SIZE 100000000
#define ARRAY_VALUE 1231
#define NUM_THREADS 4

using namespace std;

int search(set<int> myset, int value){
    set<int>::iterator it;

    for(auto number : myset){
        if(number == value){
            cout << "Found number ";
            return number;
        }
    }
    return -1;
}

void merge(int array[], int const left, int const mid, int const right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;
 
    // Create temp arrays
    auto *leftArray = new int[subArrayOne],
         *rightArray = new int[subArrayTwo];
 
    // Copy data to temp arrays leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];
 
    auto indexOfSubArrayOne = 0, // Initial index of first sub-array
        indexOfSubArrayTwo = 0; // Initial index of second sub-array
    int indexOfMergedArray = left; // Initial index of merged array
 
    // Merge the temp arrays back into array[left..right]
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
    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}

void mergeSort(int array[], int const begin, int const end){
    if (begin >= end)
        return; // Returns recursively
 
    auto mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
}

int main() {

    srand((unsigned) time(0));

    auto tinit= std::chrono::high_resolution_clock::now();

    const int sz = 20000000;
    int myArr[sz];
    set<int> myset;

    for(int i=0;i<sz;i++){
        myset.insert(rand()%10000);  //Generate number between 0 to 99
        myArr[i]= rand()%100;
    }

    auto tinit2 = std::chrono::high_resolution_clock::now();

    auto durationInit = (chrono::duration_cast<chrono::microseconds>( tinit2 - tinit ).count());
	float timeInit = (float)durationInit/1000000;

    cout << "Init time: " << timeInit << endl;

    auto t1 = std::chrono::high_resolution_clock::now();
    
    cout << search(myset, 217) << endl;

    auto t2 = std::chrono::high_resolution_clock::now();

	auto duration = (std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count());

	float time = (float)duration/1000000;

	std::cout << "Tempo de processamento = " << time << " segundos." << std::endl;

}
