#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define NUM_BITS 20
#define HASH_SIZE (1 << NUM_BITS)
#define HASH_MASK (HASH_SIZE - 1)
#define MAX_POOL_SIZE 500000


typedef struct password_node {
    uint64_t key;
    int total;
    struct password_node *next;
} password_node;

// Fast integer hash function (Knuth's multiplicative hash)
static uint32_t hash_int(uint64_t key) {
    key = (key ^ (key >> 30)) * 0xbf58476d1ce4e5b9ULL;
    key = (key ^ (key >> 27)) * 0x94d049bb133111ebULL;
    key = key ^ (key >> 31);
    return (uint32_t) key & HASH_MASK;
}

password_node *hash_table[HASH_SIZE] = {NULL};
password_node pool[MAX_POOL_SIZE];
int pool_ptr = 0;

// Find or create an entry in the hash table
password_node *get_node(uint64_t key, int create) {
    uint32_t slot = hash_int(key);
    password_node *curr = hash_table[slot];

    while (curr) {
        if (curr->key == key) return curr;
        curr = curr->next;
    }

    if (create) {
        if (pool_ptr >= MAX_POOL_SIZE) {
            fprintf(stderr, "Pool exhausted!\n");
            exit(1);
        }
        password_node *new_node = &pool[pool_ptr++];
        new_node->key = key;
        new_node->total = 0;
        new_node->next = hash_table[slot];
        hash_table[slot] = new_node;
        return new_node;
    }
    return NULL;
}

int main(void) {
    int num_ops;
    if (scanf("%d", &num_ops) != 1) return 0;

    char pw[11];
    uint64_t seen[55]; // Max unique substrings for length 10

    for (int op = 0; op < num_ops; op++) {
        int type;
        scanf("%d %s", &type, pw);
        const int len = strlen(pw);

        if (type == 1) {
            int seen_count = 0;
            for (int i = 0; i < len; i++) {
                uint64_t current_val = 0;
                for (int j = i; j < len; j++) {
                    // Build base-27 integer on the fly
                    current_val = (current_val * 27) + (pw[j] - 'a' + 1);

                    // Check if we've already processed this substring for THIS password
                    int already_seen = 0;
                    for (int k = 0; k < seen_count; k++) {
                        if (seen[k] == current_val) {
                            already_seen = 1;
                            break;
                        }
                    }

                    if (!already_seen) {
                        password_node *node = get_node(current_val, 1);
                        node->total++;
                        seen[seen_count++] = current_val;
                    }
                }
            }
        } else {
            uint64_t query_val = 0;
            for (int i = 0; i < len; i++) {
                query_val = (query_val * 27) + (pw[i] - 'a' + 1);
            }
            password_node *node = get_node(query_val, 0);
            printf("%d\n", node ? node->total : 0);
        }
    }
    return 0;
}
