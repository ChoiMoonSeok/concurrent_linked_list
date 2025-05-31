#include <stdio.h>
#include <time.h>
#include "LinkedList.h"

#define NUM_OF_NODES 100000
#define MAX_NANO_SECOND 1000000000

long long int calc_time_diff(time_t stt, long stt_nsec, time_t end, long end_nsec)
{
    if (stt == end)
        return (long long int)(end_nsec - stt_nsec);
    else
        return (long long int)((MAX_NANO_SECOND - stt_nsec) + end_nsec);
}

int main()
{

    struct timespec stt, end;

    head *ll = init_linked_list();

    // int *write_time = malloc(sizeof(int) * NUM_OF_NODES);
    // int *search_time = malloc(sizeof(int) * NUM_OF_NODES);

    long long sum_write_time = 0;
    long long sum_read_time = 0;

    for (int i = 0; i < NUM_OF_NODES; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &stt);
        add_node(ll, i, NUM_OF_NODES - i);
        clock_gettime(CLOCK_MONOTONIC, &end);

        sum_write_time += calc_time_diff(stt.tv_sec, stt.tv_nsec, end.tv_sec, end.tv_nsec);
    }

    for (int i = 0; i < NUM_OF_NODES; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &stt);
        node *cur = search_node(ll, i);
        clock_gettime(CLOCK_MONOTONIC, &end);
        sum_read_time += calc_time_diff(stt.tv_sec, stt.tv_nsec, end.tv_sec, end.tv_nsec);

        if (cur->val != NUM_OF_NODES - i)
            printf("Error at key : %d", i);
    }

    printf("avg write time : %f ns\n", (double)sum_write_time / NUM_OF_NODES);
    printf("avg read time : %f ns\n", (double)sum_read_time / NUM_OF_NODES);

    return 0;
}