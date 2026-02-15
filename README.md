# Operating Systems & Concurrency Portfolio

## Overview
This repository contains C programs developed for my Computer Engineering coursework. They demonstrate core OS concepts including **multithreading**, **process synchronization**, **semaphores**, and **inter-process communication**.

## Project List

### 1. Matrix Multiplication (`matrix_multiplication.c`)
* **Concepts:** Multithreading (pthreads), Performance Analysis, File I/O.
* **Description:** Performs matrix multiplication using both serial and parallel methods. It reads input from `matrixA.txt` and `matrixB.txt` and compares execution time.

### 2. Process Launcher (`process_launcher.c` & `student_info.c`)
* **Concepts:** `fork()`, `execv()`, Process Images.
* **Description:** A parent process forks a child and uses `execv` to replace the child's memory image with a separate executable.
* **How to Run:**
    The launcher looks for an executable named `myname`. You must compile `student_info.c` with that specific output name:
    ```bash
    gcc student_info.c -o myname   # Compile target as 'myname'
    gcc process_launcher.c -o app  # Compile launcher
    ./app                          # Run the system
    ```

### 3. Synchronization & Semaphores
* **`producer_consumer.c`:** Solves the Producer-Consumer problem using a shared buffer.
* **`sleeping_barber.c`:** Simulates the Sleeping Barber synchronization problem.
* **`dining_philosophers.c`:** A solution to the Dining Philosophers problem (avoiding deadlock).

## Compile & Run
To run the matrix project:
```bash
gcc -pthread matrix_multiplication.c -o matrix
./matrix
