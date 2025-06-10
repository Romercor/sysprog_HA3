#include "../lib/PRIOP.h"
#include <stdio.h>

static queue_object *PRIOP_queue;
//You can add more global variables here

process *helfer_priop(queue_object *queue) {
	if (queue == NULL || queue->next == NULL) {
        return NULL;
    }

    queue_object *prev = queue;
    queue_object *current = queue->next;
    queue_object *maximal_process_prev = prev;
    queue_object *maximal_process_queue_element = current;
    process *maximal_process = (process *)current->object;

    while (current != NULL) {
        process *current_process = (process *)current->object;

        if (current_process->priority > maximal_process->priority) {
            maximal_process = current_process;
            maximal_process_queue_element = current;
            maximal_process_prev = prev;
        }

        prev = current;
        current = current->next;
    }

    maximal_process_prev->next = maximal_process_queue_element->next;
    free(maximal_process_queue_element);
    return maximal_process;
}

process *PRIOP_tick(process *running_process)
{
	// TODO
	if (running_process != NULL && running_process -> time_left > 0)
	{
		running_process -> time_left--;
		if(running_process -> time_left >= 0)
		{
			return running_process;
		}
		else
		{
			running_process = helfer_priop(PRIOP_queue);
			return running_process;
		}
	}
	process *following = (process *) helfer_priop(PRIOP_queue);

	
	if (following == NULL)
	{
        return NULL;
    }

	running_process = following;
	running_process -> time_left--;
	return running_process;
}

int PRIOP_startup()
{
	// TODO
	PRIOP_queue = new_queue();
	if (PRIOP_queue == NULL){
		return 1;
	}
	return 0;
}

process *PRIOP_new_arrival(process *arriving_process, process *running_process)
{
	if (running_process != NULL && arriving_process != NULL)
	{ 
		if (arriving_process->priority > running_process->priority)
		{ 
            queue_add(running_process, PRIOP_queue); 
            running_process = arriving_process; 
        }
		else if (running_process->priority >= arriving_process->priority)
		{
			queue_add(arriving_process, PRIOP_queue);
		}
    }
	else if (arriving_process != NULL && running_process == NULL) 
	{ 
        queue_add(arriving_process, PRIOP_queue); 
    }
	else if (running_process == NULL)
	{
		running_process = (process *)helfer_priop(PRIOP_queue);
	}
    return running_process;
}

void PRIOP_finish()
{
	// TODO
	free_queue(PRIOP_queue);
}
