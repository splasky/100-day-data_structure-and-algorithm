#include "../include/my_malloc.h"
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

struct header_t
{
    size_t size;
    bool is_free;
    struct header_t *next;
}*head, *tail;

static pthread_mutex_t global_malloc_lock;

static struct header_t* get_free_block(size_t size)
{
    struct header_t *curr = head;
    while (curr)
    {
        if (curr->is_free == true && curr->size >= size)
        {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

void *my_malloc(size_t size)
{
    if (!size)
    {
        return NULL;
    }
    pthread_mutex_lock(&global_malloc_lock);
    struct header_t* header = get_free_block(size);
    if (header)
    {
        header->is_free = false;
        pthread_mutex_unlock(&global_malloc_lock);
        return (void*)(header + 1);
    }

    size_t total_size = sizeof(struct header_t) + size;
    void* block = sbrk(total_size);
    /* This is tricky see man page of sbrk */
    if (block == (void*) -1)
    {
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }

    header = block;
    header->is_free = false;
    header->size = total_size;
    header->next = NULL;

    /* if head is not exists */
    if (!head)
    {
        head = header;
    }

    /* if tail is exists */
    if (tail)
    {
        tail->next = header;
    }
    tail = header;
    pthread_mutex_unlock(&global_malloc_lock);
    /* we need to hide the existence of header */
    /* point to the actual memory block */
    return (void*)(header + 1);
}

void my_free(void* block)
{
    if (!block)return;
    struct header_t* header = (struct header_t*)block - 1;
    void* programebreak = sbrk(0);
    pthread_mutex_lock(&global_malloc_lock);
    if ((char*)block + header->size == programebreak)
    {
        if (head == tail)
        {
            head = tail = NULL;
        }
        else
        {
            struct header_t* curr = head;
            while (curr)
            {
                if (curr->next == tail)
                {
                    curr->next = NULL;
                    tail = curr;
                }
                curr = curr->next;
            }
        }
        sbrk(0 - sizeof(struct header_t) - header->size);
        pthread_mutex_unlock(&global_malloc_lock);
        return;
    }
    header->is_free = true;
    pthread_mutex_unlock(&global_malloc_lock);
}

void *my_calloc(size_t num, size_t nsize)
{
    if (!num || ! nsize)
    {
        return NULL;
    }

    /*check mul over flow*/
    const size_t total_size = num * nsize;
    if (num != total_size / nsize)
    {
        return NULL;
    }

    void* block = malloc(total_size);
    if (!block)
    {
        return NULL;
    }

    memset(block, 0, total_size);
    return block;
}

void *my_realloc(void* block, size_t size)
{
    if (block == NULL)
        return malloc(size);
    if (size == 0 && block != NULL)
    {
        free(block);
        return NULL;
    }

    struct header_t* header = (struct header_t*)block - 1;
    if (header->size >= size)
    {
        return block;
    }

    void* ret = malloc(size);
    if (ret)
    {
        memcpy(ret, block, header->size);
        free(block);
    }
    return ret;
}
