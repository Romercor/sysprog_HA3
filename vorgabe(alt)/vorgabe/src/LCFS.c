#include "../lib/LCFS.h"

static queue_object *LCFS_queue;
//You can add more global variables here


process *get_last_process(queue_object *queue) {
	if (queue == NULL || queue->next == NULL) {
        return NULL;
    }

    queue_object *current = queue;
    queue_object *previous = NULL;

    while (current->next != NULL) {
        previous = current;
        current = current->next;
    }

    if (previous != NULL) {
        previous->next = NULL;
    } else {
        queue->next = NULL;
    }

    process *end_process = (process *)current->object;
    free(current);

    return end_process;
}


int LCFS_startup()
{
	// TODO
	LCFS_queue = new_queue();
	if (LCFS_queue == NULL){
		return 1;
	}
	return 0;
}

process *LCFS_tick(process *running_process)
{
    if (running_process != NULL && running_process->time_left > 0) {
        running_process->time_left--;
        if (running_process->time_left >= 0) {
            return running_process;
        } 
		else
		{
            running_process = get_last_process(LCFS_queue);
			running_process->time_left--;
            if (running_process == NULL)
			{
                return NULL;
            }
            return running_process;
        }
    }

    if (running_process == NULL || running_process->time_left == 0) {
        if (LCFS_queue->next == NULL) {
            return NULL;
        }
        running_process = get_last_process(LCFS_queue);
        if (running_process == NULL) {
            return NULL;
        }
		running_process->time_left--;
        return running_process;
    }

    return running_process;
}

process *LCFS_new_arrival(process *arriving_process, process *running_process) {
    if (arriving_process != NULL) 
	{
        queue_add(arriving_process, LCFS_queue);
    }
    if (running_process == NULL) {
        running_process = get_last_process(LCFS_queue);
    }
    if (running_process != NULL) {
    }
    return running_process;
}

void LCFS_finish()
{
	// TODO
	free_queue(LCFS_queue);
}
