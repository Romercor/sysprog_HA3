#define LEVELS 4

#include "../lib/MLF.h"
#include <stdio.h>
#include <stdlib.h>
//You can add more global variables here

static queue_object *MLF_queues[LEVELS]; 
 
static int time_slices[LEVELS] = {1, 2, 4, 8}; 

void add_process_to_queue(process *proc, int level) {
    queue_object *temp = (queue_object *)malloc(sizeof(queue_object)); 
    if (temp == NULL) 
	{
        exit(EXIT_FAILURE); 
    } 

    temp->object = proc; 
    temp->next = NULL; 

    queue_object *previos_object = MLF_queues[level]; 
    queue_object *next_object = previos_object->next; 

    while (next_object != NULL && ((process *)next_object->object)->priority >= proc->priority) { 
        previos_object = next_object; 
        next_object = next_object->next; 
    } 
    temp->next = next_object; 
    previos_object->next = temp; 
}

process *get_next_process(int *current_level, int *passed_time) {
    process *next_process = NULL;
    for (int i = 0; i < LEVELS; i++) { 
        if (MLF_queues[i]->next != NULL) { 
            *current_level = i; 
            next_process = queue_poll(MLF_queues[i]); 
            *passed_time = 0; 
            break; 
        } 
    } 
    return next_process;
}

process *MLF_tick(process *running_process) {
	static int passed_time = 0; 
    static int current_level = 0; 

    if (running_process != NULL) { 
        running_process->time_left--; 
        passed_time++; 

        if (running_process->time_left == 0) { 
            running_process = NULL; 
            passed_time = 0; 
        }
		else if (passed_time >= time_slices[current_level] && current_level != 3)
		{ 
            passed_time = 0; 
            int next_level;
            if (current_level + 1 < LEVELS)
			{
                next_level = current_level + 1;
            }
			else
			{
                next_level = current_level;
            }
            add_process_to_queue(running_process, next_level);
            running_process = NULL; 
        } 
    } 

    if (running_process == NULL) { 
        running_process = get_next_process(&current_level, &passed_time);
    } 

    return running_process; 
}

int MLF_startup()
{
    for (int i = 0; i < LEVELS; i++) { 
        MLF_queues[i] = new_queue(); 
        if (MLF_queues[i] == NULL){
			 return 1;
		} 
    } 
    return 0; 
}



process *MLF_new_arrival(process *arriving_process, process *running_process) {
    if (arriving_process != NULL) { 
        add_process_to_queue(arriving_process, 0);
    } 
    return running_process; 
}

void MLF_finish()
{
	for (int i = 0; i < LEVELS; i++) { 
        free_queue(MLF_queues[i]); 
    } 
}