#include "lru.h"
#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

void lru_init_queue(Set *set) {
  LRUNode *s = NULL;
  LRUNode **pp = &s;  // place to chain in the next node
  for (int i = 0; i < set->line_count; i++) {
    Line *line = &set->lines[i];
    LRUNode *node = (LRUNode *)(malloc(sizeof(LRUNode)));
    node->line = line;
    node->next = NULL;
    (*pp) = node;
    pp = &((*pp)->next);
  }
  set->lru_queue = s;
}

void lru_init(Cache *cache) {
  Set *sets = cache->sets;
  for (int i = 0; i < cache->set_count; i++) {
    lru_init_queue(&sets[i]);
  }
}

void lru_destroy(Cache *cache) {
  Set *sets = cache->sets;
  for (int i = 0; i < cache->set_count; i++) {
    LRUNode *p = sets[i].lru_queue;
    LRUNode *n = p;
    while (p != NULL) {
      p = p->next;
      free(n);
      n = p;
    }
    sets[i].lru_queue = NULL;
  }
}

void lru_fetch(Set *set, unsigned int tag, LRUResult *result) {
  // TODO:
  // Implement the LRU algorithm to determine which line in
  // the cache should be accessed.
  //
  result -> access = CONFLICT_MISS; 
  LRUNode *p = set -> lru_queue;
  LRUNode *n = p;

  while(p != NULL) {
    Line *line = p -> line;
    
    if(line -> valid == 0){     
      line -> valid = 1;
      line -> tag = tag;
      result -> line = line;
      result -> access = COLD_MISS;  
      return;
    }
    
    else if(line -> tag == tag) {
      result -> access = HIT; 
      break; 
    }
    if(p -> next == NULL){
      break;
    }
    p = p -> next; 
  }

  if(result -> access || n -> line -> tag == tag){
    p -> next = n;
    Line *l = n -> line;
    l -> tag = tag; 
    n = n -> next;
   // p -> next = l; 
    p -> next -> next = NULL;
  }
  else {
    
    LRUNode *temp = p;
    
    while(temp -> next -> line -> tag != tag) {
      temp = temp -> next;
    }
    
    temp -> next = temp -> next -> next;
    
    while(temp -> next != NULL) {
      temp = temp -> next;
    }
    temp -> next = p;
    p -> next = NULL; 
    
  }
}
