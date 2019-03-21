#include <aclib.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


void print_array(array_list * list)
{
	for(int i=0; i< array_list_size(list); i++)
	{
		printf("%d\n", *((int*)array_list_get(list, i)));
	}

	putchar('\n');
}


int main()
{
	array_list * list = array_list_new();

	for(int i =0; i<10; i++)
	{
		int * val = (int *) malloc(sizeof(int));
		*val = i;

		array_list_add(list, val);
	}

	print_array(list);

	int * val = (int *) malloc(sizeof(int));
	*val = 50;

	array_list_add_to_index(list, 10, val);



	print_array(list);

	array_list_destructor_with_release_v2(list, free);

	return 0;
}
