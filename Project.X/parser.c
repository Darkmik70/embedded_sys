/*
 * File:   parser.c
 * Author: RomiC
 *
 * Created on 9 giugno 2024, 22.19
 */

#include "xc.h"
#include "parser.h"
#include "function.h"


int parse_byte(parser_state* ps, char byte) {
    switch (ps->state) {
        case STATE_DOLLAR:
            if (byte == '$') {
                // Transition to STATE_TYPE when '$' is received
                ps->state = STATE_TYPE;
                ps->index_type = 0; // Reset index for the message type
                toggleLed(2);
            }
            break;
            
        case STATE_TYPE:    // Reading the message type
            if (byte == ',') {
                // Transition to STATE_PAYLOAD when ',' is received
                ps->state = STATE_PAYLOAD;
                ps->msg_type[ps->index_type] = '\0';    // Null-terminate the message type string
                ps->index_payload = 0;          // initialize properly the index
                
            } else if (ps->index_type == 6) {   // error message to long
                ps->state = STATE_DOLLAR;       // Reset to the initial state
                ps->index_type = 0;             // Reset index
                
			} else if (byte == '*') {
				ps->state = STATE_DOLLAR; // get ready for a new message
                ps->msg_type[ps->index_type] = '\0';
				ps->msg_payload[0] = '\0'; // no payload
                //toggleLed(2);
                return NEW_MESSAGE;
                
            } else {
                ps->msg_type[ps->index_type] = byte; // ok!
                ps->index_type++; // increment for the next time;
            }
            break;
            
        case STATE_PAYLOAD: // REading the message payload
            if (byte == '*') {
                ps->state = STATE_DOLLAR; // get ready for a new message
                ps->msg_payload[ps->index_payload] = '\0';
                return NEW_MESSAGE;
                
            } else if (ps->index_payload == 100) { // error
                ps->state = STATE_DOLLAR;   // Reset to initial state
                ps->index_payload = 0;      // Reset payload index
                
            } else {
                ps->msg_payload[ps->index_payload] = byte; // ok!
                ps->index_payload++; // increment for the next time;
            }
            break;
    }
    return NO_MESSAGE;
}

int extract_integer(const char* str) {
	int i = 0, number = 0, sign = 1;
	
	if (str[i] == '-') {
		sign = -1;
		i++;
	}
	else if (str[i] == '+') {
		sign = 1;
		i++;
	}
	while (str[i] != ',' && str[i] != '\0') {
		number *= 10; // multiply the current number by 10;
		number += str[i] - '0'; // converting character to decimal number
		i++;
	}
	return sign*number;
}		

int next_value(const char* msg, int i) {
	while (msg[i] != ',' && msg[i] != '\0') { i++; }
	if (msg[i] == ',')
		i++;
	return i;
}

void init_parser(parser_state *pstate){
    pstate->state = STATE_DOLLAR;
    pstate->index_type = 0;
    pstate->index_payload = 0;
}
