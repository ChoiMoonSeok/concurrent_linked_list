#include "LinkedList.h"

head *init_linked_list()
{

    head *h = malloc(sizeof(head));
    h->stt = NULL;
    pthread_spin_init(&h->spinlock, PTHREAD_PROCESS_PRIVATE);

    return h;
};

int add_node(head *h, int key, int val)
{

    node *new = malloc(sizeof(node));
    pthread_spin_init(&new->spinlock_node, PTHREAD_PROCESS_PRIVATE);
    pthread_spin_lock(&new->spinlock_node);

    new->next = NULL;
    new->prev = NULL;
    new->val = val;
    new->key = key;

    // critical section
    pthread_spin_lock(&h->spinlock);
    node *cur = h->stt;
    if (cur != NULL)
    {
        pthread_spin_lock(&cur->spinlock_node);
        pthread_spin_unlock(&h->spinlock);
        while (cur->next != NULL)
        {
            pthread_spin_lock(&cur->next->spinlock_node);
            pthread_spin_unlock(&cur->spinlock_node);
            cur = cur->next;
        }

        cur->next = new;
        new->prev = cur;
        pthread_spin_unlock(&cur->spinlock_node);
    }
    else
    {
        h->stt = new;
        pthread_spin_unlock(&h->spinlock);
    }

    pthread_spin_unlock(&new->spinlock_node);

    return 0;
}

int search_node(head *h, int key)
{

    int ret;

    // critical section
    pthread_spin_lock(&h->spinlock);
    node *cur = h->stt;
    pthread_spin_lock(&cur->spinlock_node);
    pthread_spin_unlock(&h->spinlock);

    while (cur->key != key)
    {
        if (cur->next != NULL)
        {
            pthread_spin_lock(&cur->next->spinlock_node);
            pthread_spin_unlock(&cur->spinlock_node);
            cur = cur->next;
        }
        else
        {
            pthread_spin_unlock(&cur->spinlock_node);
            return -1;
        }
    }

    ret = cur->val;
    pthread_spin_unlock(&cur->spinlock_node);

    return ret;
}

int remove_node(head *h, int key)
{

    node *cur = h->stt;
    if (cur != NULL)
    {
        while (cur->key != key)
        {
            if (cur->next != NULL)
                cur = cur->next;
            else
                return 1;
        }
    }
    else
        return 1;

    return 0;
}

/*
    measure elapsed time
*/

int calc_time_diff_search(head *h, int key)
{

    struct timespec stt, end;

    clock_gettime(CLOCK_MONOTONIC, &stt);
    int val = search_node(h, key);
    clock_gettime(CLOCK_MONOTONIC, &end);
    if (val != NUM_OF_NODES - key)
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