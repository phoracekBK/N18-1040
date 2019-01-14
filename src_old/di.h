#ifndef _DATA_LAYER_H_
#define _DATA_LAYER_H_

/**
** @file di.h
**
** Container for all shared objects in program strucutre
**
*/ 

/**
**
** @defgroup data_layer
**
** Container for all shared object in program structure. 
**
*/ 

/**************************** End of header *************************************************************/
/**************************** Includes ******************************************************************/
#include <stdlib.h>
#include <stdint.h>


/**************************** End of include section ***************************************************/
/**************************** Constants and macro definitions ******************************************/

/**
** @ingroup data_layer
** @enum _data_layer_items_
** Named indexis for addressing shared objects
*/ 
enum _di_index_
{
	/* named indexis */
	di_core,
	di_cli,
	di_gui,	

	di_size		 			/**< Number of shared data items in program structure */
};
//typedef enum _di_index_ di_index;
/*************************** End of section with constants and macros section definition ***************/
/*************************** Structures declarations ***************************************************/



/*************************** End of section with structures definitions *******************************/
/*************************** Functions declarations ***************************************************/
/**
** @ingroup DI
** @fn void di_init()
**
** Initialization for dependency injection layer.
*/ 
extern void di_init();

/**
** @ingroup DI
** @fn void * di_ref(data_layer_items index)
** Funtion for return shared object on given position.
**
** @param index Index to array with shared objects
** @return object on given position in array with object or NULL
*/ 
extern void * di_ref(enum _di_index_ index);

/**
** @ingroup DI
** @fn void di_set(data_layer_items index, void * item)
** Interface function for setting the object on current position given data_layer_intems index.
**
** @param index Index to array with object
** @param item Pointer to shared object
*/ 
extern void di_set(enum _di_index_ index_, void * item);

#endif

/**************************** End of file di.h **********************************************/




