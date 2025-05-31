#include <stdio.h>
#include <time.h>
#include "LinkedList.h"

#define NUM_OF_THREAD 50

typedef struct ll_op_args
{
    head *h;
    int key_stt;
    int key_end;
    int ret[NUM_OF_NODES / NUM_OF_THREAD];
} ll_op_args;

void *wrapper_search(void *arg)
{

    ll_op_args *arg_convert = (ll_op_args *)arg;

    for (int i = arg_convert->key_stt; i < arg_convert->key_end; i++)
        arg_convert->ret[i - arg_convert->key_stt] = calc_time_diff_search(arg_convert->h, i);

    return NULL;
}

void *wrapper_add(void *arg)
{

    ll_op_args *arg_convert = (ll_op_args *)arg;

    for (int i = arg_convert->key_stt; i < arg_convert->key_end; i++)
        arg_convert->ret[i - arg_convert->key_stt] = calc_time_diff_add(arg_convert->h, i);

    return NULL;
}

int main()
{
    // values for measuring time
    long long int total_add_time = 0;
    long long int total_search_time = 0;

    // values for creating threads
    ll_op_args args_add[NUM_OF_THREAD];
    ll_op_args args_search[NUM_OF_THREAD];
    pthread_t threads[NUM_OF_THREAD];

    head *ll = init_linked_list();

    for (int i = 0; i < NUM_OF_THREAD; i++)
    {

        args_add[i].h = ll;
        args_add[i].key_stt = i * (NUM_OF_NODES / NUM_OF_THREAD);
        args_add[i].key_end = (i + 1) * (NUM_OF_NODES / NUM_OF_THREAD);

        pthread_create(&threads[i], NULL, wrapper_add, (void *)&args_add[i]);
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < NUM_OF_THREAD; i++)
    {

        args_search[i].h = ll;
        args_search[i].key_stt = i * (NUM_OF_NODES / NUM_OF_THREAD);
        args_search[i].key_end = (i + 1) * (NUM_OF_NODES / NUM_OF_THREAD);

        pthread_create(&threads[i], NULL, wrapper_search, (void *)&args_search[i]);
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < NUM_OF_THREAD; i++)
    {
        for (int j = 0; j < NUM_OF_NODES / NUM_OF_THREAD; j++)
        {
            total_add_time += (long long int)args_add[i].ret[j];
            total_search_time += (long long int)args_search[i].ret[j];
        }
    }

    printf("avg add time : %lld\n", total_add_time / NUM_OF_NODES);
    printf("avg search time : %lld\n", total_search_time / NUM_OF_NODES);

    return 0;
}