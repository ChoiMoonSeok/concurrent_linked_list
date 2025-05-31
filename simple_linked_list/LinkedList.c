#include "LinkedList.h"

head *init_linked_list()
{

    head *h = malloc(sizeof(head));
    h->stt = NULL;

    return h;
};

int add_node(head *h, int key, int val)
{

    node *new = malloc(sizeof(node));

    new->next = NULL;
    new->prev = NULL;
    new->val = val;
    new->key = key;

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

    return 0;
}

node *search_node(head *h, int key)
{

    node *cur = h->stt;

    while (cur->key != key)
    {
        if (cur->next != NULL)
            cur = cur->next;
        else
            return NULL;
    }

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
