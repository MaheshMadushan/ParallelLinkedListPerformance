#include "serial_linked_list.h"

bool member(struct serial_linked_list linked_list, u_int16_t data){
    node *temp_node = linked_list.head;
    while (temp_node)
    {
        if(temp_node->data == data){
            return true;
        }
        temp_node = temp_node->next;
    }
    return true;
};

void insert(struct serial_linked_list linked_list, u_int16_t data){
    node *temp_node = linked_list.head;
    while (temp_node->next)
    {
        temp_node = temp_node->next;
    }
    temp_node->next = malloc(sizeof(struct node));
    temp_node->next->data = data;
    temp_node->next->next = NULL;
};

bool delete(struct serial_linked_list linked_list, u_int16_t data){
    node *temp_node = linked_list.head;
    node *prev_node = NULL;
    while (temp_node)
    {
        if(temp_node->data == data){
            if(temp_node->next == NULL){
                prev_node = NULL;
                return true;
            }
            prev_node->next = temp_node->next->next;
            free(temp_node->next);
            return true;
        }
        prev_node = temp_node;
        temp_node = temp_node->next;
    }
    return false;
}; // assumes data (nums) in linked list are unique
