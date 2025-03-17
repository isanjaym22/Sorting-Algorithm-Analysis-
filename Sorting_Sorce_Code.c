#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define the maximum number of data set sizes you want to support
#define MAX_DATA_SIZES 10

// Define the data set sizes
int dataSizes[MAX_DATA_SIZES] = {10, 50, 100, 1000, 5000, 10000, 50000, 100000, 250000, 500000};

typedef struct {
    int size;
    double time;
    long long comparisons;
} Result;

void generateRandomData(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand();
    }
}

// Sorting algorithms implementations
long long insertionSortComparisons;
void insertionSort(int *arr, int size) {
    insertionSortComparisons = 0;
    for (int i = 1; i < size; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            insertionSortComparisons++; // Count every comparison
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
}

long long selectionSortComparisons;
void selectionSort(int *arr, int size) {
    selectionSortComparisons = 0;
    for (int i = 0; i < size - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < size; j++) {
            selectionSortComparisons++;
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        int temp = arr[minIndex];
        arr[minIndex] = arr[i];
        arr[i] = temp;
    }
}

long long mergeSortComparisons;
void merge(int *arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        mergeSortComparisons++;
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L);
    free(R);
}

void mergeSort(int *arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

long long quickSortComparisons;
void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int *arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        quickSortComparisons++;
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

long long heapSortComparisons;
void heapify(int *arr, int size, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    // Count every comparison correctly
    if (left < size) {
        heapSortComparisons++;
        if (arr[left] > arr[largest]) {
            largest = left;
        }
    }
    
    if (right < size) {
        heapSortComparisons++;
        if (arr[right] > arr[largest]) {
            largest = right;
        }
    }
    
    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, size, largest);
    }
}

