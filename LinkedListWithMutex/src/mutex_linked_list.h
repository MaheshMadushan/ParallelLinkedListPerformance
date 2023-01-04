#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
typedef struct node node;

struct node{
    u_int16_t data;
    node *next;
};

struct mutex_linked_list{
    pthread_mutex_t lock;
    node *head;
    node *tail;
};

bool member(struct mutex_linked_list*, u_int16_t);
void insert(struct mutex_linked_list*, u_int16_t);
bool delete(struct mutex_linked_list*, u_int16_t); // assumes data (nums) in linked list are unique
void traverseLinkeList(struct mutex_linked_list*);
