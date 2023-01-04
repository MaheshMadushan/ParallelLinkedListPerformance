#include "serial_linked_list.h"

// assumes data (nums) in linked list are unique
bool serial_linked_list_member(struct serial_linked_list *linked_list, u_int16_t data){

    node *temp_node = linked_list->head;

    while (temp_node)
    {
        if(temp_node->data == data){ return true; }
        temp_node = temp_node->next;
    }

    return false;
};

void serial_linked_list_insert(struct serial_linked_list *linked_list, u_int16_t data){

    if(linked_list->head == NULL){
        linked_list->head = malloc(sizeof(struct node));
        linked_list->head->data = data;
        linked_list->head->next = NULL;
        linked_list->tail = linked_list->head;
        return;
    }

    linked_list->tail->next = malloc(sizeof(struct node));
    linked_list->tail->next->data = data;
    linked_list->tail->next->next = NULL;
    linked_list->tail = linked_list->tail->next;
};

bool serial_linked_list_delete(struct serial_linked_list *linked_list, u_int16_t data){

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
void serial_linked_list_traverseLinkedList(struct serial_linked_list* linked_list){
    node *temp_node = linked_list->head;
    while (temp_node)
    {
        printf("%d ->",temp_node->data);
        temp_node = temp_node->next;
    }
    printf("\n");
};