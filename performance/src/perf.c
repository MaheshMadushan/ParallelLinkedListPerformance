#include "mutex_linked_list.h"
#include "rwlock_linked_list.h"
#include "serial_linked_list.h"
#include <math.h> 
#define SAMPLES_PER_WORKLOAD 97
#define NUM_OF_OPS 10000
int thread_count = 0;
int random_nums[10000];

struct common_thread_data{
    u_int16_t ops;
    int thread_id;
    int thread_count;
    float member_ops_fraction;
    float insert_ops_fraction;
    float delete_ops_fraction;
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
    float work_loads[3][3] = {{ 0.99, 0.005 , 0.005},{ 0.90, 0.05 , 0.05},{ 0.50, 0.25 , 0.25}};

    struct timeval te; 
    long long start_time_in_us = 0;
    long long end_time_in_us = 0;
    
    struct common_thread_data *common_thread_data_arr;
    common_thread_data_arr = malloc(sizeof(struct common_thread_data) * thread_count);

    //================================RW LOCKED LINKED LIST===================================================

    for (int i = 0; i < 3; i++)
    {
        
        // for calculating standard deviation and avg
        double sum_of_execution_times = 0;
        double squared_sum = 0;
        double mean = 0;
        double std = 0;
        double execution_times_arr[SAMPLES_PER_WORKLOAD];
        // initializing arr
        for (int i = 0; i < SAMPLES_PER_WORKLOAD; i++)
        {
            execution_times_arr[i] = 0;
        }
        

        // trials for a given workload
        for(int trial = 0; trial < SAMPLES_PER_WORKLOAD; trial++)
        {
            // prepare thread details
            for (int thread_id = 0; thread_id < thread_count; thread_id++)
            {

                common_thread_data_arr[thread_id].ops = NUM_OF_OPS;                              // num of ops all threads should do
                common_thread_data_arr[thread_id].thread_count = thread_count;
                common_thread_data_arr[thread_id].thread_id = thread_id;
                common_thread_data_arr[thread_id].member_ops_fraction = work_loads[i][0];   // means need member fucntion called 0.99 * ops times
                common_thread_data_arr[thread_id].delete_ops_fraction = work_loads[i][1];   // means need member fucntion called 0.005 * ops times
                common_thread_data_arr[thread_id].insert_ops_fraction = work_loads[i][2];   // means need member fucntion called 0.005 * ops times
            
            }
            
            gettimeofday(&te, NULL);
            start_time_in_us = te.tv_usec;

            // create worker threads for rw locked linked list
            for(thread_id = 0; thread_id < thread_count; thread_id++){

                // set thread function arguments
                struct rwlock_thread_data *rwlock_thread_data = malloc(sizeof(struct rwlock_thread_data));
                rwlock_thread_data->linked_list = rwlock_linked_list;
                rwlock_thread_data->ctd = common_thread_data_arr[thread_id];
                
                pthread_create(&thread_handles[thread_id], NULL, (void*) rwlock_thread_worker, (void *) rwlock_thread_data);
            }
            
            // join worker threads for rw locked linked list
            for(thread_id = 0; thread_id < thread_count; thread_id++){
                pthread_join(thread_handles[thread_id], NULL);
            }

            gettimeofday(&te, NULL);                                        
            end_time_in_us = te.tv_usec;

            // calculate execution time
            double execution_time_ms = (end_time_in_us - start_time_in_us) / 1000;
            if(execution_time_ms > 0){ execution_times_arr[trial] = execution_time_ms;}
#ifdef DEBUG
            printf("execution time rw locked linked list for work load {%f,%f,%f} : %lld ms\n",work_loads[i][0],work_loads[i][1],work_loads[i][2],execution_time_ms);
#endif
        }
        for (int i = 0; i < SAMPLES_PER_WORKLOAD; i++)
        {
            sum_of_execution_times += execution_times_arr[i];            
        }


        mean = sum_of_execution_times / SAMPLES_PER_WORKLOAD;

        for (int i = 0; i < SAMPLES_PER_WORKLOAD; i++)
        {
            squared_sum += (execution_times_arr[i] - mean) * (execution_times_arr[i] - mean);
        }

        std = sqrt(squared_sum / SAMPLES_PER_WORKLOAD);
        
        printf("avg execution time and std for rwlocked linked list for workload {%f,%f,%f}: %lf , %lf\n",work_loads[i][0],work_loads[i][1],work_loads[i][2],mean,std);
    }
    printf("\n");

    //==================================MUTEX LOCKED LINKED LIST==================================================

