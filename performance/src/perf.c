#include "mutex_linked_list.h"
#include "rwlock_linked_list.h"
#include "serial_linked_list.h"

int thread_count = 0;
int random_nums[1000];

struct common_thread_data{
    u_int16_t ops;
    int thread_id;
    int thread_count;
};

struct rwlock_thread_data
{
    struct rwlock_linked_list *linked_list;
    struct common_thread_data ctd;
};

struct mutex_thread_data
{
    struct mutex_linked_list *linked_list;
    struct common_thread_data ctd;
};

struct serial_thread_data
{
    struct serial_linked_list *linked_list;
    struct common_thread_data ctd;
};

void* rwlock_thread_worker(void *);
void* mutex_thread_worker(void *);
void* serial_thread_worker(void *);

int main(int arg_count, char* argv[]){

    // generating random nums for use in thread to insert delete and member functions
    for (int i = 0; i < 1000; i++)
    {
        random_nums[i] = (i * random()) % 0xffff; 
    }

    // populating linked lists of all types
    struct rwlock_linked_list *rwlock_linked_list = malloc(sizeof(struct rwlock_linked_list));
    for (u_int16_t i = 0; i < 1000; i++)
    {
        rwlock_linked_list_insert(rwlock_linked_list,i); // populate linked list
    }

    struct mutex_linked_list *mutex_linked_list = malloc(sizeof(struct mutex_linked_list));
    for (u_int16_t i = 0; i < 1000; i++)
    {
        mutex_linked_list_insert(mutex_linked_list,i); // populate linked list
    }

    struct serial_linked_list *serial_linked_list = malloc(sizeof(struct serial_linked_list));
    for (u_int16_t i = 0; i < 1000; i++)
    {
        serial_linked_list_insert(serial_linked_list,i); // populate linked list
    }

    long thread_id;
    pthread_t* thread_handles;
    
    // user input handling
    if(argv[1] == NULL){
        printf("Invalid input. should run as ./program_name <num_of_threads>\n");
        exit(0);
    }

    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    
    struct common_thread_data *common_thread_data_arr;
    common_thread_data_arr = malloc(sizeof(struct common_thread_data) * thread_count);

    for (int thread_id = 0; thread_id < thread_count; thread_id++)
    {
        common_thread_data_arr[thread_id].ops = 10000;
        common_thread_data_arr[thread_id].thread_count = thread_count;
        common_thread_data_arr[thread_id].thread_id = thread_id;
    }
    
    struct timeval te; 
    gettimeofday(&te, NULL);
    long long start_time_in_us = te.tv_usec;

    // create worker threads for rw locked linked list
    for(thread_id = 0; thread_id < thread_count; thread_id++){

        struct rwlock_thread_data *rwlock_thread_data = malloc(sizeof(struct rwlock_thread_data));

        rwlock_thread_data->linked_list = rwlock_linked_list;
        rwlock_thread_data->ctd = common_thread_data_arr[thread_id];
        
        pthread_create(&thread_handles[thread_id], NULL, (void*) rwlock_thread_worker, (void *) rwlock_thread_data);
    }
    
    for(thread_id = 0; thread_id < thread_count; thread_id++){
        pthread_join(thread_handles[thread_id], NULL);
    }

    gettimeofday(&te, NULL);                                        
    long long end_time_in_us = te.tv_usec;

    // calculate execution time
    printf("execution time %lld us\n",end_time_in_us - start_time_in_us);

    gettimeofday(&te, NULL);
    start_time_in_us = te.tv_usec;

    // create worker threads for mutex locked linked list
    for(thread_id = 0; thread_id < thread_count; thread_id++){

        struct mutex_thread_data *mutex_thread_data = malloc(sizeof(struct mutex_thread_data));

        mutex_thread_data->linked_list = mutex_linked_list;
        mutex_thread_data->ctd = common_thread_data_arr[thread_id];
        
        pthread_create(&thread_handles[thread_id], NULL, (void*) mutex_thread_worker, (void *) mutex_thread_data);
    }
    
    for(thread_id = 0; thread_id < thread_count; thread_id++){
        pthread_join(thread_handles[thread_id], NULL);
    }

    gettimeofday(&te, NULL);                                        
    end_time_in_us = te.tv_usec;

    // calculate execution time
    printf("execution time %lld us\n",end_time_in_us - start_time_in_us);
    
    gettimeofday(&te, NULL);
    start_time_in_us = te.tv_usec;

    // create worker threads for serial locked linked list
    for(thread_id = 0; thread_id < 1; thread_id++){

        struct serial_thread_data *serial_thread_data = malloc(sizeof(struct serial_thread_data));

        serial_thread_data->linked_list = serial_linked_list;
        serial_thread_data->ctd.ops = 10000;
        serial_thread_data->ctd.thread_count = 1;
        serial_thread_data->ctd.thread_id = thread_id;
        
        pthread_create(&thread_handles[thread_id], NULL, (void*) serial_thread_worker, (void *) serial_thread_data);
    }
    
    for(thread_id = 0; thread_id < thread_count; thread_id++){
        pthread_join(thread_handles[thread_id], NULL);
    }

    gettimeofday(&te, NULL);                                        
     end_time_in_us = te.tv_usec;

    // calculate execution time
    printf("execution time %lld us\n",end_time_in_us - start_time_in_us);
    free(thread_handles);
    return 0;
}

