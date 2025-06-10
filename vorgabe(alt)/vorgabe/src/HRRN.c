#include "../lib/HRRN.h"

static queue_object *HRRN_queue;
//You can add more global variables and structs here

static int time;

void print_queue_hrrn(queue_object *queue) {
    if (queue == NULL || queue->next == NULL) {
        return;
    }

    queue_object *current = queue->next;
    while (current != NULL) {
        process *proc = (process *)current->object;
        current = current->next;
    }
}

int process_remove(queue_object *queue, process *target_process) {
    if (queue == NULL || queue->next == NULL) {
        return 1;
    }

    queue_object *current = queue->next;
    queue_object *previous = queue;

    while (current != NULL) {
        HRRN_process *hrrn_proc = (HRRN_process *)current->object;
        if (hrrn_proc->this_process == target_process) {
            previous->next = current->next;
            free(hrrn_proc);
            free(current);
            return 0;
        }
        previous = current;
        current = current->next;
    }

    return 1;
}

void update_response_ratio(HRRN_process *hrrn_proc) {
    hrrn_proc->rr = (float)(hrrn_proc->waiting_time + hrrn_proc->this_process->time_left) / hrrn_proc->this_process->time_left;
}

process *HRRN_tick(process *running_process)
{
	// TODO

	queue_object *current = HRRN_queue->next;
    while (current != NULL) {
        HRRN_process *current_hrrn_proc = (HRRN_process *)current->object;
        current_hrrn_proc->waiting_time++;
        update_response_ratio(current_hrrn_proc);
        current = current->next;
    }

    if (running_process != NULL && running_process->time_left > 0) {
        running_process->time_left--;
        time++;
        if (running_process->time_left > 0) {
            return running_process;
        }
        process_remove(HRRN_queue, running_process);
        running_process = NULL;
    }

    current = HRRN_queue->next;
    queue_object *max_rr_process = NULL;
    float max_rr = -1.0;

    while (current != NULL) {
        HRRN_process *current_hrrn_proc = (HRRN_process *)current->object;
        if (current_hrrn_proc->rr > max_rr) {
            max_rr = current_hrrn_proc->rr;
            max_rr_process = current;
        }
        current = current->next;
    }

    if (max_rr_process != NULL) {
        HRRN_process *selected_hrrn_proc = (HRRN_process *)max_rr_process->object;
        running_process = selected_hrrn_proc->this_process;
        process_remove(HRRN_queue, running_process);
    }

    return running_process;
}

int HRRN_startup()
{
	// TODO
	HRRN_queue = new_queue();
	if (HRRN_queue == NULL){
		return 1;
	}
	return 0;
}

process *HRRN_new_arrival(process *arriving_process, process *running_process)
{

	if (arriving_process != NULL) {
        HRRN_process *new_process_hrrn = (HRRN_process *)malloc(sizeof(HRRN_process));
        if (new_process_hrrn == NULL) {
            return running_process;
        }

        new_process_hrrn->this_process = arriving_process;
        new_process_hrrn->waiting_time = 0;
        update_response_ratio(new_process_hrrn);

        if (queue_add(new_process_hrrn, HRRN_queue) != 0) {
            free(new_process_hrrn);
            return running_process;
        }
    }

    return running_process;
}

void HRRN_finish()
{
	// TODO
	free_queue(HRRN_queue);
}
