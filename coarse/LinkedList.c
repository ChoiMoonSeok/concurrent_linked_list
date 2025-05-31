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

    new->next = NULL;
    new->prev = NULL;
    new->val = val;
    new->key = key;

    // critical section
    pthread_spin_lock(&h->spinlock);
    node *cur = h->stt;
    if (cur != NULL)
    {
        while (cur->next != NULL)
            cur = cur->next;

        cur->next = new;
        new->prev = cur;
    }
    else
    {
        h->stt = new;
    }
    pthread_spin_unlock(&h->spinlock);

    return 0;
}

node *search_node(head *h, int key)
{

    // critical section
    pthread_spin_lock(&h->spinlock);
    node *cur = h->stt;

    while (cur->key != key)
    {
        if (cur->next != NULL)
            cur = cur->next;
        else
            return NULL;
    }
    pthread_spin_unlock(&h->spinlock);

    return cur;
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
