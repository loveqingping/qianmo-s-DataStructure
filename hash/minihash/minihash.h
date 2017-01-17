//
//  minihash.h
//  MiniHash
//
//  Created by 浩 罗 on 11-10-10.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef MiniHash_minihash_h
#define MiniHash_minihash_h

#include <stdint.h>



#define MINI_HASH_NULL_PTR       -1
#define MINI_HASH_INVALID_BN     -2
#define MINI_HASH_MALLOC_FAIL    -3
#define MINI_HASH_EMPTY          -4
#define MINI_HASH_NOT_FOUND      -5
#define MINI_HASH_NO_NODES       -6
#define MINI_HASH_EXPIRE_FAIL    -7
#define MINI_HASH_KEY_TOO_LONG   -8
#define MINI_HASH_VALUE_TOO_LONG -9


typedef int (*hash_calc_func)(const uint8_t *key, int key_len);

typedef enum _mini_hash_behavior_tag_ {
    fifo_expire = 1,
    manual_expire,
    no_expire
}mini_hash_behavior_t;

typedef struct _mini_hash_user_node_t_ {
    uint16_t key_len;
    uint16_t value_len;
    uint8_t  *key;
    uint8_t  *value;
}mini_hash_user_node_t;

/* 
 About node link:
 Add some comment here someday.
 */
typedef struct __mini_hash_node_tag_ {
    struct __mini_hash_node_tag_ *bucket_next, *bucket_pre, *link_pre, *link_next;
	uint32_t bucket_index;
    uint16_t key_len;
    uint16_t value_len;
    uint8_t content[0];
}mini_hash_node_t;

typedef struct __mini_hash_bucket_tag_ {
    mini_hash_node_t *nodes;
    uint32_t node_num;
}mini_hash_bucket_t;


typedef struct __mini_hash_tag__ {
    int bucket_num;
    int node_num;
    uint16_t key_size;
    uint16_t value_size;
    mini_hash_behavior_t behavior; 
    uint8_t *node_ptr;
    mini_hash_node_t **node_array;
    
    int used_nodes_count, free_nodes_count;
    
    mini_hash_node_t *free_nodes, *link_nodes;
    mini_hash_bucket_t * buckets;
    mini_hash_node_t *link_head, *link_tail;

    hash_calc_func hash_func;
}mini_hash_t;

int mini_hash_create(mini_hash_t *minihash, int buckets, int nodes, 
                     uint32_t key_size, uint32_t value_size, mini_hash_behavior_t behavior);


uint32_t mini_hash_calc(mini_hash_t *minihash, const uint8_t *key, int key_len);

void* mini_hash_find_value(mini_hash_t *minihash, void *key, int key_len);
mini_hash_node_t * mini_hash_find_node(mini_hash_t *minihash, void *key, int key_len);
mini_hash_user_node_t * mini_hash_find_user_node(mini_hash_t *minihash, void *key, int key_len, 
                                                               mini_hash_user_node_t *usernode);
void *mini_hash_get_node_key(mini_hash_t *minihash, mini_hash_node_t *node);
void *mini_hash_get_node_value(mini_hash_t *minihash, mini_hash_node_t *node);

int  mini_hash_remove(mini_hash_t *minihash, void *key, int key_len);
int  mini_hash_remove_node(mini_hash_t *minihash, mini_hash_node_t *node);
void mini_hash_remove_node_by_percent(mini_hash_t *minihash, float rate);

int mini_hash_add(mini_hash_t *minihash, void *key, int key_len, void *value, int value_len);
int mini_hash_add_ex(mini_hash_t *minihash, void *key, int key_len, void *value, int value_len,
                              mini_hash_node_t **ret_node);

mini_hash_node_t *mini_hash_get_oldest_node(mini_hash_t *minihash);

int mini_hash_expire(mini_hash_t *minihash, int count);


void mini_hash_lru_update(mini_hash_t *minihash, void *key, int key_len);
void mini_hash_lru_update_node(mini_hash_t *minihash, mini_hash_node_t *node);




#endif

