#include "mmalloc.h"

struct MemNode
{
    void *ptr;
    struct MemNode *next;
};

struct MemNode *mm = NULL;

void *my_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
        return NULL;

    struct MemNode *new_node = (struct MemNode *)malloc(sizeof(struct MemNode));
    new_node->ptr = ptr;
    new_node->next = mm;
    mm = new_node;

    return ptr;
}

void *my_calloc(size_t nmemb, size_t size)
{
    void *ptr = calloc(nmemb, size);
    if (ptr == NULL)
        return NULL;

    struct MemNode *new_node = (struct MemNode *)malloc(sizeof(struct MemNode));
    new_node->ptr = ptr;
    new_node->next = mm;
    mm = new_node;

    return ptr;
}

void *my_realloc(void *ptr, size_t size)
{
    struct MemNode *curr = mm;
    while (curr != NULL)
    {
        if (curr->ptr == ptr)
            break;

        curr = curr->next;
    }

    if (curr == NULL)
        return NULL;

    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL)
        return NULL;

    curr->ptr = new_ptr;

    return new_ptr;
}

void my_free(void)
{
    struct MemNode *curr = mm;
    while (curr != NULL)
    {
        free(curr->ptr);
        struct MemNode *temp = curr;
        curr = curr->next;
        free(temp);
    }

    mm = NULL;
}
