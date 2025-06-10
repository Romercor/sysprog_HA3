#include "../lib/RR.h"

static queue_object *RR_queue;
//You can add more global variables

static int time_quantum;
static int current_time_slice;
static int time;

process *RR_tick(process *running_process)
{
	// TODO
	if (running_process != NULL && running_process->time_left > 0) {
        running_process->time_left--;
        current_time_slice++;
        time++;
        if (running_process->time_left == 0 || current_time_slice >= time_quantum) {
            if (running_process->time_left > 0) {
                queue_add(running_process, RR_queue);
            }
            running_process = queue_poll(RR_queue);
            current_time_slice = 0;
        }
    } else 
	{
        running_process = queue_poll(RR_queue);
        current_time_slice = 0;
    }
    return running_process;
}

int RR_startup(int quantum)
{
	// TODO
	RR_queue = new_queue();
    if (RR_queue == NULL) {
        return 1;
    }
    time_quantum = quantum;
    current_time_slice = 0;
    return 0;
}

process *RR_new_arrival(process *arriving_process, process *running_process)
{
	// TODO
	if (arriving_process != NULL) {
        if (queue_add(arriving_process, RR_queue) != 0) {
            return running_process;
        }
    }
    return running_process;
}

void RR_finish()
{
	// TODO
	free_queue(RR_queue);
}
