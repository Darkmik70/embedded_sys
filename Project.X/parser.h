/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: RomiC.
 * Comments:
 * Revision history: 
 */

#ifndef PARSER_H
#define	PARSER_H

#define STATE_DOLLAR  (1)   // we discard everything until a dollar is found
#define STATE_TYPE    (2)   // we are reading the type of msg until a comma is found
#define STATE_PAYLOAD (3)   // we read the payload until an asterix is found
#define NEW_MESSAGE (1)     // new message received and parsed completely
#define NO_MESSAGE (0)      // no new messages

typedef struct { 
	int state;
	char msg_type[6];       // type is 5 chars + string terminator
	char msg_payload[100];  // assume payload cannot be longer than 100 chars
	int index_type;
	int index_payload;
} parser_state;

/*
Requires a pointer to a parser state, and the byte to process.
returns NEW_MESSAGE if a message has been successfully parsed.
The result can be found in msg_type and msg_payload.
Parsing another byte will override the contents of those arrays.
*/
int parse_byte(parser_state* ps, char byte);

/*
Takes a string as input, and converts it to an integer. Stops parsing when reaching
the end of string or a ","
the result is undefined if a wrong string is passed
*/
int extract_integer(const char* str);

/*
The function takes a string, and an index within the string, and returns the index where the next data can be found
Example: with the string "10,20,30", and i=0 it will return 3. With the same string and i=3, it will return 6.
*/  
int next_value(const char* msg, int i);

void init_parser(parser_state *pstate);

#endif	/* PARSER_H */