#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_OF_NODES 100000
#define MAX_NANO_SECOND 1000000000

typedef struct node_t
{
    int key;
    int val;
    struct node_t *next;
    struct node_t *prev;
} node;

typedef struct head_t
{
    struct node_t *stt;
    pthread_spinlock_t spinlock;
} head;

head *init_linked_list();
int add_node(head *h, int key, int val);
node *search_node(head *h, int key);
int remove_node(head *h, int key);
int calc_time_diff_search(head *h, int key);
int calc_time_diff_add(head *h, int key);
