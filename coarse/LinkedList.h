#include <stdlib.h>
#include <pthread.h>

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
