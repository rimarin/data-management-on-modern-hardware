#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    /*
     * Write a C program that measures your cache performance and memory hierarchy.
       We want to derive the following parameters of our CPU experimentally:
       • The number of caches
       • The sizes of all caches
       • The access latencies of all caches and main memory
       • The cache line size
     */

    // parameter stride s: distance in byte we jump between memory locations.
    // by increasing it, the memory access gets more random
    // vary the stride size s (e.g., from 8 bytes to 16 kilobytes)
    int stride = 8; // 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8196, 16384
    // vary the size of the array, from 4 kB to 512 MB
    int array_sizes[] = {1024,  2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144};
    int num_array_sizes = sizeof(array_sizes)/sizeof(array_sizes[0]);
    // Perform one million iterations
    int iterations = 1000000;

    // Test different array sizes
    float avg_access_time_per_array_size[num_array_sizes];
    for (int array_size_idx = 0; array_size_idx < num_array_sizes; array_size_idx++) {
        int size = array_sizes[array_size_idx];
        // Initialize the array
        int *data = (int *) malloc(size * sizeof(int));
        data[0] = (int) &data[0];
        for (int k = 1; k < size; k++) {
            data[k] = (int) &data[k+stride];
        }
        // Perform one million to one billion iterations over the array to get precise results
        float total_iterations_access_time = 0;
        for (int i = 0; i < iterations; i++) {
            // Perform random access by jumping into different memory positions. The stride defines the length of jump
            // In this way the access is "controlled" but still random from the hardware point of view
            clock_t start = clock();
            for (int k = 0; k < size; k++) {
                int *value = (int*) data[k];
            }
            clock_t end = clock();
            float total_access_time = (float)(end - start) / CLOCKS_PER_SEC;
            float iteration_avg_access_time = total_access_time / (float) size;
            total_iterations_access_time += iteration_avg_access_time;
        }
        avg_access_time_per_array_size[array_size_idx] = total_iterations_access_time / (float) iterations;
        // Deallocate the memory with the array
        free(data);
    }
    // Print the results
    for (int i = 0; i < num_array_sizes; i++) {
        printf("%6.6e ", avg_access_time_per_array_size[i]);
    }
    printf("\n");
    return 0;
}
