#include "../lib/queue.h"
#include <stdlib.h>
#include <stdio.h>

int queue_add(void *new_object, queue_object *queue)
{
	// TODO
	if (new_object == NULL || queue == NULL){
		return 1;
	}

	queue_object *node = (queue_object *)malloc(sizeof(queue_object));

	if (node == NULL) {
        return 1;
    }

	node->object = new_object;
	node->next = NULL;

	queue_object *current = queue;
    while (current->next != NULL) {
        current = current->next;
    }

	current->next = node;

    return 0;
}

void *queue_poll(queue_object *queue)
{
	// TODO
	if (queue == NULL || queue->next == NULL){
		return NULL;
	}

	queue_object *oldest_node = queue->next;
	void *data = oldest_node -> object;

	queue -> next = oldest_node -> next;

	free(oldest_node);

	return data;
}

queue_object *new_queue()
{
	// TODO
	queue_object *node = (queue_object *)malloc(sizeof(queue_object));

	node -> object = NULL;
	node -> next = NULL;

	return node;
}

void free_queue(queue_object *queue)
{
	// TODO

	if (queue == NULL){
		return;
	}

	queue_object *current_node = queue->next;

	while(current_node != NULL){
		
		queue_object *next_node = current_node->next;
		free(current_node);
		current_node = next_node;
	
	}
}

void *queue_peek(queue_object *queue)
{
	// TODO
	if (queue == NULL || queue->next == NULL){
		return NULL;
	}

	queue_object *oldest_node = queue->next;

	return oldest_node -> object;
}
