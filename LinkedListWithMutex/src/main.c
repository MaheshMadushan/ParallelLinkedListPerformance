#include "mutex_linked_list.h"
bool is950deleted = false;
bool is500Member = false;
bool is800Member = false;
struct thread_data
{
    struct mutex_linked_list *linked_list;
    u_int16_t ops;
    int thread_id;
    int thread_count;
};

void* thread_worker(void *);
int thread_count = 0;

int main(int arg_count, char* argv[]){
    // generating random nums for use in threads
    int random_nums[1000];
    for (int i = 0; i < 1000; i++)
    {
        random_nums[i] = (i * random()) % 0xff;
    }
    


    struct mutex_linked_list *linked_list = malloc(sizeof(struct mutex_linked_list));
    for (u_int16_t i = 0; i < 1000; i++)
    {
        insert(linked_list,i); // populate linked list
    }
        
    long thread_id;
    pthread_t* thread_handles;

    if(argv[1] == NULL){
        printf("Invalid input. should run as ./program_name <num_of_threads>\n");
        exit(0);
    }
    thread_count = strtol(argv[1], NULL, 10);
    
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    struct timeval te; 
    gettimeofday(&te, NULL);
    long long start_time_in_ms = te.tv_usec; 

    for(thread_id = 0; thread_id < thread_count; thread_id++){
        struct thread_data *thread_data = malloc(sizeof(struct thread_data));
        thread_data->linked_list = linked_list;
        thread_data->ops = 10000;
        thread_data->thread_count = thread_count;
        thread_data->thread_id = thread_id;
        pthread_create(&thread_handles[thread_id], NULL, (void*) thread_worker, (void *) thread_data);
    }
    
    for(thread_id = 0; thread_id < thread_count; thread_id++){
        pthread_join(thread_handles[thread_id], NULL);
    }

    gettimeofday(&te, NULL);                                        
    long long end_time_in_ms = te.tv_usec;  

    printf("execution time %lld us\n",end_time_in_ms - start_time_in_ms);
    free(thread_handles);
    return 0;
}

void* thread_worker(void* void_Type_thread_arguments){

    struct thread_data *thread_arguments = (struct thread_data*) void_Type_thread_arguments;
    u_int16_t ops = thread_arguments->ops;
    u_int16_t thread_id = thread_arguments->thread_id;
    u_int16_t thread_count = thread_arguments->thread_count;

    int start = thread_id * (ops / thread_count) + 1;
    int end = start + (ops / thread_count) - 1;
    
    // printf("thread id : %d, thread count : %d, ",thread_id,thread_count);
    // printf("start : %d, end : %d, workload of thread: %d, full workload : %d\n",start,end,end-start,ops );

    for (u_int16_t i = start; i < end; i++)
    {
        member(thread_arguments->linked_list,i);
    }

    return NULL;
}