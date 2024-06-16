/*
 * File:   command_list.h
 * Author: Bua Odetti, Contreras, Krepa, Trovatello
 *
 * Created on 30 maggio 2024, 12.00
 */
#ifndef COMMAND_LIST_H
#define	COMMAND_LIST_H

#define CMD_LIST_SIZE 10

enum msg_type{
    Forward = 1,
    CounterClockwise = 2,
    Clockwise = 3,
    Backward = 4};

typedef struct {
    int type[CMD_LIST_SIZE];
    int time[CMD_LIST_SIZE];
    int size;
} CommandList;

typedef struct {
    int type;
    int time;
    int cnt;
} CurrentCommand;

/**
 * @brief Initialize command list with zeros
 * 
 * @param cmd_list ptr to CommandList struct
 */
void init_cmd_list(CommandList *cmd_list); 

/**
 * @brief Initialize command element with zeros
 * 
 * @param cmd ptr to CurrentCommand struct
 */
void init_command(CurrentCommand *cmd);

/**
 * @brief checks if the command list is full
 *
 * @param cmd_list ptr to CommandList struct
 * @return (int) 1 if fifo is full, 0 otherwise
 */
int is_cmd_list_full(CommandList *cmd_list); 

/**
 * @brief checks if command list is empty
 *
 * @param cmd_list
 * @return (int) 1 if fifo is empty, 0 otherwise
 */
int is_cmd_list_empty(CommandList *cmd_list); 

/**
 * @brief Add element at the end of the CommandList queue
 *
 * @param cmd_list  ptr to Command List
 * @param cmd_type  (int) Type of command
 * @param cmd_time  (int) duration of the command
 * @return (int) 1 if success, 0 if full
 */
int enqueue_cmd_list(CommandList *cmd_list, int cmd_type, int cmd_time);

/**
 * @brief Remove the first element from CommandList fifo
 * 
 * @param cmd_list ptr to CommandList struct
 * @param cmd_type (int) type of command
 * @param cmd_time (int) time duration of command
 * @return int 1 if success, 0 if fifo is empty
 */
int dequeue_cmd_list(CommandList *cmd_list, int *cmd_type, int *cmd_time);


#endif /* COMMAND_LIST_H */