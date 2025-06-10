#include "../lib/SRTN.h"

static queue_object *SRTN_queue;
//You can add more global variables here

process *find_the_shortest_process(queue_object *queue) {
    if (queue == NULL || queue->next == NULL) {
        return NULL;
    }

    queue_object *prev = queue;
    queue_object *current = queue->next;
    queue_object *minimal_process_prev = prev;
    queue_object *minimal_process_queue_element = current;
    process *minimal_process = (process *)current->object;

    while (current != NULL) {
        process *current_process = (process *)current->object;

        if (current_process->time_left < minimal_process->time_left) {
            minimal_process = current_process;
            minimal_process_queue_element = current;
            minimal_process_prev = prev;
        }

        prev = current;
        current = current->next;
    }

    minimal_process_prev->next = minimal_process_queue_element->next;
    free(minimal_process_queue_element);
    return minimal_process;
}

process *SRTN_tick(process *running_process)
{
	// TODO
	if (running_process != NULL && running_process -> time_left > 0)
	{
		running_process -> time_left--;

		if(running_process -> time_left >= 0)
		{
			return running_process;
		}
		else{
			running_process = find_the_shortest_process(SRTN_queue);
			return running_process;
		}
	}
	process *following = (process *) find_the_shortest_process(SRTN_queue);

	
	if (following == NULL)
	{
        return NULL;
    }

	running_process = following;
	running_process -> time_left--;
		
	return running_process;
}

int SRTN_startup()
{
	// TODO
	SRTN_queue = new_queue();
    if (SRTN_queue == NULL) {
        return 1;
    }
    return 0;
}

process *SRTN_new_arrival(process *arriving_process, process *running_process)
{
	// TODO
 
    if (running_process != NULL && arriving_process != NULL)
	{ 
        if (arriving_process->time_left < running_process->time_left)
		{ 
            queue_add(running_process, SRTN_queue); 
            running_process = arriving_process; 
        }
		else if (running_process->time_left <= arriving_process->time_left)
		{
			queue_add(arriving_process, SRTN_queue); 
		}
    }
	else if (arriving_process != NULL && running_process == NULL) 
	{ 
        queue_add(arriving_process, SRTN_queue);  
    }
	else if (running_process == NULL)
	{
		running_process = (process *)find_the_shortest_process(SRTN_queue);
	}
    return running_process;
}

void SRTN_finish()
{
	// TODO
	free_queue(SRTN_queue);
}
