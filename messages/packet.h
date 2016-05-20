/*Communication framework for UNL CSE436 Fall*/
/* Developed by: Michael Fay */

#ifndef TX_STACK_SIZE
	#define TX_STACK_SIZE 10
#endif

#ifndef RX_STACK_SIZE
	#define RX_STACK_SIZE 10
#endif

/* Header Includes requred for package to work */ 
#ifndef datatype
	#include "datatype.h" //for byte
#endif

/* Packet TX Stack */
Stack_Message TX_Stack[TX_STACK_SIZE];

/* Packet RX Stack */
Stack_Message TX_Stack[TX_STACK_SIZE];