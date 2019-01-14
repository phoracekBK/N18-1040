/**
** @file di.c
**
** @author Petr Horacek
**
*/

/**************************** End of header *************************************************************/
/**************************** Includes ******************************************************************/
#include "di.h"

/**************************** End of include section ***************************************************/
/**************************** Constants and macro definitions ******************************************/
static void ** di_array;

/*************************** End of section with constants and macros section definition ***************/
/*************************** Structures definitions ****************************************************/

/*************************** End of section with structures definitions *******************************/
/*************************** Functions declarations ***************************************************/

/**
** @ingroup DI
** Call di_init function before save data item into di layer.
*/ 
void di_init()
{
	if(di_array == NULL)
	{
		di_array = (void**) malloc(sizeof(void *)*di_size);

		int index;
		for(index = 0; index < di_size; index++)
		{
			di_array[index] = NULL;
		}
	}
}

void * di_ref(enum _di_index_ index)
{
	return di_array[index];
}

void di_set(enum _di_index_ index, void * ref)
{
	di_array[index] = ref;
}

/*************************** End of file di.c *************************************************/
