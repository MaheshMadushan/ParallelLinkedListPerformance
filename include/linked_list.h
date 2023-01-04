#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node node;

struct node{
    u_int16_t data;
    node *next;
};

#endif // LINKED_LIST_H