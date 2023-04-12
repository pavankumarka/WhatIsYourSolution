# WhatIsYourSolution
Solve challenging queries

/* ================================================================================== *

  Filename:  prod_cons_threads_using_signals_in_Queue.c
   Created:  10 March 2023 12:59:00  IST
    Author:  Pavan Kumar K A
Organization: NA
   Summary:  Program illustrating Producer and consumer threads 
             with signals (to eachother threads) to implement Queue.
             1. When data is written and stack index is updated, producer signals 
                consumer to read data.
             2. When data is read and stack is available, consumer signals
                producer to write data.
Code version: v0.1: create producer consumer threads.
 	    v0.2: Update it for queue and solve overflow underflow index issues. 
 	    v0.3: upgrade threads based on signals generated after data read/write.
 	    v0.4: Error handle: conditions when NOT to acquire lock and to release.
Enhancements: 1. Can use binary semaphore for lock mechanism.
     2. Terminate threads based on conditions.
     3. Implement based on Application Design / requirements.
================================================================================== */
Compiler: gcc 
Command: gcc -Wall -i <inputfile.c> -o <outfile>
Execution: ./<outfile>
