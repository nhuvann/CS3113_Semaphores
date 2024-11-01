# CS3113_Semaphores
Author: Nhu Van
Description: In this assignment, four processes share a memory location, with each process attempting independently to increase the shared memory's value from 1 to a specified target by increments of one. Process 1 aims to reach 100,000, Process 2 targets 200,000, Process 3 aims for 300,000, and Process 4's goal is 500,000. Thus, when the program concludes, the shared memory variable will hold a total value of 1,100,000, which will be displayed by the process that finishes last.
Once all child processes have completed, the parent process should release the shared memory and semaphore resources and terminate. Utilize the "wait" function to allow the parent to track precisely when each child process completes. As each child finishes, the parent should print the child's process ID. Afterward, it should release the shared memory and semaphore resources and display "End of Program."

#### The goal is to use semaphores to protect critical sections 
## **Compilation Instructions**

1. **Prerequisites:**

   - Make sure you have a **C compiler** installed, such as `gcc`.
   - You should also be using a **UNIX-like environment** (e.g., Linux or macOS) since the program uses system calls specific to UNIX (e.g., `fork()`, `shmget()`, `shmat()`, `waitpid()`).
2. **To run**
   - Clone the code from the github( or import the attached code)
   - In your terminal, cd into the directory CS3113ProgAssign2 with "cd CCS3113ProgAssign2"
   - Ls to check if the file "ProgAssign2Van0006.c" is in the directory
   - To compile use, "gcc ProgAssign2Van0006.c -o runnable"
   - To make changes, "nano ProgAssign2Van0006.c"
   
   - Lastly, "./runnable" to execute the program.
  
3.  **Expected Result**
From Process 1: counter = XXXX.
From Process 2: counter = XXXX.
From Process 3: counter = XXXX.
From Process 4: counter = 1100000
Child with ID XXXX has just exited.
Child with ID XXXX has just exited.
Child with ID XXXX has just exited.
Child with ID XXXX has just exited.
End of Program.

**For every output, there would be a pause before the next one**
 
   
