#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linked_list.h"

struct serial_linked_list{
    node *head;
    node *tail;
};

bool serial_linked_list_member(struct serial_linked_list*, u_int16_t);
void serial_linked_list_insert(struct serial_linked_list*, u_int16_t);
bool serial_linked_list_delete(struct serial_linked_list*, u_int16_t); // assumes data (nums) in linked list are unique
void serial_linked_list_traverseLinkedList(struct serial_linked_list*);
