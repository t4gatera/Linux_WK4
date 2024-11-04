#include <stdio.h>

#define LIMIT 1000000

int main() {
    long total_sum = 0;

    // Calculate the sum from 1 to LIMIT in a single thread
    for (int i = 1; i <= LIMIT; i++) {
        total_sum += i;
    }

    printf("The sum of the first %d numbers is: %ld\n", LIMIT, total_sum);

    return 0;
}

