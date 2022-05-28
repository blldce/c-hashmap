
#if !defined(HASH_H)
#define HASH_H 1

#define DEF_CAP 3

struct node
{
    int key;
    int value;
    char is_added : 2;
};

struct bucket
{
    int capacity;
    int size;
    struct node(*nodes[]); // zero sized pointer array, not pointer to array! Must be end of struct
};

struct hashmap
{
    int capacity;
    char (*add_item)(struct hashmap(*), int, int);
    char (*remove_item)(struct hashmap(*), int);
    int (*get_item)(struct hashmap(*), int);
    struct bucket(*buckets[]); // zero sized pointer array, not pointer to array! Must be end of struct
};

// member funcs
extern char add_item(struct hashmap(*), int, int);
extern char remove_item(struct hashmap(*), int);
extern int get_item(struct hashmap(*const), int);

static int get_hash(struct hashmap(*const), int);

struct hashmap *init_hashmap();

static int make_prime(int);
static void extend_array(struct hashmap(*), int);

#endif // HASH_H
