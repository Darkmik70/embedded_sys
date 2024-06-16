#include "command_list.h"
#include <xc.h>

/**
 * @brief Initialize command list with zeros
 * 
 * @param cmd_list ptr to CommandList struct
 */
void init_cmd_list(CommandList *cmd_list) {

    for (int i = 0; i < CMD_LIST_SIZE; i++) {
        cmd_list->type[i] = 0;
        cmd_list->time[i] = 0;
    }
    cmd_list->size = 0;
}

/**
 * @brief Add element at the end of the CommandList queue
 *
 * @param cmd_list  ptr to Command List
 * @param cmd_type  (int) Type of command
 * @param cmd_time  (int) duration of the command
 * @return (int) 1 if success, 0 if full
 */
int enqueue_cmd_list(CommandList *cmd_list, int cmd_type, int cmd_time) {
    if (is_cmd_list_full(cmd_list)) {
        return 0;
    }
    else {
        // save the elements into the queue
        cmd_list->type[cmd_list->size] = cmd_type;
        cmd_list->time[cmd_list->size] = cmd_time;
        // increment size
        cmd_list->size++;
        return 1;
    }
}

/**
 * @brief Remove the first element from CommandList fifo
 * 
 * @param cmd_list ptr to CommandList struct
 * @param cmd_type (int) type of command
 * @param cmd_time (int) time duration of command
 * @return int 1 if success, 0 if fifo is empty
 */
int dequeue_cmd_list(CommandList *cmd_list, int *cmd_type, int *cmd_time) {
    if (is_cmd_list_empty(cmd_list))
    {
        return 0;
    }
    else {
        // Get the elements from cmd_list
        *cmd_type = cmd_list->type[0];
        *cmd_time = cmd_list->time[0];

        // shift all elements one step back
        for (int i = 1; i < cmd_list->size; i++) {
            cmd_list->type[i - 1] = cmd_list->type[i];
            cmd_list->time[i - 1] = cmd_list->time[i];
        }
        // reduce the size
        cmd_list->size--;
        return 1;
    }
}

/**
 * @brief checks if the command list is full
 * 
 * @param cmd_list ptr to CommandList struct
 * @return (int) 1 if fifo is full, 0 otherwise
 */
int is_cmd_list_full(CommandList *cmd_list){
    return cmd_list->size == 10;
}
/**
 * @brief checks if command list is empty
 * 
 * @param cmd_list 
 * @return (int) 1 if fifo is empty, 0 otherwise
 */
int is_cmd_list_empty(CommandList *cmd_list){
    return cmd_list->size == 0;
}

#include "command_list.h"
#include <xc.h>

/**
 * @brief Initialize command list with zeros
 * 
 * @param cmd_list ptr to CommandList struct
 */
void init_cmd_list(CommandList *cmd_list) {

    for (int i = 0; i < CMD_LIST_SIZE; i++) {
        cmd_list->type[i] = 0;
        cmd_list->time[i] = 0;
    }
    cmd_list->size = 0;
}

/**
 * @brief Add element at the end of the CommandList queue
 *
 * @param cmd_list  ptr to Command List
 * @param cmd_type  (int) Type of command
 * @param cmd_time  (int) duration of the command
 * @return (int) 1 if success, 0 if full
 */
int enqueue_cmd_list(CommandList *cmd_list, int cmd_type, int cmd_time) {
    if (is_cmd_list_full(cmd_list)) {
        return -1;
    }
    else {
        // save the elements into the queue
        cmd_list->type[cmd_list->size] = cmd_type;
        cmd_list->time[cmd_list->size] = cmd_time;
        // increment size
        cmd_list->size++;
        return 0;
    }
}

/**
 * @brief Remove the first element from CommandList fifo
 * 
 * @param cmd_list ptr to CommandList struct
 * @param cmd_type (int) type of command
 * @param cmd_time (int) time duration of command
 * @return int 1 if success, 0 if fifo is empty
 */
int dequeue_cmd_list(CommandList *cmd_list, int *cmd_type, int *cmd_time) {
    if (is_cmd_list_empty(cmd_list))
    {
        return 0;
    }
    else {
        // Get the elements from cmd_list
        *cmd_type = cmd_list->type[0];
        *cmd_time = cmd_list->time[0];

        // shift all elements one step back
        for (int i = 1; i < cmd_list->size; i++) {
            cmd_list->type[i - 1] = cmd_list->type[i];
            cmd_list->time[i - 1] = cmd_list->time[i];
        }
        // reduce the size
        cmd_list->size--;
        return 1;
    }
}

/**
 * @brief checks if the command list is full
 * 
 * @param cmd_list ptr to CommandList struct
 * @return (int) 1 if fifo is full, 0 otherwise
 */
int is_cmd_list_full(CommandList *cmd_list){
    return cmd_list->size == 10;
}
/**
 * @brief checks if command list is empty
 * 
 * @param cmd_list 
 * @return (int) 1 if fifo is empty, 0 otherwise
 */
int is_cmd_list_empty(CommandList *cmd_list){
    return cmd_list->size == 0;
}