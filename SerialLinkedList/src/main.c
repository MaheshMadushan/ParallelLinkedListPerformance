// #include "serial_linked_list.h"
// bool is950deleted = false;
// bool is500Member = false;
// bool is800Member = false;
// int main(){
//     struct serial_linked_list *linked_list = malloc(sizeof(struct serial_linked_list));
//     for (int i = 0; i < 100000; i++)
//     {
//         serial_linked_list_insert(linked_list,i);
//     }
//     is950deleted = serial_linked_list_delete(linked_list,950);
//     printf("num 950 is exist and now deleted ? : %s\n", (is950deleted ? "true":"false"));
//     is500Member = serial_linked_list_member(linked_list,500);
//     printf("num 500 is member ? : %s\n",(is500Member ? "true":"false"));
//     is800Member = serial_linked_list_member(linked_list,800);
//     printf("num 800 is member ? : %s\n",(is800Member ? "true":"false"));
//     serial_linked_list_traverseLinkedList(linked_list);
//     return 0;
// }