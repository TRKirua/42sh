#ifndef MMALLOC_H
#define MMALLOC_H

#include "main.h"

void *my_malloc(size_t size);
void *my_calloc(size_t nmemb, size_t size);
void *my_realloc(void *ptr, size_t size);
void my_free(void);

#endif // ! MMALLOC_H
