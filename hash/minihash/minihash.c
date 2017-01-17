//
//  minihash.c
//  MiniHash
//
//  Created by 浩 罗 on 11-10-10.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#include "minihash.h"

int mini_hash_create(mini_hash_t *minihash, int buckets, int nodes, 
                         uint32_t key_size, uint32_t value_size, mini_hash_behavior_t behavior)
{
    int i, nodesize;
    
    if (minihash == NULL)
        return MINI_HASH_NULL_PTR;
    if (buckets < 3 || nodes < 3 || buckets > nodes)
        return MINI_HASH_INVALID_BN;
    
    minihash->bucket_num = buckets;
    minihash->node_num   = nodes;
    minihash->key_size   = key_size;
    minihash->value_size = value_size;
    minihash->behavior   = behavior;
    
    minihash->buckets = (mini_hash_bucket_t *)malloc(sizeof(mini_hash_bucket_t) * buckets );
    for (i = 0; i < buckets; i++)
    {
        minihash->buckets[i].nodes = NULL;
        minihash->buckets[i].node_num  = 0;
    }
    
    nodesize = sizeof(mini_hash_node_t) + key_size + value_size;
    if ( nodesize != ((nodesize / 4) * 4) )
    {
        nodesize = 4 * (nodesize/4) + 4;
    }
    
    minihash->node_ptr = (uint8_t *)malloc( nodesize * nodes );
    if (minihash->node_ptr == NULL)
        return MINI_HASH_MALLOC_FAIL;

    minihash->node_array = (mini_hash_node_t **)malloc(sizeof(mini_hash_node_t *) * nodes);
    minihash->free_nodes = NULL;
    minihash->link_nodes = NULL;
    for (i = 0; i < nodes; i++)
    {
        minihash->node_array[i] = (mini_hash_node_t *)(minihash->node_ptr + nodesize * i);
    }
    for (i = 0; i < nodes - 1; i++)
    {
        minihash->node_array[i]->link_next = minihash->node_array[i + 1];
        minihash->node_array[i]->link_pre = NULL;
    }
    minihash->node_array[nodes - 1]->link_next = NULL;
    minihash->free_nodes = minihash->node_array[0];
    
    
    minihash->used_nodes_count = 0;
    minihash->free_nodes_count = nodes;
    minihash->link_head = NULL;
    minihash->link_tail = NULL;
    minihash->hash_func = NULL;

    return 0;
}

int mini_hash_set_func(mini_hash_t *minihash, hash_calc_func func)
{
    if (minihash == NULL)
        return MINI_HASH_NULL_PTR;

    minihash->hash_func = func;

    return 0;
}

static mini_hash_node_t * mini_hash_find_internal(mini_hash_t *minihash, void *key, int key_len)
{
    uint32_t code;
    mini_hash_node_t *node;
    
    if (minihash == NULL)
        return NULL;
    
    if (minihash->used_nodes_count < 1)
        return NULL;
    
    code = mini_hash_calc((void *)minihash, key, key_len);
    node = minihash->buckets[code].nodes;
    
    if (node == NULL)
        return NULL;
    
    while (node)
    {
        if (key_len != node->key_len)
        {
            node = node->bucket_next;
            continue;
        }
        
        if (memcmp(key, node->content, key_len) == 0)
        {
            return node;
        }
        else
        {
            node = node->bucket_next;
            continue;
        }
    }
    
    return NULL;
}

void* mini_hash_find_value(mini_hash_t *minihash, void *key, int key_len)
{
    mini_hash_node_t *node;
    
    node = mini_hash_find_internal(minihash, key, key_len);
    
    if (NULL == node)
        return NULL;
    
    return (void *)(node->content + minihash->key_size);
}

void *mini_hash_get_node_value(mini_hash_t *minihash, mini_hash_node_t *node)
{
    return (void *)(node->content + minihash->key_size);
}

void *mini_hash_get_node_key(mini_hash_t *minihash, mini_hash_node_t *node)
{
    return (void *)(node->content);
}

mini_hash_node_t * mini_hash_find_node(mini_hash_t *minihash, void *key, int key_len)
{
    return mini_hash_find_internal(minihash, key, key_len);
}

mini_hash_user_node_t * mini_hash_find_user_node(mini_hash_t *minihash, void *key, int key_len, 
                                                 mini_hash_user_node_t *usernode)
{
    mini_hash_node_t *node;
    
    node = mini_hash_find_internal(minihash, key, key_len);
    if (NULL == node)
        return NULL;

    usernode->key_len = node->key_len;
    usernode->value_len = node->value_len;
    usernode->key = node->content;
    usernode->value = node->content + minihash->key_size;
    
    return usernode;
}


