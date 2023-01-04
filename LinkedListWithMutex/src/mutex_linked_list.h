#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include "linked_list.h"

struct mutex_linked_list{
    pthread_mutex_t lock;
    node *head;
    node *tail;
};

bool mutex_linked_list_member(struct mutex_linked_list*, u_int16_t);
void mutex_linked_list_insert(struct mutex_linked_list*, u_int16_t);
bool mutex_linked_list_delete(struct mutex_linked_list*, u_int16_t); // assumes data (nums) in linked list are unique
void mutex_linked_list_traverseLinkedList(struct mutex_linked_list*);