    for (int i = 0; i < 3; i++)
    {   
        
        // for calculating standard deviation and avg
        double sum_of_execution_times = 0;
        double squared_sum = 0;
        double mean = 0;
        double std = 0;
        double execution_times_arr[SAMPLES_PER_WORKLOAD];
        // initializing arr
        for (int i = 0; i < SAMPLES_PER_WORKLOAD; i++)
        {
            execution_times_arr[i] = 0;
        }

        // trials for a given workload
        for(int trial = 0; trial < SAMPLES_PER_WORKLOAD; trial++)
        {
            for (int thread_id = 0; thread_id < thread_count; thread_id++)
            {
                common_thread_data_arr[thread_id].ops = NUM_OF_OPS;
                common_thread_data_arr[thread_id].thread_count = thread_count;
                common_thread_data_arr[thread_id].thread_id = thread_id;
                common_thread_data_arr[thread_id].member_ops_fraction = work_loads[i][0];
                common_thread_data_arr[thread_id].delete_ops_fraction = work_loads[i][1];
                common_thread_data_arr[thread_id].insert_ops_fraction = work_loads[i][2];
            }
            
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
            double execution_time_ms = (end_time_in_us - start_time_in_us) / 1000;
            if(execution_time_ms > 0){ execution_times_arr[trial] = execution_time_ms;}
#ifdef DEBUG
            printf("execution time mutex locked linked list for work load {%f,%f,%f} : %lld ms\n",work_loads[i][0],work_loads[i][1],work_loads[i][2],execution_time_ms);
#endif        
        }
        for (int i = 0; i < SAMPLES_PER_WORKLOAD; i++)
        {
            sum_of_execution_times += execution_times_arr[i];            
        }


        mean = sum_of_execution_times / SAMPLES_PER_WORKLOAD;

        for (int i = 0; i < SAMPLES_PER_WORKLOAD; i++)
        {
            squared_sum += (execution_times_arr[i] - mean) * (execution_times_arr[i] - mean);
        }

        std = sqrt(squared_sum / SAMPLES_PER_WORKLOAD);
        
        printf("avg execution time and std for rwlocked linked list for workload  {%f,%f,%f} : %lf , %lf\n",work_loads[i][0],work_loads[i][1],work_loads[i][2],mean,std);
    }
    printf("\n");
    
    //==================================SERIAL LINKED LIST===================================================

    for (int i = 0; i < 3; i++)
    {

        // for calculating standard deviation and avg
        double sum_of_execution_times = 0;
        double squared_sum = 0;
        double mean = 0;
        double std = 0;
        double execution_times_arr[SAMPLES_PER_WORKLOAD];
        // initializing arr
        for (int i = 0; i < SAMPLES_PER_WORKLOAD; i++)
        {
            execution_times_arr[i] = 0;
        }
       
       // trials for a given workload
        for(int trial = 0; trial < SAMPLES_PER_WORKLOAD; trial++){

            gettimeofday(&te, NULL);
            start_time_in_us = te.tv_usec;

            // create worker threads for serial locked linked list
            for(thread_id = 0; thread_id < 1; thread_id++){

                struct serial_thread_data *serial_thread_data = malloc(sizeof(struct serial_thread_data));

                serial_thread_data->linked_list = serial_linked_list;
                serial_thread_data->ctd.ops = NUM_OF_OPS;
                serial_thread_data->ctd.thread_count = 1;
                serial_thread_data->ctd.thread_id = thread_id;

                serial_thread_data->ctd.member_ops_fraction = work_loads[i][0];
                serial_thread_data->ctd.delete_ops_fraction = work_loads[i][1];
                serial_thread_data->ctd.insert_ops_fraction = work_loads[i][2];
                
                pthread_create(&thread_handles[thread_id], NULL, (void*) serial_thread_worker, (void *) serial_thread_data);
            }
            
            // join worker threads for rw locked linked list
            for(thread_id = 0; thread_id < 1; thread_id++){
                pthread_join(thread_handles[thread_id], NULL);
            }

            gettimeofday(&te, NULL);                                        
            end_time_in_us = te.tv_usec;

            // calculate execution time
            double execution_time_ms = (end_time_in_us - start_time_in_us) / 1000;
            if(execution_time_ms > 0){ execution_times_arr[trial] = execution_time_ms;}
#ifdef DEBUG
            printf("execution time serial linked list for work load {%f,%f,%f} : %lld ms\n",work_loads[i][0],work_loads[i][1],work_loads[i][2],execution_time_ms);
#endif
        }
        for (int i = 0; i < SAMPLES_PER_WORKLOAD; i++)
        {
            sum_of_execution_times += execution_times_arr[i];            
        }


        mean = sum_of_execution_times / SAMPLES_PER_WORKLOAD;

        for (int i = 0; i < SAMPLES_PER_WORKLOAD; i++)
        {
            squared_sum += (execution_times_arr[i] - mean) * (execution_times_arr[i] - mean);
        }

        std = sqrt(squared_sum / SAMPLES_PER_WORKLOAD);

        printf("avg execution time and std of serial linked list for workload {%f,%f,%f} : %lf , %lf\n",work_loads[i][0],work_loads[i][1],work_loads[i][2],mean,std);
    }
    
