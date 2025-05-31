#include <stdio.h>
#include <time.h>
#include "LinkedList.h"

#define NUM_OF_THREAD 1
#define NUM_OF_NODES 100000
#define MAX_NANO_SECOND 1000000000

typedef struct ll_op_args
{
    head *h;
    int key;
} ll_op_args;

int calc_time_diff_search(head *h, int key)
{

    struct timespec stt, end;

    clock_gettime(CLOCK_MONOTONIC, &stt);
    node *cur = search_node(h, key);
    clock_gettime(CLOCK_MONOTONIC, &end);
    if (cur->val != NUM_OF_NODES - key)
        printf("Error at key : %d", key);

    if (stt.tv_sec == end.tv_sec)
        return (int)(end.tv_nsec - stt.tv_nsec);
    else
        return (int)((MAX_NANO_SECOND - stt.tv_nsec) + end.tv_nsec);
}

int calc_time_diff_add(head *h, int key)
{
    struct timespec stt, end;

    clock_gettime(CLOCK_MONOTONIC, &stt);
    add_node(h, key, NUM_OF_NODES - key);
    clock_gettime(CLOCK_MONOTONIC, &end);

    if (stt.tv_sec == end.tv_sec)
        return (int)(end.tv_nsec - stt.tv_nsec);
    else
        return (int)((MAX_NANO_SECOND - stt.tv_nsec) + end.tv_nsec);
}

void *wrapper_search(void *arg)
{

    int *ret = (int *)malloc(sizeof(int));

    ll_op_args *arg_convert = (ll_op_args *)arg;

    *ret = calc_time_diff_search(arg_convert->h, arg_convert->key);

    return (void *)ret;
}

void *wrapper_add(void *arg)
{
    printf("start wrapper add\n");
    ll_op_args *arg_convert = (ll_op_args *)arg;

    calc_time_diff_add(arg_convert->h, arg_convert->key);
}

int main()
{

    int write_time[NUM_OF_NODES] = {0};
    int read_time[NUM_OF_NODES] = {0};

    pthread_t threads[NUM_OF_THREAD];

    head *ll = init_linked_list();

    for (int i = 0; i < (NUM_OF_NODES / NUM_OF_THREAD); i++)
    {
        ll_op_args args[NUM_OF_THREAD] = {0};
        int t_id[NUM_OF_THREAD];
        for (int j = 0; j < NUM_OF_THREAD; j++)
        {
            args[j].key = j + i;
            args[j].h = ll;
            t_id[j] = j;
            printf("%d\n", j);
            pthread_create(&threads[t_id[j]], NULL, wrapper_add, (void *)&args[t_id[j]]);
            printf("%d\n", j);
        }

        for (int j = 0; j < NUM_OF_THREAD; j++)
            pthread_join(&threads[j], NULL);
    }

    for (int i = 0; i < NUM_OF_NODES; i++)
    {
        node *cur = search_node(ll, i);

        if (cur->val != NUM_OF_NODES - i)
            printf("Error at key : %d", i);
    }

    // printf("avg write time : %f ns\n", (double)sum_write_time / NUM_OF_NODES);
    // printf("avg read time : %f ns\n", (double)sum_read_time / NUM_OF_NODES);

    return 0;
}