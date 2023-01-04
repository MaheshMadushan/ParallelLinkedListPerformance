#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "linked_list.h"

struct rwlock_linked_list{
    pthread_rwlock_t lock;
    node *head;
    node *tail;
};

bool rwlock_linked_list_member(struct rwlock_linked_list*, u_int16_t);
void rwlock_linked_list_insert(struct rwlock_linked_list*, u_int16_t);
bool rwlock_linked_list_delete(struct rwlock_linked_list*, u_int16_t); // assumes data (nums) in linked list are unique
void rwlock_linked_list_traverseLinkedList(struct rwlock_linked_list*);
