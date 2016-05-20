/*Communication framework for UNL CSE436 Fall*/
/* Developed by: Michael Fay */


/* Header Includes requred for package to work */
#ifndef stdint
	#include <stdint.h> //for uintx_t
#endif

#ifndef stdbool
	#include <stdbool.h> //for uintx_t
#endif

/* Type deff's for the project. Using typedef to allow for explicit sizing */
#ifndef byte
	typedef uint8_t  byte;
#endif

#ifndef word
	typedef uint16_t  word;
#endif

#ifndef dword
	typedef uint32_t  dword;
#endif

#ifndef qword
	typedef uint64_t  qword;
#endif

#ifdef Message_t
	#undef Message //Forcing the below data structure for messages
	#pragma message ( "Redclaration of Message_t struct in Communication/message/datatype." )
#endif

#ifndef Message_t

	/* Structure and type for message layout */
	typedef struct Message_t
	{
		byte destination_id;
		byte source_id;
		byte opcode;

		byte datalength; 
		byte crc[4];

		void * data;
	} Message_t;

#endif

#ifndef Stack_Message

	/* Structure and type for message layout */
	typedef struct Stack_Message
	{
		Message_t  message;     //message that needs to be transmitted
		dword      packet_id;   //packet id
		dword      measure;     //signifies which measure packet belongs to
	} Stack_Message;

#endif