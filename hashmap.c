
#include <stdlib.h>
#include "hashmap.h"

struct hashmap *init_hashmap()
{
    int buffer_size = make_prime(DEF_CAP);
    struct hashmap (*hashmap) = (struct hashmap(*)) malloc(sizeof(struct hashmap) + (buffer_size * sizeof(struct bucket)));
    hashmap->capacity = buffer_size;
    hashmap->add_item = add_item;
    hashmap->remove_item = remove_item;
    hashmap->get_item = get_item;
    // initialize buckets
    for (int i = 0; i < hashmap->capacity; i++)
    {
        hashmap->buckets[i] = (struct bucket(*))malloc(sizeof(struct bucket) + (buffer_size * sizeof(struct node)));
        hashmap->buckets[i]->capacity = hashmap->capacity;
        hashmap->buckets[i]->size = 0;
        for (int j = 0; j < hashmap->buckets[i]->capacity; j++) // initialize nodes to zeroed
        {
            hashmap->buckets[i]->nodes[j] = (struct node(*))malloc(sizeof(struct node));
            hashmap->buckets[i]->nodes[j]->is_added= 0;
        }
    }
    return hashmap;
}

static int make_prime(int number)
{
    if (number <= 2) // prime numbers must be positive
        return 2;    // return min prime number

    int count = 2; // starting from index 2
    while (1)      // endless loop to reach number
    {
        if (number % count == 0) // not prime, increase & call it again!
            return make_prime(++number);
        count++;
        if (count == number)
            return number;
    }
    return number;
}

static int get_hash(struct hashmap(*const hashmap_ptr), int key)
{
    return key % hashmap_ptr->capacity;
}

char add_item(struct hashmap(*hashmap_ptr), int key, int value)
{
    int index = get_hash(hashmap_ptr, key);

    struct bucket(*bucket) = hashmap_ptr->buckets[index];

    if (bucket->size < bucket->capacity)
    {
        index = 0;
        while (index < bucket->capacity)
        {
            struct node(*node) = bucket->nodes[index];
            if (node->is_added == 0) // check its empty
            {
                node->key = key;
                node->value = value;
                node->is_added=1;
                bucket->size++;
                return 1;
            }
            if (node->key == key) // update value, associate with key
            {
                node->value = value;
                return 1;
            }
            index++;
        }
    } // capacity excess, extend array and return here!
    else
    {
        extend_array(hashmap_ptr, index);
       return add_item(hashmap_ptr, key, value);
    }
    return 0;
}

char remove_item(struct hashmap(*hashmap_ptr), int key)
{
    int index = get_hash(hashmap_ptr, key);
    struct bucket(*bucket_ptr) = hashmap_ptr->buckets[index];
    index = 0;
    while (index < bucket_ptr->capacity)
    {
        if (bucket_ptr->nodes[index]->key == key) // update value, associate with key
        {
            bucket_ptr->nodes[index]->is_added = 0;
            bucket_ptr->size--;
            // no need to freed, we'll use this node again.
            return 1;
        }
        index++;
    }
    return 0;
}

int get_item(struct hashmap(*const hashmap_ptr), int key)
{
    int index = get_hash(hashmap_ptr, key);
    struct bucket(*bucket_ptr) = hashmap_ptr->buckets[index];
    index = 0;
    while (index < bucket_ptr->capacity)
    {
        if (bucket_ptr->nodes[index]->key == key)
            return bucket_ptr->nodes[index]->value;
        index++;
    }
    return 0;
}

static void extend_array(struct hashmap(*hashmap_ptr), int index)
{
    struct bucket(*bucket_ptr) = hashmap_ptr->buckets[index];

    int old_cap = bucket_ptr->capacity;
    bucket_ptr->capacity = make_prime(old_cap * 2);
    index = 0;
    while (index < bucket_ptr->capacity)
    {
        struct node(*new_node) = (struct node(*))malloc(sizeof(struct node));
        new_node->is_added = 0;
        if (index < old_cap)
        {
            struct node (*old_node) = bucket_ptr->nodes[index];
            new_node->key = old_node->key;
            new_node->value = old_node->value;
            new_node->is_added = old_node->is_added;
            free(old_node);
        }
        bucket_ptr->nodes[index] = new_node;
        index++;
    }
}
