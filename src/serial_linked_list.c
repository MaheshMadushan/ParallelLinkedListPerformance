#include "serial_linked_list.h"

// assumes data (nums) in linked list are unique
bool member(struct serial_linked_list *linked_list, u_int16_t data){
    node *temp_node = linked_list->head;
    while (temp_node)
    {
        if(temp_node->data == data){ return true; }
        temp_node = temp_node->next;
    }
    return false;
};
void insert(struct serial_linked_list *linked_list, u_int16_t data){
    node *ptr_to_node = linked_list->head;
    if(ptr_to_node == NULL){
        linked_list->head = malloc(sizeof(struct node));
        linked_list->head->data = data;
        linked_list->head->next = NULL;
        return;
    }
    while (ptr_to_node->next){ ptr_to_node = ptr_to_node->next; }
    ptr_to_node->next = malloc(sizeof(struct node));
    ptr_to_node->next->data = data;
    ptr_to_node->next->next = NULL;
};
bool delete(struct serial_linked_list *linked_list, u_int16_t data){
    node *temp_node = linked_list->head;
    node *prev_node = NULL;
    while (temp_node)
    {
        if(temp_node->data == data){
            if(temp_node->next == NULL){
                prev_node->next = NULL;
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
}; 
void traverseLinkeList(struct serial_linked_list* linked_list){
    node *temp_node = linked_list->head;
    while (temp_node)
    {
        printf("%d ->",temp_node->data);
        temp_node = temp_node->next;
    }
    printf("\n");
};