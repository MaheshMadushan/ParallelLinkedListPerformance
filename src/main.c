#include "serial_linked_list.h"
bool is950deleted = false;
bool is500Member = false;
bool is800Member = false;
int main(){
    struct serial_linked_list *linked_list = malloc(sizeof(struct serial_linked_list));
    for (u_int16_t i = 0; i < 100; i++)
    {
        insert(linked_list,i);
    }
    is950deleted = delete(linked_list,950);
    printf("num 950 is exist and now deleted ? : %s\n", (is950deleted ? "true":"false"));
    is500Member = member(linked_list,500);
    printf("num 500 is member ? : %s\n",(is500Member ? "true":"false"));
    is800Member = member(linked_list,800);
    printf("num 800 is member ? : %s\n",(is800Member ? "true":"false"));
    traverseLinkeList(linked_list);
    return 0;
}