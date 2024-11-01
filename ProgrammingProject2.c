//Author: Nhu Van
//van0006
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <errno.h>

/* semaphore and shared memory keys */
#define SHM_KEY ((key_t)1497)
#define SEM_KEY ((key_t)400L)
#define NSEMS 1

/* GLOBAL */
int sem_id; /* semaphore id */
/* semaphore buffers */
static struct sembuf OP = {0, -1, 0};
static struct sembuf OV = {0, 1, 0};
struct sembuf *P = &OP;
struct sembuf *V = &OV;
/* semaphore union used to generate semaphore */
typedef union {
    int val;
    struct semid_ds *buf;
    ushort *array;
} semunion;

/* POP (wait) function for semaphore to protect critical section */
int POP() {
    int status;
    status = semop(sem_id, P, 1);
    return status;
}

/* VOP (signal) function for semaphore to release protection */
int VOP() {
    int status;
    status = semop(sem_id, V, 1);
    return status;
}

typedef struct {
    int value;   // This will be the shared variable
} shared_mem;

shared_mem *total;

// Process functions
void process1(shared_mem *total, int ID);
void process2(shared_mem *total, int ID);
void process3(shared_mem *total, int ID);
void process4(shared_mem *total, int ID);

int main() {
    int shmid;
    pid_t pid1, pid2, pid3, pid4;
    int status;
    char *shmadd  =  (char*)0;
    semunion semctl_arg;
    semctl_arg.val = 1;

    // Create semaphore
    sem_id = semget(SEM_KEY, NSEMS, IPC_CREAT | 0666);
    if (sem_id < 0) {
        perror("Error in creating the semaphore");
        exit(1);
    }

    // Initialize semaphore
    if (semctl(sem_id, 0, SETVAL, semctl_arg) < 0) {
        perror("Error setting semaphore value");
        exit(1);
    }

    // Create shared memory segment
    if ((shmid = shmget(SHM_KEY, sizeof(shared_mem), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory to the current process's address space
    if ((total = (shared_mem *)shmat(shmid, shmadd, 0)) == (shared_mem *)-1) {
        perror("shmat");
        exit(0);
    }

    // Initializing total
    total->value = 0;

    // Creating child process 1
    if ((pid1 = fork()) == 0) {
        process1(total, 1);
        exit(0);
    }
    // Creating process 2
    if ((pid2 = fork()) == 0) {
        process2(total, 2);
        exit(0);
    }
    // Creating process 3
    if ((pid3 = fork()) == 0) {
        process3(total, 3);
        exit(0);
    }
    // Creating process 4
    if ((pid4 = fork()) == 0) {
        process4(total, 4);
        exit(0);
    }

    // Wait for all child processes to complete before printing exit messages
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    waitpid(pid3, &status, 0);
    waitpid(pid4, &status, 0);

    // Print child exit messages after all processes have finished
    printf("Child 1 with PID %d has exited.\n", pid1);
    printf("Child 2 with PID %d has exited.\n", pid2);
    printf("Child 3 with PID %d has exited.\n", pid3);
    printf("Child 4 with PID %d has exited.\n", pid4);

    // Detach shared memory
    if (shmdt(total) == -1) {
        perror("shmdt");
        exit(-1);
    }

    // Remove shared memory
    shmctl(shmid, IPC_RMID, NULL);

    // Remove semaphore
    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        perror("Error removing semaphore");
        exit(1);
    }

    printf("End of program\n");
    return 0;
}

// Process 1
void process1(shared_mem *total, int ID) {
    for (int i = 0; i < 100000; i++) {
        POP();  // Enter critical section
        total->value += 1;
        VOP();  // Leave critical section
    }
    printf("From Process %d: total value = %d.\n", ID, total->value);
}

// Process 2
void process2(shared_mem *total, int ID) {
    for (int i = 0; i < 200000; i++) {
        POP();  // Enter critical section
        total->value += 1;
        VOP();  // Leave critical section
    }
    printf("From Process %d: total value = %d.\n", ID, total->value);
}

// Process 3
void process3(shared_mem *total, int ID) {
    for (int i = 0; i < 300000; i++) {
        POP();  // Enter critical section
        total->value += 1;
        VOP();  // Leave critical section
    }
    printf("From Process %d: total value = %d.\n", ID, total->value);
}

// Process 4
void process4(shared_mem *total, int ID) {
    for (int i = 0; i < 500000; i++) {
        POP();  // Enter critical section
        total->value += 1;
        VOP();  // Leave critical section
    }
    printf("From Process %d: total value = %d.\n", ID, total->value);
}
