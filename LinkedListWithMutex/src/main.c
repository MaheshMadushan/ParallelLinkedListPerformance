// #include "mutex_linked_list.h"
// bool is950deleted = false;
// bool is500Member = false;
// bool is800Member = false;
// int main(){
//     struct mutex_linked_list *linked_list = malloc(sizeof(struct mutex_linked_list));
//     // pthread_mutex_init(&linked_list->lock,NULL);
//     for (u_int16_t i = 0; i < 1000; i++)
//     {
//         mutex_linked_list_insert(linked_list,i);
//     }
//     is950deleted = mutex_linked_list_delete(linked_list,950);
//     printf("num 950 is exist and now deleted ? : %s\n", (is950deleted ? "true":"false"));
//     is500Member = mutex_linked_list_member(linked_list,500);
//     printf("num 500 is member ? : %s\n",(is500Member ? "true":"false"));
//     is800Member = mutex_linked_list_member(linked_list,800);
//     printf("num 800 is member ? : %s\n",(is800Member ? "true":"false"));
//     mutex_linked_list_traverseLinkedList(linked_list);
//     return 0;
// }