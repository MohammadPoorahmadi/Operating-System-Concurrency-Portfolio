#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREAD 4
#define ROW_A 2
#define COL_A 3
#define ROW_B 3
#define COL_B 2

// Global matrices (shared between threads)
int **A, **B, **Cs, **Cp;

// Function to fill matrix A with sample data from matrixA.txt and matrixB.txt
void initialize_matrix() {
    FILE *fa = fopen("matrixA.txt","r"); // Read matrix A from matrixA.txt in the format:
    FILE *fb = fopen("matrixB.txt","r"); // 1 2 3
    A = malloc (ROW_A * sizeof(int*));// Use malloc to dynamically allocate A[ROW_A][COL_A]
    for (int i = 0 ; i< ROW_A ; i++){
        A[i] = malloc (COL_A * sizeof(int));
        for (int j = 0; j < COL_A ; j++) fscanf(fa,"%d", &A[i][j]);
    }
    B = malloc (ROW_B * sizeof(int*));
    for(int i = 0 ; i < ROW_B ; i++){
        B[i] = malloc (COL_B * sizeof (int));
        for (int j = 0; j < COL_B ; j++) fscanf(fb,"%d",&B[i][j]);
    }
    fclose(fa);  // Similarly, read matrix B from matrixB.txt:
    fclose(fb);// 7 8
    // 9 10
    // 11 12
    // Use malloc to dynamically allocate B[ROW_B][COL_B]
    // TODO   
    Cs = malloc (ROW_A * sizeof(int*));
    Cp = malloc (ROW_A * sizeof(int*));
    for (int i = 0 ; i < ROW_A ; i++){
        Cs[i] = malloc (COL_B * sizeof(int));
        Cp[i] = malloc (COL_B * sizeof(int));
    }
}

// Save data in a file
void save_matrix() {
    FILE *fc = fopen("matrixC.txt","w");
    for (int i = 0 ; i < ROW_A ; i++){
        for (int j = 0 ; j < COL_B ; j++){
            fprintf (fc,"%d",Cp[i][j]);
        }
        fprintf(fc,"\n");
    }
    fclose(fc);

    // Write the C matrix result into a file matrixC.txt as this format
    // 58    64
    // 139  154

    // TODO    
}
// Function to print a matrix
void print_matrix(int **matrix, int rows,int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Serial function to compute dot product
void dot_serial() {
    for (int i = 0 ; i < ROW_A ; i++){
        for (int j = 0 ; j < COL_B ; j++ ){
            int sum = 0;
            for (int k = 0 ; k < COL_A ; k++){
                sum += A[i][k] * B[k][j];
            }
            Cs[i][j] = sum;
        }
    }
    // dot product logic here
    // TODO
}

// Multithreaded dot product worker function
void *dot_worker(void *arg) {    
    int id = *((int*)arg);
    int r = id / COL_B;
    int c = id % COL_B;
    int sum = 0;
    for (int k = 0 ; k < COL_A ; k++){
        sum += A[r][k] * B[k][c];
    }
    Cp [r][c] = sum;
    //TODO 
    //TODO : compute one element Cp[r][c] = sum(A[r][k] * B[k][c])
    pthread_exit(NULL);
}


// Timing helper function
double get_time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main() {
    clock_t start,end;
    
    double ts, tp;

    // Initialize matrix A and B
    initialize_matrix();        // Read matrix from a file
    printf("Matrix A:\n");
    print_matrix(A,ROW_A,COL_A);
	printf("Matrix B:\n");
    print_matrix(B,ROW_B,COL_B);
    // Serial dot product
    
    start = clock();
    dot_serial();
    end = clock();
    ts = ((double)(end - start) / CLOCKS_PER_SEC);
    printf("Serial dot product Matrix C:\n");
    print_matrix(Cs,ROW_A,COL_B);
    printf("Serial dot product time: %f seconds\n", ts);
    FILE *fs = fopen("result_serial.txt", "w");
    fprintf(fs, "Serial Matrix C:\n");
    for (int i = 0; i < ROW_A; i++) {
        for (int j = 0; j < COL_B; j++) {
            fprintf(fs, "%d ", Cs[i][j]);
        }
    fprintf(fs, "\n");
    }
    fprintf(fs, "\nSerial Execution Time: %f seconds\n", ts);
    fclose(fs);


    start = clock();
    // **Multithreaded product**
    pthread_t dotProduct_threads[NUM_THREAD ];
    int ids [NUM_THREAD] = {0,1,2,3};
    for (int i = 0 ; i < NUM_THREAD ; i++){
        pthread_create(&dotProduct_threads[i], NULL , dot_worker , &ids[i]);
    }
    // Create threads for transposing here using for loop
	// TODO
	for (int i = 0 ; i < NUM_THREAD ;i ++){
        pthread_join(dotProduct_threads[i] , NULL);
    }
    // Join threads here
	// TODO
	end = clock();
    tp = ((double)(end - start) / CLOCKS_PER_SEC);
    printf("Multithreaded dot product Matrix C:\n");
    print_matrix(Cp,ROW_A,COL_B);
    printf("Multithreaded dot product time: %f seconds\n", tp);
    
    FILE *fp = fopen("result_parallel.txt", "w");
    fprintf(fp, "Parallel Matrix C:\n");
    for (int i = 0; i < ROW_A; i++) {
        for (int j = 0; j < COL_B; j++) {
         fprintf(fp, "%d ", Cp[i][j]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\nParallel Execution Time: %f seconds\n", tp);
    fclose(fp);

    save_matrix();  

    return 0;
}