void* rwlock_thread_worker(void* void_Type_thread_arguments){

    struct rwlock_thread_data *thread_arguments = (struct rwlock_thread_data*) void_Type_thread_arguments;
    u_int16_t member_ops = thread_arguments->ctd.ops * 0.99;
    u_int16_t insert_ops = thread_arguments->ctd.ops * 0.05;
    u_int16_t delete_ops = thread_arguments->ctd.ops * 0.05;
    u_int16_t thread_id = thread_arguments->ctd.thread_id;
    u_int16_t thread_count = thread_arguments->ctd.thread_count;

    int member_ops_start = thread_id * (member_ops / thread_count) + 1;
    int member_ops_end = member_ops_start + (member_ops / thread_count) - 1;

    int insert_ops_start = thread_id * (insert_ops / thread_count) + 1;
    int insert_ops_end = insert_ops_start + (insert_ops / thread_count) - 1;

    int delete_ops_start = thread_id * (delete_ops / thread_count) + 1;
    int delete_ops_end = delete_ops_start + (delete_ops / thread_count) - 1;
    
    printf("thread id : %d, thread count : %d, ",thread_id,thread_count);
    printf("member() function workload: %d ops/thread, insert() function workload: %d ops/thread, delete() function workload: %d ops/thread\n",member_ops_end-member_ops_start,insert_ops_end-insert_ops_start,delete_ops_end-delete_ops_start);

    for (u_int16_t i = member_ops_start; i < member_ops_end; i++)
    {
        rwlock_linked_list_member(thread_arguments->linked_list,random_nums[i % 1000]);
    }

    for (u_int16_t i = insert_ops_start; i < insert_ops_end; i++)
    {
        rwlock_linked_list_insert(thread_arguments->linked_list,random_nums[i % 1000]);
    }

    for (u_int16_t i = delete_ops_start; i < delete_ops_end; i++)
    {
        rwlock_linked_list_delete(thread_arguments->linked_list,random_nums[i % 1000]);
    }

    return NULL;
}

void* mutex_thread_worker(void* void_Type_thread_arguments){

    struct mutex_thread_data *thread_arguments = (struct mutex_thread_data*) void_Type_thread_arguments;
    u_int16_t member_ops = thread_arguments->ctd.ops * 0.99;
    u_int16_t insert_ops = thread_arguments->ctd.ops * 0.05;
    u_int16_t delete_ops = thread_arguments->ctd.ops * 0.05;
    u_int16_t thread_id = thread_arguments->ctd.thread_id;
    u_int16_t thread_count = thread_arguments->ctd.thread_count;

    int member_ops_start = thread_id * (member_ops / thread_count) + 1;
    int member_ops_end = member_ops_start + (member_ops / thread_count) - 1;

    int insert_ops_start = thread_id * (insert_ops / thread_count) + 1;
    int insert_ops_end = insert_ops_start + (insert_ops / thread_count) - 1;

    int delete_ops_start = thread_id * (delete_ops / thread_count) + 1;
    int delete_ops_end = delete_ops_start + (delete_ops / thread_count) - 1;
    
    printf("thread id : %d, thread count : %d, ",thread_id,thread_count);
    printf("member() function workload: %d ops/thread, insert() function workload: %d ops/thread, delete() function workload: %d ops/thread\n",member_ops_end-member_ops_start,insert_ops_end-insert_ops_start,delete_ops_end-delete_ops_start);

    for (u_int16_t i = member_ops_start; i < member_ops_end; i++)
    {
        mutex_linked_list_member(thread_arguments->linked_list,random_nums[i % 1000]);
    }

    for (u_int16_t i = insert_ops_start; i < insert_ops_end; i++)
    {
        mutex_linked_list_insert(thread_arguments->linked_list,random_nums[i % 1000]);
    }

    for (u_int16_t i = delete_ops_start; i < delete_ops_end; i++)
    {
        mutex_linked_list_delete(thread_arguments->linked_list,random_nums[i % 1000]);
    }

    return NULL;
}

void* serial_thread_worker(void* void_Type_thread_arguments){

    struct serial_thread_data *thread_arguments = (struct serial_thread_data*) void_Type_thread_arguments;
    u_int16_t member_ops = thread_arguments->ctd.ops * 0.99;
    u_int16_t insert_ops = thread_arguments->ctd.ops * 0.05;
    u_int16_t delete_ops = thread_arguments->ctd.ops * 0.05;
    u_int16_t thread_id = thread_arguments->ctd.thread_id;
    u_int16_t thread_count = thread_arguments->ctd.thread_count;

    int member_ops_start = thread_id * (member_ops / thread_count) + 1;
    int member_ops_end = member_ops_start + (member_ops / thread_count) - 1;

    int insert_ops_start = thread_id * (insert_ops / thread_count) + 1;
    int insert_ops_end = insert_ops_start + (insert_ops / thread_count) - 1;

    int delete_ops_start = thread_id * (delete_ops / thread_count) + 1;
    int delete_ops_end = delete_ops_start + (delete_ops / thread_count) - 1;
    
    printf("thread id : %d, thread count : %d, ",thread_id,thread_count);
    printf("member() function workload: %d ops/thread, insert() function workload: %d ops/thread, delete() function workload: %d ops/thread\n",member_ops_end-member_ops_start,insert_ops_end-insert_ops_start,delete_ops_end-delete_ops_start);

    for (u_int16_t i = member_ops_start; i < member_ops_end; i++)
    {
        serial_linked_list_member(thread_arguments->linked_list,random_nums[i % 1000]);
    }

    for (u_int16_t i = insert_ops_start; i < insert_ops_end; i++)
    {
        serial_linked_list_insert(thread_arguments->linked_list,random_nums[i % 1000]);
    }

    for (u_int16_t i = delete_ops_start; i < delete_ops_end; i++)
    {
        serial_linked_list_delete(thread_arguments->linked_list,random_nums[i % 1000]);
    }

    return NULL;
}