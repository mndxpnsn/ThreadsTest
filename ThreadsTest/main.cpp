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
#include <thread>

#define NUM_THREADS 4

typedef struct user_type {
    int input;
    long output;
} u_type;

void * runner(void * input_and_output) {
    
    // get input
    u_type * var = (u_type *) input_and_output;
    
    // simulate some intensive computation
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // set output
    var->output = var->input / 2;
    
    // exit success
    pthread_exit(0);
}

int main(int argc, const char * argv[]) {
    
    // declarations
    pthread_attr_t thread_attr[NUM_THREADS];
    pthread_t thread_ids[NUM_THREADS];
    u_type * input_and_output_vec = new u_type[NUM_THREADS];
    
    // set runner arguments
    for (int i = 0; i < NUM_THREADS; ++i)
        input_and_output_vec[i].input = 2 * (i + 1);
    
    // start timing
    auto start_timing = std::chrono::steady_clock::now();
    
    // create and execute threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        // set the default attributes of the thread
        pthread_attr_init(&thread_attr[i]);
        // create the thread
        pthread_create(&thread_ids[i], &thread_attr[i], runner, &input_and_output_vec[i]);
    }

    // wait for the threads to exit
    for (int i = 0; i < NUM_THREADS; ++i)
        pthread_join(thread_ids[i], NULL);
    
    // end timing
    auto end_timing = std::chrono::steady_clock::now();

    // store the time difference between start and end in (s)
    auto time = std::chrono::duration_cast<std::chrono::seconds>(end_timing - start_timing);
    
    // print output
    for (int i = 0; i < NUM_THREADS; ++i)
        std::cout << i << ": " << input_and_output_vec[i].output << std::endl;
    
    // print timing
    std::cout << "time: " << time.count() << " (s)" << std::endl;
    
    return 0;
}
