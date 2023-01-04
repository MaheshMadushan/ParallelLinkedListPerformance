#include "rwlock_linked_list.h"

// assumes data (nums) in linked list are unique
bool member(struct mutex_linked_list *linked_list, u_int16_t data){
    pthread_rwlock_rdlock(&linked_list->lock);
    node *temp_node = linked_list->head;

    while (temp_node)
    {
        if(temp_node->data == data){ 
            pthread_rwlock_unlock(&linked_list->lock);
            return true; 
        }
        temp_node = temp_node->next;
    }

    pthread_rwlock_unlock(&linked_list->lock);
    return false;
};

void insert(struct mutex_linked_list *linked_list, u_int16_t data){

    pthread_rwlock_wrlock(&linked_list->lock);

    if(linked_list->head == NULL){
        linked_list->head = malloc(sizeof(struct node));
        linked_list->head->data = data;
        linked_list->head->next = NULL;
        linked_list->tail = linked_list->head;
        pthread_rwlock_unlock(&linked_list->lock);
        return;
    }

    linked_list->tail->next = malloc(sizeof(struct node));
    linked_list->tail->next->data = data;
    linked_list->tail->next->next = NULL;
    linked_list->tail = linked_list->tail->next;
    pthread_rwlock_unlock(&linked_list->lock);

};

bool delete(struct mutex_linked_list *linked_list, u_int16_t data){

    pthread_rwlock_wrlock(&linked_list->lock);
    node *temp_node = linked_list->head;
    node *prev_node = NULL;

    while (temp_node)
    {
        if(temp_node->data == data){
            if(temp_node->next == NULL){
                prev_node->next = NULL;
                pthread_rwlock_unlock(&linked_list->lock);
                return true;
            }
            prev_node->next = temp_node->next->next;
            free(temp_node->next);
            pthread_rwlock_unlock(&linked_list->lock);
            return true;
        }
        prev_node = temp_node;
        temp_node = temp_node->next;
    }

    pthread_rwlock_unlock(&linked_list->lock);
    return false;
}; 

void traverseLinkeList(struct mutex_linked_list* linked_list){

    node *temp_node = linked_list->head;

    while (temp_node)
    {
        printf("%d ->",temp_node->data);
        temp_node = temp_node->next;
    }
    printf("\n");

};