/*
* Fowler/Noll/Vo hash
*
* The basis of the hash algorithm was taken from an idea sent by email to the
* IEEE Posix P1003.2 mailing list from Phong Vo (kpv@research.att.com) and
* Glenn Fowler (gsf@research.att.com). Landon Curt Noll (chongo@toad.com)
* later improved on their algorithm.
*
* The magic is in the interesting relationship between the special prime
* 16777619 (2^24 + 403) and 2^32 and 2^8.
*
* This hash produces the fewest collisions of any that we've seen so
* far, and works well on both numbers and strings.
*
* please refer: http://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
*
*/

#if 0

 NOTE:  FACEBOOK FOLLY HASH IMPL.            LUO.HAO

/* * Fowler / Noll / Vo (FNV) Hash *     http://www.isthe.com/chongo/tech/comp/fnv/ */
const uint32_t FNV_32_HASH_START = 216613626UL;
const uint64_t FNV_64_HASH_START = 14695981039346656037ULL;
inline uint32_t fnv32(const char* s, uint32_t hash = FNV_32_HASH_START) 
{  
    for (; *s; ++s) {    
        hash += (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 24); 
        hash ^= *s;  
    }  
    return hash;
}

inline uint64_t fnv64(const char* s, uint64_t hash = FNV_64_HASH_START) 
{  
    for (; *s; ++s) 
    {    
        hash += (hash << 1) + (hash << 4) + (hash << 5) + (hash << 7) + (hash << 8) + (hash << 40); 
        hash ^= *s;  
    }  
    return hash;
}
#endif

const uint32_t FNV_32_HASH_START = 216613626UL;
uint32_t mini_hash_calc(mini_hash_t *minihash, const uint8_t *key, int key_len)
{
    const uint8_t *end = key + key_len;
    uint32_t hash;

    if (minihash->hash_func)
    {
        hash = minihash->hash_func(key, key_len);
        return hash % minihash->bucket_num;
    }
    for (hash = FNV_32_HASH_START; key < end; key++)
    {
        hash += (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 24);
        hash ^= (uint32_t) *(uint8_t *)key;
    }
 
    return hash % minihash->bucket_num;
}

#if 0
   OLD STYLE FNV IMPL. OBSOLETE BY LUO.HAO 2012.10.31
uint32_t mini_hash_calc(mini_hash_t *minihash, const uint8_t *key, int key_len)
{
    const uint8_t *end = key + key_len;
    uint32_t hash;

    if (minihash->hash_func)
    {
        hash = minihash->hash_func(key, key_len);
        return hash % minihash->bucket_num;
    }

    for (hash = 0; key < end; key++)
    {
        hash *= 16777619;
        hash ^= (uint32_t) *(uint8_t *)key;
    }
 
    return hash % minihash->bucket_num;
}
#endif

int mini_hash_remove(mini_hash_t *minihash, void *key, int key_len)
{
    uint32_t code;
    mini_hash_node_t *node;
    
    if (minihash == NULL)
        return MINI_HASH_NULL_PTR;
    
    if (minihash->used_nodes_count < 1)
        return MINI_HASH_EMPTY;
    
    code = mini_hash_calc((void *)minihash, key, key_len);
    node = minihash->buckets[code].nodes;
    
    if (node == NULL)
        return MINI_HASH_NOT_FOUND;
    
    while (node)
    {
        if (key_len != node->key_len)
        {
            node = node->bucket_next;
            continue;
        }
        
        if (memcmp(key, node->content, key_len) == 0)
        {
            break;
        }
        else
        {
            node = node->bucket_next;
            continue;
        }
    }
    
    if (NULL == node)
        return MINI_HASH_NOT_FOUND;
   
    return mini_hash_remove_node(minihash, node);
}

int mini_hash_remove_node(mini_hash_t *minihash, mini_hash_node_t *node)
{
    uint32_t code;

    if (minihash == NULL)
        return MINI_HASH_NULL_PTR;
    
    if (minihash->used_nodes_count < 1)
        return MINI_HASH_EMPTY;
    
    if (node == NULL)
        return MINI_HASH_NOT_FOUND;

    code = node->bucket_index;
    
    if (minihash->buckets[code].nodes == node)
    {
        minihash->buckets[code].nodes = node->bucket_next;
    }

    if (node->bucket_pre != NULL)
        node->bucket_pre->bucket_next = node->bucket_next;
    if (node->bucket_next != NULL)
        node->bucket_next->bucket_pre = node->bucket_pre;
    
    node->bucket_pre = NULL;
	node->bucket_next = NULL;
    
    
    // remove from manage link.
    if (node == minihash->link_head)
    {
        minihash->link_head = node->link_next;
		if(minihash->link_head)
			  minihash->link_head->link_pre=NULL;
    }
    if (node == minihash->link_tail)
    {
        minihash->link_tail = node->link_pre;

		if(minihash->link_tail)
			  minihash->link_tail->link_next=NULL;
			
    }
    if (node->link_next != NULL)
    {
        node->link_next->link_pre = node->link_pre;
    }
    if (node->link_pre != NULL)
    {
        node->link_pre->link_next = node->link_next;
    }
    
    // put back to free nodes list.
    node->link_pre=NULL;
    node->link_next = minihash->free_nodes;
    minihash->free_nodes = node;

    
    // update hash entry counter.

	minihash->buckets[code].node_num--;
    minihash->used_nodes_count --;
    minihash->free_nodes_count ++;
    
    return 0;
}

