//
//  main.cpp
//  ThreadsTest
//
//  Created by mndx on 16/02/2022.
//  Playing with threads.
//

#include <chrono>
#include <iostream>
#include <pthread.h>

#define NUM_THREADS 4

typedef struct user_type {
    int arg;
    long data;
} u_type;

void * runner(void * args_and_data) {
    
    // get arguments
    u_type * var = (u_type *) args_and_data;
    
    // do something intensive
    for(long i = 0; i < (long) 6 * INT_MAX; ++i) {}

    // set data
    var->data = var->arg / 2;
    
    // exit success
    pthread_exit(0);
}

int main(int argc, const char * argv[]) {
    
    // declarations
    pthread_attr_t thread_attr[NUM_THREADS];
    pthread_t thread_ids[NUM_THREADS];
    u_type * args_and_data_vec = new u_type[NUM_THREADS];
    
    // set runner arguments
    for (int i = 0; i < NUM_THREADS; ++i)
        args_and_data_vec[i].arg = 2 * (i + 1);
    
    // start timing
    auto start_timing = std::chrono::steady_clock::now();
    
    // create threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        // set the default attributes of the thread
        pthread_attr_init(&thread_attr[i]);
        // create the thread
        pthread_create(&thread_ids[i], &thread_attr[i], runner, &args_and_data_vec[i]);
    }

    // wait for the threads to exit
    for (int i = 0; i < NUM_THREADS; ++i)
        pthread_join(thread_ids[i], NULL);
    
    // end timing
    auto end_timing = std::chrono::steady_clock::now();

    // store the time difference between start and end in (s)
    auto time = std::chrono::duration_cast<std::chrono::seconds>(end_timing - start_timing);
    
    // print data
    for (int i = 0; i < NUM_THREADS; ++i)
        std::cout << i << ": " << args_and_data_vec[i].data << std::endl;
    
    // print timing
    std::cout << "time: " << time.count() << " (s)" << std::endl;
    
    return 0;
}
