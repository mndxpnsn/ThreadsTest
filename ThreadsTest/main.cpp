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

const int delay_time = 1; // sleep time in seconds

typedef struct user_type {
    int input;
    long output;
} u_type;

void * runner(void * input_and_output) {
    // this function executes in parallel
    
    // get input
    u_type * var = (u_type *) input_and_output;
    
    // simulate some intensive computation
    std::this_thread::sleep_for(std::chrono::seconds(delay_time));

    // set output
    var->output = var->input / 2;
    
    // exit success
    pthread_exit(0);
}

void runner(u_type * input_and_output) {
    // this function executes sequentially
    
    // get input
    u_type * var = input_and_output;
    
    // simulate some intensive computation
    std::this_thread::sleep_for(std::chrono::seconds(delay_time));

    // set output
    var->output = var->input / 2;
}

void execute_parallel(u_type * input_and_output_vec, int size_vec) {
    pthread_attr_t thread_attr[size_vec];
    pthread_t thread_ids[size_vec];
    
    // create and execute threads
    for (int i = 0; i < size_vec; ++i) {
        // set the default attributes of the thread
        pthread_attr_init(&thread_attr[i]);
        // create the thread
        pthread_create(&thread_ids[i], &thread_attr[i], runner, &input_and_output_vec[i]);
    }

    // wait for the threads to exit
    for (int i = 0; i < size_vec; ++i)
        pthread_join(thread_ids[i], NULL);
}

void execute_sequential(u_type * input_and_output_vec, int size_vec) {
    
    for (int i = 0; i < size_vec; ++i) {
        runner(&input_and_output_vec[i]);
    }
}

int main(int argc, const char * argv[]) {
    
    // declarations
    int vec_len = 3; // size data vector
    u_type * input_and_output_par_vec = new u_type[vec_len];
    u_type * input_and_output_seq_vec = new u_type[vec_len];
    
    // set input data
    for (int i = 0; i < vec_len; ++i) {
        input_and_output_par_vec[i].input = 2 * (i + 1);
        input_and_output_seq_vec[i].input = 2 * (i + 1);
    }
    
    // start timing parallelized computation
    auto start_par = std::chrono::steady_clock::now();
    
    // perform computation in parallel
    execute_parallel(input_and_output_par_vec, vec_len);
    
    // end timing parallelized computation
    auto end_par = std::chrono::steady_clock::now();
    
    // start timing sequential computation
    auto start_seq = std::chrono::steady_clock::now();
    
    // perform computation sequentially
    execute_sequential(input_and_output_seq_vec, vec_len);
    
    // end timing sequential computation
    auto end_seq = std::chrono::steady_clock::now();
    
    // store the time difference between start and end in (s)
    auto time_par = std::chrono::duration_cast<std::chrono::seconds>(end_par - start_par);
    auto time_seq = std::chrono::duration_cast<std::chrono::seconds>(end_seq - start_seq);
    
    // print output data
    for (int i = 0; i < vec_len; ++i) {
        std::cout << i << ": " << input_and_output_par_vec[i].output << ", ";
        std::cout << i << ": " << input_and_output_seq_vec[i].output << std::endl;
    }
    
    // print timing
    std::cout << "time: " << time_par.count() << " (s)" << std::endl;
    std::cout << "time: " << time_seq.count() << " (s)" << std::endl;
    
    return 0;
}
