#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 4
#define UPPER 10
#define LOWER 2

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

int matSumResult[MAX][MAX]; // matrix to hold sum result
int matDiffResult[MAX][MAX]; // matrix to hold difference result
int matProductResult[MAX][MAX]; // matrix to hold product result

typedef void* (*Operators)(void*);

void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) {
	int *index = (int*) args;
	int row = (*index)/MAX;
	int col = (*index)%MAX;
	
	matSumResult[row][col] = matA[row][col] + matB[row][col];
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) {
  int *index = (int*) args;
	int row = (*index)/MAX;
	int col = (*index)%MAX;
	
	matDiffResult[row][col] = matA[row][col] - matB[row][col];
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) {
	int *index = (int*) args;
	int row = (*index)/MAX;
	int col = (*index)%MAX;
	
	matProductResult[row][col] = matA[row][col] * matB[row][col];
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    
    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
		fillMatrix(matB);

    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads.
    pthread_t thread_pool[3][MAX*MAX];
	  int i, j;
		
		Operators operators[3];
		operators[0] = &computeSum;
    operators[1] = &computeProduct;
    operators[2] = &computeDiff;
  
		// 4. Create a thread for each cell of each matrix operation.
    // 
    // You'll need to pass in the coordinates of the cell you want the thread
    // to compute. One way to do this is to "encode" the coordinates into a number
    // and then pass it into the thread function. For example,
    // 
    // long coordinates = MAX*row + col;
    // 
    // Then the thread will have to "decode" that number back into the row and col.
    // 
    // Another option is to malloc memory for the coordinates, populate the coordinates
    // into that space, and pass that address to the thread. The thread will then have
    // to free that space when it's done with what's in that memory.
    
		for(i = 0; i < 3; i++){
			for(j = 0; j < MAX*MAX; j++){
        // as thread takes the address of i and j, the values
        // might change, so making a copy and passing it to thread
        int *function_index = (int*)malloc(sizeof(int));
        int *index = (int*)malloc(sizeof(int));

        *function_index = i;
        *index = j; 
				
        // Creating thread for matrix operation for every cell
        pthread_create(&thread_pool[*function_index][*index], NULL, operators[*function_index], (void*)index);
			}
		}
   
    // 5. Wait for all threads to finish.
    for (i = 0; i < 3; i++){
      for (j = 0; j < MAX*MAX; j++){
        pthread_join(thread_pool[i][j], NULL);
      }
    }
  
    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;
}