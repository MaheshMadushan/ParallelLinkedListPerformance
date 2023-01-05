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

    node **temp_node = &linked_list->head;
    node **next_node = &(*temp_node)->next;
    while (temp_node)
    {
        if((*temp_node) == NULL) printf("%d\n",(*temp_node) == NULL);
        if((*temp_node)->data == data){
            node *free_this_node = *temp_node;
            *temp_node = next_node;
            free(free_this_node);
            return true;
        }
        // printf("%d\n",(*temp_node)->next == NULL);
        if((*temp_node)->next == NULL){
            temp_node = NULL;
            continue;
        }
        temp_node = &(*temp_node)->next;
        next_node = &(*temp_node)->next;
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