    // //==========================================================================================================

    free(thread_handles);
    return 0;
}

void* rwlock_thread_worker(void* void_Type_thread_arguments){

    struct rwlock_thread_data *thread_arguments = (struct rwlock_thread_data*) void_Type_thread_arguments;
    u_int16_t member_ops = thread_arguments->ctd.ops * thread_arguments->ctd.member_ops_fraction;
    u_int16_t insert_ops = thread_arguments->ctd.ops * thread_arguments->ctd.insert_ops_fraction;
    u_int16_t delete_ops = thread_arguments->ctd.ops * thread_arguments->ctd.delete_ops_fraction;
    u_int16_t thread_id = thread_arguments->ctd.thread_id;
    u_int16_t thread_count = thread_arguments->ctd.thread_count;

    int member_ops_start = thread_id * (member_ops / thread_count) + 1;
    int member_ops_end = member_ops_start + (member_ops / thread_count) - 1;

    int insert_ops_start = thread_id * (insert_ops / thread_count) + 1;
    int insert_ops_end = insert_ops_start + (insert_ops / thread_count) - 1;

    int delete_ops_start = thread_id * (delete_ops / thread_count) + 1;
    int delete_ops_end = delete_ops_start + (delete_ops / thread_count) - 1;
    
#ifdef DEBUG
    printf("thread id : %d, thread count : %d, ",thread_id,thread_count);
    printf("member() function workload: %d ops/thread, insert() function workload: %d ops/thread, delete() function workload: %d ops/thread\n",member_ops_end-member_ops_start,insert_ops_end-insert_ops_start,delete_ops_end-delete_ops_start);
#endif
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
    u_int16_t member_ops = thread_arguments->ctd.ops * thread_arguments->ctd.member_ops_fraction;
    u_int16_t insert_ops = thread_arguments->ctd.ops * thread_arguments->ctd.insert_ops_fraction;
    u_int16_t delete_ops = thread_arguments->ctd.ops * thread_arguments->ctd.delete_ops_fraction;
    u_int16_t thread_id = thread_arguments->ctd.thread_id;
    u_int16_t thread_count = thread_arguments->ctd.thread_count;

    int member_ops_start = thread_id * (member_ops / thread_count) + 1;
    int member_ops_end = member_ops_start + (member_ops / thread_count) - 1;

    int insert_ops_start = thread_id * (insert_ops / thread_count) + 1;
    int insert_ops_end = insert_ops_start + (insert_ops / thread_count) - 1;

    int delete_ops_start = thread_id * (delete_ops / thread_count) + 1;
    int delete_ops_end = delete_ops_start + (delete_ops / thread_count) - 1;
#ifdef DEBUG
    printf("thread id : %d, thread count : %d, ",thread_id,thread_count);
    printf("member() function workload: %d ops/thread, insert() function workload: %d ops/thread, delete() function workload: %d ops/thread\n",member_ops_end-member_ops_start,insert_ops_end-insert_ops_start,delete_ops_end-delete_ops_start);
#endif
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
   u_int16_t member_ops = thread_arguments->ctd.ops * thread_arguments->ctd.member_ops_fraction;
    u_int16_t insert_ops = thread_arguments->ctd.ops * thread_arguments->ctd.insert_ops_fraction;
    u_int16_t delete_ops = thread_arguments->ctd.ops * thread_arguments->ctd.delete_ops_fraction;
    u_int16_t thread_id = thread_arguments->ctd.thread_id;
    u_int16_t thread_count = thread_arguments->ctd.thread_count;

    int member_ops_start = thread_id * (member_ops / thread_count) + 1;
    int member_ops_end = member_ops_start + (member_ops / thread_count) - 1;

    int insert_ops_start = thread_id * (insert_ops / thread_count) + 1;
    int insert_ops_end = insert_ops_start + (insert_ops / thread_count) - 1;

    int delete_ops_start = thread_id * (delete_ops / thread_count) + 1;
    int delete_ops_end = delete_ops_start + (delete_ops / thread_count) - 1;
#ifdef DEBUG
    printf("thread id : %d, thread count : %d, ",thread_id,thread_count);
    printf("member() function workload: %d ops/thread, insert() function workload: %d ops/thread, delete() function workload: %d ops/thread\n",member_ops_end-member_ops_start,insert_ops_end-insert_ops_start,delete_ops_end-delete_ops_start);
#endif
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