void heapSort(int *arr, int size) {
    heapSortComparisons = 0;
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify(arr, size, i);
    }
    for (int i = size - 1; i >= 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

long long bubbleSortComparisons;
void bubbleSort(int *arr, int size) {
    bubbleSortComparisons = 0;
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            bubbleSortComparisons++;
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

// Function to write results to CSV file
void writeResultsToCSV(Result* results[], int numSizes) {
    FILE *fp = fopen("sorting_algorithm_comparison.csv", "w");
    if (fp == NULL) {
        printf("Failed to open file for writing.\n");
        return;
    }
    
    const char* algorithmNames[] = {
        "Bubble Sort", 
        "Insertion Sort", 
        "Selection Sort", 
        "Quick Sort", 
        "Merge Sort", 
        "Heap Sort"
    };
    
    // For each algorithm, create a separate table
    for (int alg = 0; alg < 6; alg++) {
        // Write algorithm name as header
        fprintf(fp, "%s\n", algorithmNames[alg]);
        
        // Write table headers
        fprintf(fp, "data size,time,comparison\n");
        
        // Write all data sizes for this algorithm
        for (int i = 0; i < numSizes; i++) {
            fprintf(fp, "%d,%.6f,%lld\n", 
                results[alg][i].size, 
                results[alg][i].time, 
                results[alg][i].comparisons);
        }
        
        // Add empty line between tables
        fprintf(fp, "\n");
    }
    
    fclose(fp);
    printf("Results written to sorting_algorithm_comparison.csv\n");
}

// Function to measure all sorting algorithms
void measureSorts(int numSizes) {
    // Allocate arrays for results
    Result *results[6];
    results[0] = (Result *)malloc(numSizes * sizeof(Result)); // Bubble
    results[1] = (Result *)malloc(numSizes * sizeof(Result)); // Insertion
    results[2] = (Result *)malloc(numSizes * sizeof(Result)); // Selection
    results[3] = (Result *)malloc(numSizes * sizeof(Result)); // Quick
    results[4] = (Result *)malloc(numSizes * sizeof(Result)); // Merge
    results[5] = (Result *)malloc(numSizes * sizeof(Result)); // Heap
    
    for (int i = 0; i < numSizes; i++) {
        int size = dataSizes[i];
        printf("Processing data size: %d\n", size);
        
        int *data = (int *)malloc(size * sizeof(int));
        int *copy = (int *)malloc(size * sizeof(int));
        
        generateRandomData(data, size);
        for (int j = 0; j < size; j++) copy[j] = data[j];
        
        clock_t start, end;
        
        // Bubble Sort
        for (int j = 0; j < size; j++) data[j] = copy[j];
        start = clock();
        bubbleSort(data, size);
        end = clock();
        results[0][i] = (Result){size, ((double)(end - start)) / CLOCKS_PER_SEC, bubbleSortComparisons};
        
        // Insertion Sort
        for (int j = 0; j < size; j++) data[j] = copy[j];
        start = clock();
        insertionSort(data, size);
        end = clock();
        results[1][i] = (Result){size, ((double)(end - start)) / CLOCKS_PER_SEC, insertionSortComparisons};
        
        // Selection Sort
        for (int j = 0; j < size; j++) data[j] = copy[j];
        start = clock();
        selectionSort(data, size);
        end = clock();
        results[2][i] = (Result){size, ((double)(end - start)) / CLOCKS_PER_SEC, selectionSortComparisons};
        
        // Quick Sort
        for (int j = 0; j < size; j++) data[j] = copy[j];
        quickSortComparisons = 0;
        start = clock();
        quickSort(data, 0, size - 1);
        end = clock();
        results[3][i] = (Result){size, ((double)(end - start)) / CLOCKS_PER_SEC, quickSortComparisons};
        
        // Merge Sort
        for (int j = 0; j < size; j++) data[j] = copy[j];
        mergeSortComparisons = 0;
        start = clock();
        mergeSort(data, 0, size - 1);
        end = clock();
        results[4][i] = (Result){size, ((double)(end - start)) / CLOCKS_PER_SEC, mergeSortComparisons};
        
        // Heap Sort
        for (int j = 0; j < size; j++) data[j] = copy[j];
        start = clock();
        heapSort(data, size);
        end = clock();
        results[5][i] = (Result){size, ((double)(end - start)) / CLOCKS_PER_SEC, heapSortComparisons};
        
        free(data);
        free(copy);
    }
    
    // Print results for each sorting algorithm
    printf("\nBubble Sort Results:\n");
    for (int i = 0; i < numSizes; i++) {
        printf("Size: %d\tTime: %f\tComparisons: %lld\n", results[0][i].size, results[0][i].time, results[0][i].comparisons);
    }
    
    printf("\nInsertion Sort Results:\n");
    for (int i = 0; i < numSizes; i++) {
        printf("Size: %d\tTime: %f\tComparisons: %lld\n", results[1][i].size, results[1][i].time, results[1][i].comparisons);
    }

    printf("\nSelection Sort Results:\n");
    for (int i = 0; i < numSizes; i++) {
        printf("Size: %d\tTime: %f\tComparisons: %lld\n", results[2][i].size, results[2][i].time, results[2][i].comparisons);
    }
    
    printf("\nQuick Sort Results:\n");
    for (int i = 0; i < numSizes; i++) {
        printf("Size: %d\tTime: %f\tComparisons: %lld\n", results[3][i].size, results[3][i].time, results[3][i].comparisons);
    }

    printf("\nMerge Sort Results:\n");
    for (int i = 0; i < numSizes; i++) {
        printf("Size: %d\tTime: %f\tComparisons: %lld\n", results[4][i].size, results[4][i].time, results[4][i].comparisons);
    }
    
    printf("\nHeap Sort Results:\n");
    for (int i = 0; i < numSizes; i++) {
        printf("Size: %d\tTime: %f\tComparisons: %lld\n", results[5][i].size, results[5][i].time, results[5][i].comparisons);
    }
    
    // Write results to CSV file
    writeResultsToCSV(results, numSizes);
    
    // Free allocated memory
    for (int i = 0; i < 6; i++) {
        free(results[i]);
    }
}

int main() {
    srand(time(0));
    
    // Use the actual number of initialized data sizes
    int numSizes = 10;
    while (numSizes < MAX_DATA_SIZES && dataSizes[numSizes] > 0) {
        numSizes++;
    }
    
    printf("Starting sorting algorithm comparison for %d different data sizes...\n", numSizes);
    measureSorts(numSizes);
    
    return 0;
}