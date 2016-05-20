/******************************************************************************
  Filename:       message_stack.c
  Revised:        $Date: 2015-11-24 $
  Revision:       $Revision: A1 $
  Author:         $Author: Michael Fay $

  Description:    Message stack to allow for easier communication

******************************************************************************/

/******************************************************************************
 * INCLUDES
 */

#include "packet.h" 

/******************************************************************************
 * MACROS
 */

/******************************************************************************
 * CONSTANTS AND DEFINES
 */

/******************************************************************************
 * TYPEDEFS
 */

/******************************************************************************
 * LOCAL VARIABLES
 */
static stack_tx_base;
static stack_tx_addr;

static current_measure; 
static current_frame_id;

/******************************************************************************
 * LOCAL FUNCTIONS
 */

//increment current_measure

//increment current frame id

//increment stack tx addr



/******************************************************************************
 * GLOBAL VARIABLES
 */

/******************************************************************************
 * GLOBAL FUNCTIONS
 */



//TODO break up into global and local ftns

#if  defined(__TRANSMIT_DEVICE) || defined(__BI_DIRECTIONAL)   //if need both RX/TX
//TODO create a function to add to TX stack

//TODO create a function to verify stack integrity
 	//packet id[i+1] >packet id[i]
 	//measure[i + 1] >= measure[i]

//TODO create a function to verify section of stack integrity
  	//packet id[i+1] >packet id[i]
 	//measure[i + 1] >= measure[i]

//TODO create function to remove a frame from TX stack

//TODO ftn to send full measure.

#endif			
 										//if need only RX


/*******************************************************************************/