void mini_hash_remove_node_by_percent(mini_hash_t *minihash, float rate)
{

	uint32_t count=minihash->used_nodes_count;
	mini_hash_node_t *node=NULL;
	int i=0;
	for(i=0;i<count*rate;i++)
	{
		 node=mini_hash_get_oldest_node(minihash);
	 	 mini_hash_remove_node(minihash,node);
	}



}



mini_hash_node_t *mini_hash_get_oldest_node(mini_hash_t *minihash)
{
    return minihash->link_tail;
}

int mini_hash_add_ex(mini_hash_t *minihash, void *key, int key_len, 
            void *value, int value_len, mini_hash_node_t **ret_node)
{
    mini_hash_node_t *node,*node_oldest;
    uint32_t code;
    
    if (minihash == NULL)
        return MINI_HASH_NULL_PTR;
   
    if (key_len > minihash->key_size)
        return MINI_HASH_KEY_TOO_LONG;
    if (value_len > minihash->value_size)
        return MINI_HASH_VALUE_TOO_LONG;

    if (minihash->free_nodes_count < 1)
    {
        if (minihash->behavior != fifo_expire)
        {
            return MINI_HASH_NO_NODES;
        }
        else
        {
            // we do expire here
            node_oldest = mini_hash_get_oldest_node(minihash);
            if (mini_hash_remove_node(minihash,node_oldest) != 0)
                return MINI_HASH_EXPIRE_FAIL;
        }
    }
    
    code = mini_hash_calc(minihash, key, key_len);
    
    if ( minihash->free_nodes == NULL )
        return MINI_HASH_EXPIRE_FAIL;
    
	
    node = minihash->free_nodes;
    minihash->free_nodes = node->link_next;

    node->bucket_index=code;
    node->bucket_pre = NULL;
    node->bucket_next = minihash->buckets[code].nodes;
    if (minihash->buckets[code].nodes != NULL)
    {
        minihash->buckets[code].nodes->bucket_pre = node;
    }
    minihash->buckets[code].nodes = node;

    memcpy(node->content, key, key_len);
    node->key_len = key_len;

    memcpy(node->content + minihash->key_size, value, value_len);
    node->value_len = value_len;

	minihash->buckets[code].node_num++;
    minihash->used_nodes_count ++;
    minihash->free_nodes_count --;
    
    // someday fix here!
    if (minihash->link_head != NULL)
    {
        minihash->link_head->link_pre = node;
		node->link_next = minihash->link_head;
		node->link_pre  = NULL;
    }
	else
	{
		 node->link_next=node->link_pre=NULL;
	}

	
    minihash->link_head = node;
    
    if (minihash->link_tail == NULL)
    {
        minihash->link_tail = node;
		node->link_next=node->link_pre=NULL;
    }

    if (ret_node != NULL)
        *ret_node = node;
   
    return 0;
}

int mini_hash_add(mini_hash_t *minihash, void *key, int key_len, void *value, int value_len)
{
    return mini_hash_add_ex(minihash, key, key_len, value, value_len, NULL);
}

int mini_hash_expire(mini_hash_t *minihash, int count)
{
    mini_hash_node_t *node;
    int ret, total;

    total = 0;
    while (total < count )
    {
        node = mini_hash_get_oldest_node(minihash);
        if (!node)
            return total;

        ret = mini_hash_remove_node(minihash, node);

        if (ret != 0)
            break;

        total++;
    }

    return total;
}

void mini_hash_lru_update(mini_hash_t *minihash, void *key, int key_len)
{
	mini_hash_node_t *node;
    node = mini_hash_find_internal(minihash, key, key_len);

    if(NULL == node)
	    return ;

    mini_hash_lru_update_node(minihash,node);
}


void mini_hash_lru_update_node(mini_hash_t *minihash, mini_hash_node_t *node)
{

   if (NULL == node)
	    return ;

   if (minihash->link_head!=NULL &&	minihash->link_head!= node)	
   {	  
	   mini_hash_node_t  *next, *prev;	
	   next = node->link_next;		
	   prev = node->link_pre;				
	   minihash->link_head->link_pre = node ;				
	   node->link_next = minihash->link_head;		
	   node->link_pre = NULL;			
	   if(next) next->link_pre = prev; 	
	   if(prev) prev->link_next = next; 		
	   minihash->link_head= node;	

	   if(minihash->link_tail==node)
	   	      minihash->link_tail=prev;
	   	
   }

   
}
