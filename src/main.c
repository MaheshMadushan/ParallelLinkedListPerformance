#include "serial_linked_list.h"

int main(){
    struct serial_linked_list linked_list;
    linked_list.head = malloc(sizeof(struct node));
    linked_list.head->data = 0;
    insert(linked_list,450);
    return 0;
}