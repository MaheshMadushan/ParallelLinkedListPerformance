#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node node;

struct node{
    u_int16_t data;
    node *next;
};

struct serial_linked_list{
    node *head;
};

bool member(struct serial_linked_list, u_int16_t);
void insert(struct serial_linked_list, u_int16_t);
bool delete(struct serial_linked_list, u_int16_t); // assumes data (nums) in linked list are unique

